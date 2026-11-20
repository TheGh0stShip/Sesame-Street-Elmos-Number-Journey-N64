#!/usr/bin/env python3
"""Sesame Street (Elmo's Letter Adventure / Number Journey) audio extractor.

    extract_audio.py ROM OUTDIR          extract samples (WAV + JSON), banks (JSON), songs and the
                                         sound-effect bank (text dump + MIDI)
    extract_audio.py check ROM OUTDIR    re-assemble every bank / song / effect dump and re-encode every
                                         sample from its WAV, compare with the ROM bytes
    extract_audio.py map ROM             print MAP.tsv lines for the audio territory

The engine is Software Creations' libmus ("N64 PtrTables"/"N64 WaveTables" banks, command-byte songs)
on libultra n_audio. Formats: see ../FORMAT.md. The tool holds no ROM content: only offsets taken
from the game code (song file bounds, which are ROM-address constants in the code).
"""
import json, os, struct, sys, wave

# ---------------------------------------------------------------- per-game layout (from the code)
GAMES = {
    # key: ROM offset of the first "N64 PtrTables" signature
    0x2FBD50: dict(
        name='letter',
        ptr=[0x2FBD50, 0x3E4830], wbk=[0x2FE8D0, 0x3EA160], fx=(0x3E3FB0, 0x3E4830),
        songs=[0x4D9D80, 0x4DB3E0, 0x4DD320, 0x4E32C0, 0x4EA530, 0x4EADF0, 0x4F1850, 0x4F2030,
               0x4FA460, 0x4FAC20, 0x501130, 0x501820, 0x507070, 0x507890, 0x50C970, 0x50D000,
               0x50F9E0],
        # song-format files no code references as a start address (found by the files survey,
        # confirmed by parsing): placed after the referenced ones in the numbering
        unused_starts=[0x4DD320],          # 0x4DD320 only appears as an end address
        extra_songs=[(0x2F9B60, 0x2FBD50), (0x50F9E0, 0x510A10)],
        init='800170B0', songload='8001892C'),
    0x2E6FC0: dict(
        name='number',
        ptr=[0x2E6FC0, 0x3CB880], wbk=[0x2E9A90, 0x3D0980], fx=(0x3CB0C0, 0x3CB880),
        songs=[0x4B00C0, 0x4B8E70, 0x4BA590, 0x4BC400, 0x4BCB50, 0x4C12F0, 0x4C1970, 0x4C6350,
               0x4C69A0, 0x4CB8D0, 0x4CBCD0, 0x4CFEC0, 0x4D0690, 0x4D7C90, 0x4D83F0, 0x4DF7C0,
               0x4E0870, 0x4E1090, 0x4E7620, 0x4E9810],
        init='80017520', songload='8001933C'),
}
OUTRATE = 22050          # synth output rate set by the init function (0x5622)
PPQN = 48                # tempo command: ticks/s = bpm*3*8192/120/256 = bpm*0.8 -> 48 per beat

def song_list(g):
    sb = g['songs']
    return [(sb[i], sb[i + 1]) for i in range(len(sb) - 1)] + list(g.get('extra_songs', []))

def game_of(rom):
    for k, g in GAMES.items():
        if rom[k:k + 13] == b'N64 PtrTables':
            return g
    sys.exit('unknown ROM (no N64 PtrTables at a known offset)')

def u32(d, o): return struct.unpack_from('>I', d, o)[0]
def u16(d, o): return struct.unpack_from('>H', d, o)[0]

# ---------------------------------------------------------------- ptr bank (instrument bank)
def parse_ptrbank(d, o, end):
    """Returns (regions, waves). regions: list of (off, len, kind, obj) inside the file."""
    cnt = u32(d, o + 0x20)
    obase, odet, olist = u32(d, o + 0x24), u32(d, o + 0x28), u32(d, o + 0x2C)
    regs = {0: (0x30, 'header', None), obase: (cnt, 'basenotes', None),
            odet: (4 * cnt, 'detunes', None), olist: (4 * cnt, 'wavelist', None)}
    waves = []
    for i in range(cnt):
        it = u32(d, o + olist + 4 * i)
        base, ln, ty, fl = struct.unpack_from('>IIBB', d, o + it)
        lp, bk = u32(d, o + it + 0xC), u32(d, o + it + 0x10)
        regs[it] = (0x14, 'wavetable', None)
        w = dict(index=i, item=it, base=base, len=ln, type=ty, flags=fl, loop_off=lp, book_off=bk,
                 basenote=d[o + obase + i], detune=struct.unpack_from('>b', d, o + odet + 4 * i)[0])
        if lp:
            regs[lp] = (0x2C, 'loop', None)
            w['loop'] = dict(start=u32(d, o + lp), end=u32(d, o + lp + 4), count=u32(d, o + lp + 8),
                             state=list(struct.unpack_from('>16h', d, o + lp + 12)))
        if bk and ty == 0:
            order, npred = struct.unpack_from('>ii', d, o + bk)
            regs[bk] = (8 + 16 * order * npred, 'book', None)
            w['book'] = dict(order=order, npred=npred,
                             coefs=list(struct.unpack_from('>%dh' % (8 * order * npred), d, o + bk + 8)))
        t8 = (w['basenote'] - 0x30) & 0xFF
        w['tune'] = (t8 - 256 if t8 & 0x80 else t8) + w['detune'] / 100.0   # func_8004E9E0
        waves.append(w)
    return cover(regs, end - o), waves

def cover(regs, size):
    """Sort regions and fill holes with 'raw' (zero-only holes are 'pad')."""
    out, pos = [], 0
    for off in sorted(regs):
        ln, kind, obj = regs[off]
        if off < pos:
            raise ValueError('overlap at 0x%X' % off)
        if off > pos:
            out.append((pos, off - pos, 'raw', None))
        out.append((off, ln, kind, obj))
        pos = off + ln
    if pos < size:
        out.append((pos, size - pos, 'raw', None))
    return out

# ---------------------------------------------------------------- VADPCM
def expand_book(order, npred, coefs):
    """coef table per predictor: 8 rows x (order+8) columns, as in the n_audio ADPCM decoder."""
    tabs = []
    for p in range(npred):
        bk = [coefs[(p * order + k) * 8:(p * order + k) * 8 + 8] for k in range(order)]
        rows = []
        for i in range(8):
            r = [bk[k][i] for k in range(order)]
            for m in range(8):
                r.append(2048 if m == i else (bk[order - 1][i - m - 1] if m < i else 0))
            rows.append(r)
        tabs.append(rows)
    return tabs

def sx4(n): return n - 16 if n & 8 else n

def clamp16(v): return -32768 if v < -32768 else 32767 if v > 32767 else v

def adpcm_decode(data, book):
    order = book['order']
    tabs = expand_book(order, book['npred'], book['coefs'])
    out = []
    last = [0] * 8
    for f in range(len(data) // 9):
        fr = data[9 * f:9 * f + 9]
        scale = 1 << (fr[0] >> 4)
        tab = tabs[fr[0] & 15]
        ins = []
        for b in fr[1:]:
            ins.append(sx4(b >> 4) * scale); ins.append(sx4(b & 15) * scale)
        for h in range(2):
            vec = last[8 - order:] + ins[8 * h:8 * h + 8]
            cur = []
            for i in range(8):
                acc = sum(a * b for a, b in zip(tab[i], vec))
                cur.append(clamp16(acc >> 11))          # floor division by 2048
            out.extend(cur)
            last = cur
    return out

def adpcm_encode_check(pcm, book, orig):
    """Re-encode decoded PCM frame by frame. For each frame the residuals are exactly determined
    once (scale, predictor) is chosen, so we search all pairs and keep the first that rebuilds the
    frame (original header tried first). Returns (bytes, n_frames, n_exact_by_search)."""
    order = book['order']
    tabs = expand_book(order, book['npred'], book['coefs'])
    nf = len(orig) // 9
    res = bytearray()
    last = [0] * 8
    for f in range(nf):
        tgt = pcm[16 * f:16 * f + 16]
        h0 = orig[9 * f]
        cands = [h0] + [s << 4 | p for s in range(13) for p in range(book['npred']) if (s << 4 | p) != h0]
        got = None
        for hdr in cands:
            sc = 1 << (hdr >> 4)
            if (hdr & 15) >= len(tabs):
                continue
            tab = tabs[hdr & 15]
            ins, ok, prev = [], True, last
            for h in range(2):
                vec = prev[8 - order:] + [0] * 8
                for i in range(8):
                    partial = sum(a * b for a, b in zip(tab[i], vec))
                    want = tgt[8 * h + i]
                    r = want - (partial >> 11)
                    if r % sc:
                        ok = False; break
                    n = r // sc
                    if n < -8 or n > 7:
                        ok = False; break
                    vec[order + i] = n * sc
                    ins.append(n)
                    if clamp16((partial + 2048 * n * sc) >> 11) != want:
                        ok = False; break
                if not ok:
                    break
                prev = tgt[8 * h:8 * h + 8]
            if ok:
                got = bytes([hdr]) + bytes(((ins[2 * k] & 15) << 4) | (ins[2 * k + 1] & 15) for k in range(8))
                break
        if got is None:
            got = b'\xff' * 9
        res += got
        last = tgt[8:16]
    return bytes(res)

def write_wav(path, pcm, rate, loop=None):
    import io
    body = struct.pack('<%dh' % len(pcm), *pcm)
    with wave.open(path, 'wb') as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(rate); w.writeframes(body)
    if loop and loop['end'] > loop['start']:
        smpl = struct.pack('<9I', 0, 0, int(1e9 / rate), 60, 0, 0, 0, 1, 0)
        smpl += struct.pack('<6I', 0, 0, loop['start'], loop['end'] - 1, 0,
                            0 if loop['count'] == 0xFFFFFFFF else loop['count'])
        with open(path, 'r+b') as f:
            f.seek(0, 2)
            f.write(b'smpl' + struct.pack('<I', len(smpl)) + smpl)
            size = f.tell()
            f.seek(4); f.write(struct.pack('<I', size - 8))

def read_wav(path):
    with wave.open(path, 'rb') as w:
        n = w.getnframes()
        return list(struct.unpack('<%dh' % n, w.readframes(n)))

# ---------------------------------------------------------------- command streams
# opcode -> (name, argument spec). spec letters: b=u8 s=s8 w=u16 v=varlen(1|2 bytes) k=marker
OPS = {
    0x80: ('stop', ''), 0x81: ('wave', 'v'), 0x82: ('port', 'b'), 0x83: ('portoff', ''),
    0x84: ('envdef', 'bbbbbbb'), 0x85: ('tempo', 'b'), 0x86: ('cutoff', 'w'), 0x87: ('endit', 'b'),
    0x88: ('vibup', 'bbb'), 0x89: ('vibdown', 'bbb'), 0x8A: ('viboff', ''), 0x8B: ('length', 'v'),
    0x8C: ('ignore', ''), 0x8D: ('trans', 's'), 0x8E: ('ignoretrans', ''), 0x8F: ('distort', 's'),
    0x90: ('env', 'v'), 0x91: ('envoff', ''), 0x92: ('envon', ''), 0x93: ('troff', ''),
    0x94: ('tron', ''), 0x95: ('for', 'b'), 0x96: ('next', ''), 0x97: ('wobble', 'bbb'),
    0x98: ('wobbleoff', ''), 0x99: ('velon', ''), 0x9A: ('veloff', ''), 0x9B: ('velocity', 'b'),
    0x9C: ('pan', 'b'), 0x9D: ('stereo', 'bb'), 0x9E: ('drums', 'v'), 0x9F: ('drumsoff', ''),
    0xA0: ('print', 'b'), 0xA1: ('goto', 'www'), 0xA2: ('reverb', 'b'), 0xA3: ('randnote', 'bb'),
    0xA4: ('randvol', 'bb'), 0xA5: ('randpan', 'bb'), 0xA6: ('volume', 'b'), 0xA7: ('startfx', 'v'),
    0xA8: ('bendrange', 'b'), 0xA9: ('sweep', 'b'), 0xAA: ('changefx', 'b'), 0xAB: ('marker', 'bv'),
    0xAC: ('length0', ''),
}
BYNAME = {v[0]: k for k, v in OPS.items()}

def rd_var(d, p):
    c = d[p]
    if c < 0x80:
        return c, p + 1, ''
    v = ((c & 0x7F) << 8) | d[p + 1]
    return v, p + 2, ('L' if v < 0x80 else '')

def fmt_var(v, tag): return '%d%s' % (v, tag)

def disasm_track(d, p, end, vel):
    """Linear decode of a track/effect stream d[p:end]. vel: velocity-per-note mode at stream start.
    Returns (lines, stop_pos). Stops after 'stop'/'goto', after the 'next' that closes an endless
    (count 255) loop, or when the stream would cross end."""
    fixed, ign = 0, 0
    loops = []
    lines = []
    while p < end:
        c = d[p]
        try:
            if c >= 0x80:
                if c not in OPS:
                    break
                name, spec = OPS[c]
                q, args = p + 1, []
                for a in spec:
                    if a == 'b': args.append('%d' % d[q]); q += 1
                    elif a == 's': args.append('%d' % struct.unpack_from('b', d, q)[0]); q += 1
                    elif a == 'w': args.append('0x%04X' % u16(d, q)); q += 2
                    elif a == 'v':
                        v, q, t = rd_var(d, q); args.append(fmt_var(v, t))
                if q > end:
                    break
                if c == 0x95: loops.append(int(args[0]))
                elif c == 0x96 and loops:
                    if loops.pop() == 0xFF:          # closes an endless loop: nothing after it runs
                        lines.append(' '.join([name] + args)); p = q
                        break
                if c == 0x8B: fixed = int(args[0].rstrip('L'))
                elif c == 0xAC: fixed = 0
                elif c == 0x8C: ign = 1
                elif c == 0x99: vel = 1
                elif c in (0x9A, 0x9B): vel = 0
                lines.append(' '.join([name] + args)); p = q
                if c in (0x80, 0xA1):
                    break
            else:
                q, parts = p + 1, ['note', '%d' % c]
                if vel:
                    v = d[q]; q += 1
                    if v >= 0x80:
                        parts.append('vel!%d' % (v & 0x7F)); vel = 0
                    else:
                        parts.append('vel=%d' % v)
                if fixed == 0 or ign:
                    ign = 0
                    v, q, t = rd_var(d, q); parts.append('len=' + fmt_var(v, t))
                if q > end:
                    break
                lines.append(' '.join(parts)); p = q
        except IndexError:
            break
    return lines, p

def asm_var(s):
    if s.endswith('L'):
        v = int(s[:-1]); return bytes([0x80 | (v >> 8), v & 0xFF])
    v = int(s)
    return bytes([v]) if v < 0x80 else bytes([0x80 | (v >> 8), v & 0xFF])

def asm_track_line(ln):
    t = ln.split()
    if t[0] == 'note':
        b = bytes([int(t[1])])
        for x in t[2:]:
            if x.startswith('vel='): b += bytes([int(x[4:])])
            elif x.startswith('vel!'): b += bytes([0x80 | int(x[4:])])
            elif x.startswith('len='): b += asm_var(x[4:])
        return b
    op = BYNAME[t[0]]
    b = bytes([op])
    for a, s in zip(OPS[op][1], t[1:]):
        if a == 'b': b += bytes([int(s)])
        elif a == 's': b += struct.pack('b', int(s))
        elif a == 'w': b += struct.pack('>H', int(s, 16))
        elif a == 'v': b += asm_var(s)
    return b

def disasm_ctl(d, p, end):
    """volume / pitch-bend stream: byte<0x80 = value for 1 tick; byte>=0x80 = value&0x7F then a
    varlen hold count (held count+2 ticks). No terminator: runs to the next region."""
    lines = []
    while p < end:
        c = d[p]
        if c < 0x80:
            lines.append('v %d' % c); p += 1
        else:
            if p + 1 >= end: break
            v, q, t = rd_var(d, p + 1)
            if q > end: break
            lines.append('v %d hold %s' % (c & 0x7F, fmt_var(v, t))); p = q
    return lines, p

def asm_ctl_line(ln):
    t = ln.split()
    if len(t) == 2: return bytes([int(t[1])])
    return bytes([0x80 | int(t[1])]) + asm_var(t[3])

def hexlines(b):
    return ['hex ' + b[i:i + 32].hex() for i in range(0, len(b), 32)]

# ---------------------------------------------------------------- song file
SONG_TABLES = ['chan', 'vol', 'bend', 'env', 'drum', 'wavemap', 'master']

def song_regions(s):
    n, nw = u32(s, 4), u32(s, 8)
    offs = [u32(s, 0xC + 4 * i) for i in range(7)]
    regs = {0: (0x2C, 'header', None)}
    regs[offs[0]] = (4 * n, 'ptrs', 'chan'); regs[offs[1]] = (4 * n, 'ptrs', 'vol')
    regs[offs[2]] = (4 * n, 'ptrs', 'bend'); regs[offs[5]] = (2 * nw, 'wavemap', None)
    starts = {}
    for o, kind in ((offs[3], ('envs', None)), (offs[4], ('drums', None)), (offs[6], ('track', 'master'))):
        if o and o not in regs and o not in starts:
            starts[o] = kind
    for i in range(n):
        for k, kind in ((0, 'track'), (1, 'vol'), (2, 'bend')):
            o = u32(s, offs[k] + 4 * i)
            if o and o not in starts:
                starts[o] = (kind, i)
    return n, nw, offs, regs, starts

def dump_song(s, title, vel0=1):
    n, nw, offs, regs, starts = song_regions(s)
    allstarts = sorted(set(list(regs) + list(starts)))
    out = ['# %s  size 0x%X  tracks %d  waves %d' % (title, len(s), n, nw)]
    pos = 0
    for i, st in enumerate(allstarts):
        if st > pos:
            out.append('@%04X raw' % pos); out += hexlines(s[pos:st])
        nxt = allstarts[i + 1] if i + 1 < len(allstarts) else len(s)
        if st in regs:
            ln, kind, tag = regs[st]
            if kind == 'header':
                out.append('@%04X header' % st)
                out.append('version 0x%08X' % u32(s, 0)); out.append('tracks %d' % n)
                out.append('waves %d' % nw)
                for k, nm in enumerate(SONG_TABLES): out.append('%s 0x%X' % (nm, offs[k]))
                out.append('relocated %d' % u32(s, 0x28))
            elif kind == 'ptrs':
                out.append('@%04X ptrs %s' % (st, tag))
                out.append(' '.join('0x%X' % u32(s, st + 4 * j) for j in range(n)))
            elif kind == 'wavemap':
                out.append('@%04X wavemap' % st)
                out.append(' '.join('%d' % u16(s, st + 2 * j) if u16(s, st + 2 * j) != 0xFFFF else '-'
                                    for j in range(nw)))
            pos = st + ln
            if pos < nxt:
                out.append('@%04X raw' % pos); out += hexlines(s[pos:nxt])
            pos = nxt
            continue
        kind, tag = starts[st]
        if kind == 'envs' or kind == 'drums':
            sz = 7 if kind == 'envs' else 6
            cnt = (nxt - st) // sz
            out.append('@%04X %s' % (st, kind))
            for j in range(cnt):
                e = s[st + sz * j:st + sz * j + sz]
                if kind == 'envs':
                    out.append('env %s' % ' '.join('%d' % x for x in e))
                else:
                    out.append('drum wave=%d env=%d pan=%d note=%d' % (u16(e, 0), u16(e, 2), e[4], e[5]))
            p = st + sz * cnt
        elif kind == 'track':
            out.append('@%04X track %s' % (st, tag))
            lines, p = disasm_track(s, st, nxt, vel0)
            out += ['  ' + x for x in lines]
        else:
            out.append('@%04X %s %s' % (st, kind, tag))
            lines, p = disasm_ctl(s, st, nxt)
            out += ['  ' + x for x in lines]
        if p < nxt:
            out.append('@%04X raw' % p); out += hexlines(s[p:nxt])
        pos = nxt
    return '\n'.join(out) + '\n'

def asm_song(text):
    buf = bytearray()
    kind = None
    hdr = {}
    for ln in text.splitlines():
        if not ln or ln.startswith('#'):
            continue
        if ln.startswith('@'):
            if kind == 'header': buf += pack_song_header(hdr)
            off = int(ln[1:5], 16)
            if off != len(buf):
                raise ValueError('offset mismatch at %s (have 0x%X)' % (ln, len(buf)))
            kind = ln.split()[1]; hdr = {}
            continue
        t = ln.split()
        if kind == 'header': hdr[t[0]] = int(t[1], 0)
        elif kind == 'raw': buf += bytes.fromhex(t[1])
        elif kind == 'ptrs': buf += b''.join(struct.pack('>I', int(x, 16)) for x in t)
        elif kind == 'wavemap': buf += b''.join(struct.pack('>H', 0xFFFF if x == '-' else int(x)) for x in t)
        elif kind == 'envs': buf += bytes(int(x) for x in t[1:])
        elif kind == 'drums':
            f = dict(x.split('=') for x in t[1:])
            buf += struct.pack('>HHBB', int(f['wave']), int(f['env']), int(f['pan']), int(f['note']))
        elif kind == 'track': buf += asm_track_line(ln)
        elif kind in ('vol', 'bend'): buf += asm_ctl_line(ln)
        elif kind == 'fxheader': hdr[t[0]] = int(t[1], 0)
        elif kind == 'fxtable': buf += struct.pack('>II', int(t[1], 16), int(t[2]))
        else: raise ValueError(kind)
    if kind == 'header': buf += pack_song_header(hdr)
    return bytes(buf)

def pack_song_header(h):
    return struct.pack('>III7II', h['version'], h['tracks'], h['waves'],
                       *[h[n] for n in SONG_TABLES], h['relocated'])

# ---------------------------------------------------------------- sound-effect bank
def dump_fx(f, title):
    cnt = u32(f, 0); nw = u32(f, 8); owm = u32(f, 0x14)
    out = ['# %s  size 0x%X  effects %d  waves %d' % (title, len(f), cnt, nw), '@0000 raw']
    out += hexlines(f[0:0x18])
    out.append('@0018 raw')
    ents = [(u32(f, 0x18 + 8 * i), u32(f, 0x1C + 8 * i)) for i in range(cnt)]
    out += hexlines(f[0x18:0x18 + 8 * cnt])
    out += ['# fx%d at 0x%X priority %d' % (i, o, pr) for i, (o, pr) in enumerate(ents)]
    starts = sorted(set([o for o, _ in ents] + [owm]))
    pos = 0x18 + 8 * cnt
    fxid = {o: i for i, (o, _) in reversed(list(enumerate(ents)))}
    for i, st in enumerate(starts):
        nxt = starts[i + 1] if i + 1 < len(starts) else len(f)
        if st > pos:
            out.append('@%04X raw' % pos); out += hexlines(f[pos:st])
        if st == owm:
            out.append('@%04X wavemap' % st)
            out.append(' '.join('%d' % u16(f, st + 2 * j) for j in range(nw)))
            p = st + 2 * nw
        else:
            out.append('@%04X track fx%d' % (st, fxid[st]))
            lines, p = disasm_track(f, st, nxt, 0)
            out += ['  ' + x for x in lines]
        if p < nxt:
            out.append('@%04X raw' % p); out += hexlines(f[p:nxt])
        pos = nxt
    return '\n'.join(out) + '\n', ents

# ---------------------------------------------------------------- MIDI rendering (approximate)
def vlq(n):
    b = [n & 0x7F]; n >>= 7
    while n: b.append(0x80 | (n & 0x7F)); n >>= 7
    return bytes(reversed(b))

def render_track(d, start, vel, ch, tempo_ev, startfx, volp=None, bendp=None, cap=200000):
    """Execute a stream (loops expanded once for infinite counts, goto ends it).
    Returns list of (tick, priority, bytes)."""
    ev = []
    t, p = 0, start
    fixed = ign = 0
    stack = []
    br = 1 / 32.0
    trans = 0
    curvel = 0x7F
    cut = endit = 0
    steps = 0
    ev.append((0, 0, bytes([0xB0 | ch, 101, 0, 0xB0 | ch, 100, 0, 0xB0 | ch, 6, 12])))
    while p is not None and steps < 20000 and t < cap and p < len(d):
        steps += 1
        c = d[p]
        if c >= 0x80:
            name, spec = OPS.get(c, ('?', ''))
            if name == '?': break
            q, args = p + 1, []
            for a in spec:
                if a in 'bs': args.append(d[q] if a == 'b' else struct.unpack_from('b', d, q)[0]); q += 1
                elif a == 'w': args.append(u16(d, q)); q += 2
                else: v, q, _ = rd_var(d, q); args.append(v)
            p = q
            if c == 0x80 or c == 0xA1: p = None
            elif c == 0x81: ev.append((t, 1, bytes([0xC0 | ch, args[0] & 0x7F])))
            elif c == 0x85: tempo_ev.append((t, args[0]))
            elif c == 0x86: cut, endit = args[0], 0
            elif c == 0x87: cut, endit = 0, args[0]
            elif c == 0x8B: fixed = args[0]
            elif c == 0xAC: fixed = 0
            elif c == 0x8C: ign = 1
            elif c == 0x8D: trans = args[0]
            elif c == 0x99: vel = 1
            elif c == 0x9A: vel = 0
            elif c == 0x9B: vel = 0; curvel = args[0]
            elif c == 0x9C: ev.append((t, 1, bytes([0xB0 | ch, 10, args[0] >> 1])))
            elif c == 0xA6: ev.append((t, 1, bytes([0xB0 | ch, 11, args[0] & 0x7F])))
            elif c == 0xA7: startfx.append(args[0])
            elif c == 0x95: stack.append([p, args[0]])
            elif c == 0x96 and stack:
                top = stack[-1]
                if top[1] == 0xFF:
                    p = None             # infinite loop: the MIDI rendering stops after one pass
                else:
                    top[1] -= 1
                    if top[1] == 0: stack.pop()
                    else: p = top[0]
        else:
            q = p + 1
            v = None
            if vel:
                v = d[q]; q += 1
                if v >= 0x80: v &= 0x7F; vel = 0
                curvel = v
            if fixed == 0 or ign:
                ign = 0; ln, q, _ = rd_var(d, q)
            else:
                ln = fixed
            p = q
            if v is None: v = curvel
            if c != 0x60:
                dur = cut if cut else max(1, ln - endit)
                key = max(0, min(127, c + 12 + trans))
                ev.append((t, 2, bytes([0x90 | ch, key, max(1, v)])))
                ev.append((t + dur, 0, bytes([0x80 | ch, key, 0])))
            t += ln
    end = t
    for sp, cc in ((volp, 7), (bendp, None)):
        if sp is None: continue
        tt, q, last = 0, sp, None
        while tt < end and q < len(d):
            c = d[q]
            if c < 0x80:
                v, n, q = c, 1, q + 1
            else:
                v = c & 0x7F; n, q, _ = rd_var(d, q + 1); n += 2
            if v != last:
                if cc: ev.append((tt, 1, bytes([0xB0 | ch, cc, v])))
                else:
                    semis = (v - 64) * br
                    pb = max(0, min(16383, int(8192 + semis / 12.0 * 8192)))
                    ev.append((tt, 1, bytes([0xE0 | ch, pb & 0x7F, pb >> 7])))
                last = v
            tt += n
    return [e for e in ev if e[2]]

def write_midi(path, tracks, tempo_ev):
    def chunk(evs):
        evs.sort(key=lambda e: (e[0], e[1]))
        b, last = bytearray(), 0
        for t, _, data in evs:
            # split multi-message blobs (3-byte channel messages)
            i = 0
            while i < len(data):
                st = data[i]; n = 2 if (st & 0xF0) in (0xC0, 0xD0) else 3
                if st == 0xFF: n = 3 + data[i + 2]
                b += vlq(t - last) + data[i:i + n]; last = t; i += n
        b += b'\x00\xff\x2f\x00'
        return b'MTrk' + struct.pack('>I', len(b)) + b
    cond = [(0, 0, b'\xff\x51\x03' + struct.pack('>I', 500000)[1:])]
    for t, bpm in tempo_ev:
        cond.append((t, 0, b'\xff\x51\x03' + struct.pack('>I', int(60e6 / max(1, bpm)))[1:]))
    data = b'MThd' + struct.pack('>IHHH', 6, 1, len(tracks) + 1, PPQN) + chunk(cond)
    for evs in tracks: data += chunk(evs)
    open(path, 'wb').write(data)

def song_midi(s, path):
    n, nw, offs, regs, starts = song_regions(s)
    tempo_ev, fx, tracks = [], [], []
    render_track(s, offs[6], 1, 15, tempo_ev, fx)          # master: timing/tempo only
    for i in range(n):
        o = u32(s, offs[0] + 4 * i)
        if not o: continue
        vo, bo = u32(s, offs[1] + 4 * i), u32(s, offs[2] + 4 * i)
        tracks.append(render_track(s, o, 1, i & 15, tempo_ev, fx, vo or None, bo or None))
    write_midi(path, tracks, tempo_ev)
    return fx

# ---------------------------------------------------------------- driver
def extract(rom, outdir, check=False):
    g = game_of(rom)
    gd = os.path.join(outdir, g['name'])
    os.makedirs(gd, exist_ok=True)
    bad = 0
    summary = dict(game=g['name'], banks=[], songs=[], fx=None)
    for bi, (po, wo) in enumerate(zip(g['ptr'], g['wbk'])):
        pend = wo
        regs, waves = parse_ptrbank(rom, po, pend)
        bdir = os.path.join(gd, 'bank%d' % (bi + 1)); os.makedirs(bdir, exist_ok=True)
        jpath = os.path.join(bdir, 'ptrbank.json')
        dump = dict(rom=po, size=pend - po, regions=[
            dict(off=o, len=l, kind=k, hex=rom[po + o:po + o + l].hex() if k in ('raw', 'header') else None)
            for o, l, k, _ in regs], waves=waves)
        if not check:
            json.dump(dump, open(jpath, 'w'), indent=1)
        else:
            bad += check_ptrbank(rom, po, pend, json.load(open(jpath)))
        seen = {}
        for w in waves:
            key = (w['base'], w['len'])
            name = 'w%03d.wav' % w['index']
            raw = rom[wo + w['base']:wo + w['base'] + w['len']]
            if key in seen:
                w['alias_of'] = seen[key]; continue
            seen[key] = w['index']
            wp = os.path.join(bdir, name)
            if w['type'] == 0:
                if not check:
                    pcm = adpcm_decode(raw, w['book'])
                    lp = w.get('loop')
                    loop = None
                    if lp: loop = dict(start=lp['start'], end=lp['end'], count=lp['count'])
                    write_wav(wp, pcm, OUTRATE, loop)
                    side = dict(index=w['index'], rom=wo + w['base'], bytes=w['len'], frames=w['len'] // 9,
                                tail_hex=raw[9 * (w['len'] // 9):].hex(), tune=w['tune'],
                                basenote=w['basenote'], detune=w['detune'], loop=w.get('loop'),
                                book=w['book'], nominal_rate=OUTRATE,
                                unity_note=-w['tune'])
                    # frames whose canonical re-encode differs from the ROM are stored verbatim
                    enc = adpcm_encode_check(pcm, w['book'], raw)
                    side['exceptions'] = {str(f): raw[9 * f:9 * f + 9].hex()
                                          for f in range(len(raw) // 9) if enc[9 * f:9 * f + 9] != raw[9 * f:9 * f + 9]}
                    json.dump(side, open(wp[:-4] + '.json', 'w'), indent=1)
                else:
                    side = json.load(open(wp[:-4] + '.json'))
                    pcm = read_wav(wp)
                    enc = bytearray(adpcm_encode_check(pcm, side['book'], raw))
                    for f, h in side['exceptions'].items():
                        enc[9 * int(f):9 * int(f) + 9] = bytes.fromhex(h)
                    enc += bytes.fromhex(side['tail_hex'])
                    if bytes(enc) != raw:
                        print('MISMATCH sample', g['name'], bi + 1, w['index']); bad += 1
            else:
                if not check:
                    pcm = list(struct.unpack('>%dh' % (len(raw) // 2), raw[:len(raw) // 2 * 2]))
                    write_wav(wp, pcm, OUTRATE)
        summary['banks'].append(dict(ptr=po, wbk=wo, waves=len(waves)))
    # effects
    fo, fe = g['fx']
    ftxt, ents = dump_fx(rom[fo:fe], '%s sound-effect bank' % g['name'])
    fpath = os.path.join(gd, 'fxbank.txt')
    if not check:
        open(fpath, 'w').write(ftxt)
    else:
        if asm_song(open(fpath).read()) != rom[fo:fe]:
            print('MISMATCH fxbank'); bad += 1
    # songs
    for i, (a, b) in enumerate(song_list(g)):
        s = rom[a:b]
        base = os.path.join(gd, 'song%02d' % i)
        if not check:
            open(base + '.txt', 'w').write(dump_song(s, '%s song%02d rom 0x%X' % (g['name'], i, a)))
            fx = song_midi(s, base + '.mid')
            summary['songs'].append(dict(index=i, rom=a, size=len(s), startfx=sorted(set(fx))))
        else:
            if asm_song(open(base + '.txt').read()) != s:
                print('MISMATCH song', i); bad += 1
    if not check:
        json.dump(summary, open(os.path.join(gd, 'summary.json'), 'w'), indent=1)
    return bad

def check_ptrbank(rom, po, pend, dump):
    buf = bytearray(pend - po)
    for r in dump['regions']:
        if r['hex'] is not None:
            buf[r['off']:r['off'] + r['len']] = bytes.fromhex(r['hex'])
    cnt = len(dump['waves'])
    hdr = rom[po:po + 0x30]
    obase, odet, olist = struct.unpack_from('>III', buf, 0x24)
    for w in dump['waves']:
        i = w['index']
        struct.pack_into('>I', buf, olist + 4 * i, w['item'])
        buf[obase + i] = w['basenote']
        struct.pack_into('>b', buf, odet + 4 * i, w['detune'])
        struct.pack_into('>IIBBxxII', buf, w['item'], w['base'], w['len'], w['type'], w['flags'],
                         w['loop_off'], w['book_off'])
        if 'loop' in w:
            l = w['loop']
            struct.pack_into('>III16h', buf, w['loop_off'], l['start'], l['end'], l['count'], *l['state'])
        if 'book' in w:
            b = w['book']
            struct.pack_into('>ii%dh' % len(b['coefs']), buf, w['book_off'], b['order'], b['npred'], *b['coefs'])
    if bytes(buf) != rom[po:pend]:
        print('MISMATCH ptrbank 0x%X' % po); return 1
    return 0

# ---------------------------------------------------------------- MAP
def map_lines(rom):
    g = game_of(rom)
    nm = g['name']
    L = []
    def add(a, b, cls, fmt, desc, ref):
        if b > a: L.append('\t'.join([nm, '0x%06X' % a, '0x%06X' % b, cls, fmt, desc, ref,
                                      'tools/extract_audio.py']))
    def padcls(a, b):
        return 'pad' if not any(rom[a:b]) else 'pad'
    sl = 'func_%s (init) -> MusInitialize-style config' % g['init']
    for bi, (po, wo) in enumerate(zip(g['ptr'], g['wbk'])):
        regs, waves = parse_ptrbank(rom, po, wo)
        user = ('default bank for sound effects: config.ptr/wbk in func_%s, relocated by func_8004E9E0 (letter)'
                % g['init']) if bi == 0 else ('song bank: loaded by func_%s, relocated by func_8004E9E0 (letter) and '
                                              'made the next-song bank by func_8004D2B0 (letter)' % g['songload'])
        add(po, wo, 'audio', 'libmus ptr bank (N64 PtrTablesV2)',
            'instrument bank %d: header, %d wave-table entries, ADPCM books and loops, base notes, detunes' % (bi + 1, len(waves)), user)
        # wave data
        add(wo, wo + 0x10, 'audio', 'N64 WaveTables header', 'wave bank %d signature' % (bi + 1),
            'sample addresses = wave bank ROM address + entry base (func_8004E9E0)')
        spans = {}
        for w in waves: spans.setdefault((w['base'], w['len']), []).append(w['index'])
        pos = wo + 0x10
        for (b, ln), idx in sorted(spans.items()):
            a = wo + b
            if a > pos:
                add(pos, a, 'pad' if not any(rom[pos:a]) else 'unknown', 'gap',
                    'gap between samples (%s)' % ('zero' if not any(rom[pos:a]) else 'non-zero, unreferenced'), 'none')
            if a < pos:
                continue
            add(a, a + ln, 'audio', 'VADPCM sample' if waves[idx[0]]['type'] == 0 else 'PCM16 sample',
                'bank%d wave %s, %d frames' % (bi + 1, ','.join(map(str, idx)), ln // 9),
                'wave-table entry of ptr bank %d' % (bi + 1))
            pos = a + ln
        nxt = sorted([x for x in g['ptr'] + [g['fx'][0], g['songs'][0]] if x > wo])[0]
        if pos < nxt:
            add(pos, nxt, 'pad', 'fill' if not any(rom[pos:nxt]) else 'fill (non-zero)',
                'alignment after wave bank %d (%s)' % (bi + 1, 'zero' if not any(rom[pos:nxt]) else
                                                       'non-zero leftover bytes, not referenced'), 'none')
    fo, fe = g['fx']
    add(fo, fe, 'audio', 'libmus effect bank', 'sound-effect bank: %d effect streams + wave map' % u32(rom, fo),
        'loaded by func_%s into RAM and given as config default fx bank; effects started by id' % g['init'])
    starts_used = set(g['songs'][:-1]) - set(g.get('unused_starts', []))
    for i, (a, b) in enumerate(song_list(g)):
        s = rom[a:b]
        ref = ('func_%s(romStart=0x%X, romEnd, ...) callers' % (g['songload'], a)) if a in starts_used \
            else 'none: no code uses 0x%X as a start (unused song)' % a
        txt = dump_song(s, '')
        last = [l for l in txt.splitlines() if l.startswith('@')][-1].split()
        end = a + int(last[0][1:], 16) if last[1] == 'raw' else b
        add(a, end, 'audio', 'libmus song', 'song%02d: %d tracks, %d waves' % (i, u32(s, 4), u32(s, 8)), ref)
        add(end, b, 'pad', 'fill' if not any(rom[end:b]) else 'fill (non-zero)',
            'tail of song%02d file after its last stream (%s)' % (i, 'zero' if not any(rom[end:b]) else
                                                                 'non-zero leftover bytes, never executed'), 'none')
    return L

def main():
    a = sys.argv[1:]
    if not a:
        sys.exit(__doc__)
    if a[0] == 'map':
        rom = open(a[1], 'rb').read()
        print('\n'.join(map_lines(rom))); return
    if a[0] == 'check':
        rom = open(a[1], 'rb').read()
        bad = extract(rom, a[2], check=True)
        print('check: %s' % ('OK' if bad == 0 else '%d mismatches' % bad))
        sys.exit(1 if bad else 0)
    rom = open(a[0], 'rb').read()
    extract(rom, a[1])
    print('done', game_of(rom)['name'])

if __name__ == '__main__':
    main()
