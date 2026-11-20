#!/usr/bin/env python3
"""Extract the streamed GSM 06.10 speech clips ("MORT" streams) of Elmo's Letter Adventure /
Elmo's Number Journey.

usage:
  extract_speech.py ROM OUTDIR            decode every clip to OUTDIR/<game>_<romoff>.wav, dump frame
                                          parameters to OUTDIR/<game>_<romoff>.params.json, write
                                          OUTDIR/clips.tsv (id, ROM range, frames, duration, references)
  extract_speech.py check ROM             re-pack every clip's parameters into the bit stream and compare
                                          with the ROM bytes (lossless proof); exit 1 on any mismatch
  extract_speech.py list ROM              print the clip table only

No ROM content is embedded here: the game is identified by its ROM size/layout (see GAMES) and every
clip is found by walking the speech region.
"""
import json, os, struct, sys, wave

# Speech region per game (ROM offsets) and where the program's .data lives (for reference search).
GAMES = {
    # key: name, region start/end (the region is the run of MORT streams), .data/.rodata ROM range,
    #      RAM-ROM delta, ROM header name check
    'letter': dict(data=(0x6D5AC, 0x7C6B0), delta=0x7FFFF400),
    'number': dict(data=(0x6EF1C, 0x7CB20), delta=0x7FFFF400),
}
MAGIC = b'MORT'


def detect(rom):
    name = rom[0x20:0x34].decode('ascii', 'replace').upper()
    if 'NUMBER' in name:
        return 'number'
    if 'LETTER' in name:
        return 'letter'
    # fall back on the size of .text: Letter's .data starts at 0x6D5AC
    return 'letter'


def find_clips(rom):
    """Return list of (rom_offset, header dict). Clips are 16-byte aligned MORT streams."""
    clips = []
    o = rom.find(MAGIC)
    while o != -1:
        if o % 16 == 0:
            w1, w2 = struct.unpack('>II', rom[o + 4:o + 12])
            frames, rate, nwords = w1 >> 16, w1 & 0xFFFF, w2
            if 0 < frames and rate in (8000, 11025, 16000, 22050) and 0 < nwords < 0x100000 \
                    and o + nwords * 4 <= len(rom):
                clips.append((o, dict(frames=frames, rate=rate, words=nwords)))
                o = rom.find(MAGIC, o + nwords * 4)
                continue
        o = rom.find(MAGIC, o + 1)
    return clips


# ---------------------------------------------------------------- bit stream (func_80069E90)
FIELDS_LAR = (6, 6, 5, 5, 4, 4, 3, 3)
FIELDS_SUB = (7, 2, 2, 6) + (3,) * 13  # Nc, bc, Mc, xmaxc, xMc[13]


class BitReader:
    """32-bit big-endian words, bits taken LSB first (as func_80069E90 does)."""
    def __init__(self, data, bitpos):
        self.words = struct.unpack('>%dI' % (len(data) // 4), data[:len(data) // 4 * 4])
        self.pos = bitpos

    def get(self, n):
        v = 0
        for i in range(n):
            w = self.words[self.pos >> 5] if (self.pos >> 5) < len(self.words) else 0
            v |= ((w >> (self.pos & 31)) & 1) << i
            self.pos += 1
        return v


class BitWriter:
    def __init__(self):
        self.bits = []

    def put(self, v, n):
        for i in range(n):
            self.bits.append((v >> i) & 1)

    def words(self, nwords):
        out = [0] * nwords
        for i, b in enumerate(self.bits):
            if b:
                out[i >> 5] |= 1 << (i & 31)
        return out


def parse_clip(rom, off, hdr):
    """Parse one MORT stream into a list of events: ('run', 'gsm', n) / ('run', 'sil', n) and frames."""
    data = rom[off:off + hdr['words'] * 4]
    br = BitReader(data, 0x60)
    events = []
    frames_left = hdr['frames']
    voiced = silent = 0
    while frames_left > 0:
        if voiced == 0 and silent == 0:
            if br.get(1):
                silent = br.get(4) + 1
                events.append({'sil': silent})
            else:
                voiced = br.get(7) + 1
                events.append({'gsm': voiced, 'frames': []})
        if silent:
            silent -= 1
        else:
            lar = [br.get(n) for n in FIELDS_LAR]
            subs = []
            for _ in range(4):
                subs.append([br.get(n) for n in FIELDS_SUB])
            events[-1]['frames'].append({'LARc': lar, 'sub': subs})
            voiced -= 1
        frames_left -= 1
    return events, br.pos


def repack(hdr, events, w0):
    bw = BitWriter()
    for ev in events:
        if 'sil' in ev:
            bw.put(1, 1); bw.put(ev['sil'] - 1, 4)
        else:
            bw.put(0, 1); bw.put(ev['gsm'] - 1, 7)
            for fr in ev['frames']:
                for v, n in zip(fr['LARc'], FIELDS_LAR):
                    bw.put(v, n)
                for s in fr['sub']:
                    for v, n in zip(s, FIELDS_SUB):
                        bw.put(v, n)
    body = bw.words(hdr['words'] - 3)
    # bits start at bit 0x60 = word 3; header words are rebuilt from the header fields
    return struct.pack('>III', w0, (hdr['frames'] << 16) | hdr['rate'], hdr['words']) + \
        struct.pack('>%dI' % len(body), *body), len(bw.bits)


# ---------------------------------------------------------------- GSM 06.10 decoder (libgsm algorithm)
def sat(x):
    return -32768 if x < -32768 else 32767 if x > 32767 else x


def mult_r(a, b):
    if a == -32768 and b == -32768:
        return 32767
    return sat((a * b + 16384) >> 15)


def gsm_add(a, b):
    return sat(a + b)


def gsm_sub(a, b):
    return sat(a - b)


def gsm_abs(a):
    return 32767 if a == -32768 else abs(a)


FAC = (18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767)
QLB = (3277, 11469, 21299, 32767)
MIC = (-32, -32, -16, -16, -8, -8, -4, -4)
B = (0, 0, 2048, -2560, 94, -1792, -341, -1144)
INVA = (13107, 13107, 13107, 13107, 19223, 17476, 31454, 29708)


class GsmDecoder:
    def __init__(self):
        self.dp0 = [0] * 280
        self.LARpp = [[0] * 8, [0] * 8]
        self.j = 0
        self.nrp = 40
        self.v = [0] * 9
        self.msr = 0

    def decode(self, LARc, subs):
        erp = [0] * 40
        wt = [0] * 160
        for k, s in enumerate(subs):
            Nc, bc, Mc, xmaxc = s[0], s[1], s[2], s[3]
            xMc = s[4:]
            # RPE decoding: APCM inverse quantization
            exp = 0
            if xmaxc > 15:
                exp = (xmaxc >> 3) - 1
            mant = xmaxc - (exp << 3)
            if mant == 0:
                exp = -4; mant = 7
            else:
                while mant <= 7:
                    mant = (mant << 1) | 1; exp -= 1
                mant -= 8
            temp1 = FAC[mant]
            temp2 = sat(6 - exp)
            temp3 = 1 << (temp2 - 1) if temp2 > 0 else 0
            xMp = []
            for x in xMc:
                t = (x << 1) - 7
                t <<= 12
                t = mult_r(temp1, sat(t))
                t = sat(t + temp3)
                xMp.append(t >> temp2 if temp2 >= 0 else t << -temp2)
            # RPE grid positioning
            erp = [0] * 40
            for i, x in enumerate(xMp):
                erp[Mc + 3 * i] = x
            # long-term synthesis
            Nr = Nc if 40 <= Nc <= 120 else self.nrp
            self.nrp = Nr
            brp = QLB[bc]
            base = 120 + k * 40
            for i in range(40):
                drpp = mult_r(brp, self.dp0[base + i - Nr])
                self.dp0[base + i] = gsm_add(erp[i], drpp)
            for i in range(40):
                wt[k * 40 + i] = self.dp0[base + i]
        # shift dp0 history: keep last 120
        self.dp0 = self.dp0[160:280] + [0] * 160
        # short-term synthesis
        LARpp_j = self.LARpp[self.j]
        LARpp_j_1 = self.LARpp[self.j ^ 1]
        for i in range(8):
            t = sat((LARc[i] + MIC[i]) << 10)
            t = gsm_sub(t, B[i] << 1)
            t = mult_r(INVA[i], t)
            LARpp_j[i] = gsm_add(t, t)
        self.j ^= 1
        out = [0] * 160
        for (lo, hi, mode) in ((0, 13, 0), (13, 27, 1), (27, 40, 2), (40, 160, 3)):
            LARp = [0] * 8
            for i in range(8):
                a, b = LARpp_j_1[i], LARpp_j[i]
                if mode == 0:
                    LARp[i] = gsm_add(a >> 2, b >> 2) + (a >> 1) if False else gsm_add(gsm_add(a >> 2, b >> 2), a >> 1)
                elif mode == 1:
                    LARp[i] = gsm_add(a >> 1, b >> 1)
                elif mode == 2:
                    LARp[i] = gsm_add(gsm_add(a >> 2, b >> 2), b >> 1)
                else:
                    LARp[i] = b
            rrp = self._lar_to_rp(LARp)
            for n in range(lo, hi):
                sri = wt[n]
                for i in range(7, -1, -1):
                    tmp1 = rrp[i]
                    tmp2 = self.v[i]
                    tmp2 = mult_r(tmp1, tmp2)
                    sri = gsm_sub(sri, tmp2)
                    tmp1 = mult_r(tmp1, sri)
                    self.v[i + 1] = gsm_add(self.v[i], tmp1)
                self.v[0] = sri
                out[n] = sri
        # postprocessing: de-emphasis, upscaling, truncation
        for n in range(160):
            tmp = mult_r(self.msr, 28180)
            self.msr = gsm_add(out[n], tmp)
            out[n] = sat(gsm_add(self.msr, self.msr)) & ~7
            if out[n] >= 0x8000:
                out[n] -= 0x10000
        return out

    @staticmethod
    def _lar_to_rp(LARp):
        rp = []
        for t in LARp:
            if t < 0:
                t = 32767 if t == -32768 else -t
                neg = True
            else:
                neg = False
            if t < 11059:
                t <<= 1
            elif t < 20070:
                t += 11059
            else:
                t = gsm_add(t >> 2, 26112)
            rp.append(-t if neg else t)
        return rp


def decode_clip(events):
    dec = GsmDecoder()
    pcm = []
    for ev in events:
        if 'sil' in ev:
            pcm.extend([0] * (160 * ev['sil']))   # the game writes zeros and leaves decoder state alone
        else:
            for fr in ev['frames']:
                pcm.extend(dec.decode(fr['LARc'], fr['sub']))
    return pcm


# ---------------------------------------------------------------- references from the program
def find_refs(rom, game, clips):
    """32-bit words in .data/.rodata equal to a clip's ROM offset. Phrase tables are {u32 rom, s16 ms}."""
    lo, hi = GAMES[game]['data']
    delta = GAMES[game]['delta']
    want = {off: [] for off, _ in clips}
    for a in range(lo, hi - 3, 4):
        w = struct.unpack('>I', rom[a:a + 4])[0]
        if w in want:
            ms = struct.unpack('>h', rom[a + 4:a + 6])[0] if a + 6 <= hi else None
            want[w].append((a + delta, ms))
    return want


def main():
    args = sys.argv[1:]
    mode = 'extract'
    if args and args[0] in ('check', 'list'):
        mode = args.pop(0)
    rom = open(args[0], 'rb').read()
    game = detect(rom)
    clips = find_clips(rom)
    refs = find_refs(rom, game, clips)
    bad = 0
    rows = []
    for idx, (off, hdr) in enumerate(clips):
        end = off + hdr['words'] * 4
        events, bitend = parse_clip(rom, off, hdr)
        dur = hdr['frames'] * 160 / hdr['rate']
        r = ';'.join('%08X/%sms' % (ra, ms) for ra, ms in refs[off]) or '-'
        rows.append('%d\t0x%06X\t0x%06X\t%d\t%d\t%.3f\t%d\t%s' % (
            idx, off, end, hdr['frames'], hdr['rate'], dur, bitend, r))
        if mode == 'check':
            w0 = struct.unpack('>I', rom[off:off + 4])[0]
            packed, nbits = repack(hdr, events, w0)
            orig = rom[off:end]
            if packed != orig:
                bad += 1
                print('MISMATCH clip %d at 0x%X' % (idx, off))
        elif mode == 'extract':
            out = args[1]
            os.makedirs(out, exist_ok=True)
            base = os.path.join(out, '%s_%06X' % (game, off))
            pcm = decode_clip(events)
            with wave.open(base + '.wav', 'wb') as wf:
                wf.setnchannels(1); wf.setsampwidth(2); wf.setframerate(hdr['rate'])
                wf.writeframes(struct.pack('<%dh' % len(pcm), *pcm))
            with open(base + '.params.json', 'w') as f:
                json.dump({'rom': off, 'magic': 'MORT', 'frames': hdr['frames'], 'rate': hdr['rate'],
                           'words': hdr['words'], 'runs': events}, f, separators=(',', ':'))
    head = 'id\trom_start\trom_end\tframes\trate\tseconds\tbits_used\trefs(ram_of_table_entry/ms)'
    if mode == 'list':
        print(head); print('\n'.join(rows))
    elif mode == 'extract':
        with open(os.path.join(args[1], '%s_clips.tsv' % game), 'w') as f:
            f.write(head + '\n' + '\n'.join(rows) + '\n')
        print('%s: %d clips written' % (game, len(clips)))
    else:
        print('%s: %d clips, %d mismatches' % (game, len(clips), bad))
        sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
