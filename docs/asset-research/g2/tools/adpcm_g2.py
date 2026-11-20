#!/usr/bin/env python3
"""VADPCM frame headers: a faithful port of the N64 SDK `vadpcm_enc` frame encoder (vencodeframe,
inner_product, qsample, clamp, clip; public reimplementation n64decomp/sdk-tools adpcm/), and a
generator that rebuilds every sample's frames from the extracted WAV (audio agent) plus, where the
SDK algorithm run on the decoded PCM picks another header, an explicit per-frame header field.

usage: adpcm_g2.py extract ROM AUDIO_OUT OUTDIR   -> OUTDIR/<game>/headers.json (+ stats)
       adpcm_g2.py check   ROM AUDIO_OUT OUTDIR   -> regenerate all frames from WAV + JSON, compare with ROM
AUDIO_OUT = sesassets/audio/out (wNNN.wav + wNNN.json per bank).
"""
import sys, os, json, struct, glob, wave

def f32(x): return struct.unpack('<f', struct.pack('<f', x))[0]

def expand_book(order, npred, coefs):
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

def inner_product(n, v1, v2):
    out = 0
    for j in range(n):
        out += v1[j] * v2[j]
    return out >> 11                      # C: trunc then -1 if negative remainder == floor

def qsample(x, scale):                    # (s16)((x / scale) +/- 0.4999999), x/scale in f32 then double
    q = f32(x / scale)
    v = q + 0.4999999 if x > 0.0 else q - 0.4999999
    return int(v)                         # C truncation toward zero

def clip(ix, lo, hi): return lo if ix < lo else hi if ix > hi else ix

def choose(inbuf, state, tabs, order):
    """vencodeframe's predictor search and scale derivation. Returns (scale, predictor, ix[16], newstate)."""
    npred = len(tabs)
    mn = f32(1e30); optp = 0
    def errs(k):
        iv = [0] * 16; pred = [0] * 16; e = [0.0] * 16
        for i in range(order): iv[i] = state[16 - order + i]
        for i in range(8):
            pred[i] = inner_product(order + i, tabs[k][i], iv)
            iv[i + order] = inbuf[i] - pred[i]; e[i] = float(iv[i + order])
        for i in range(order): iv[i] = pred[8 - order + i] + iv[8 + i]
        for i in range(8):
            pred[8 + i] = inner_product(order + i, tabs[k][i], iv)
            iv[i + order] = inbuf[8 + i] - pred[8 + i]; e[8 + i] = float(iv[i + order])
        return e
    for k in range(npred):
        e = errs(k)
        se = 0.0
        for j in range(16): se = f32(se + f32(e[j] * e[j]))
        if se < mn: mn = se; optp = k
    e = errs(optp)
    ie = []
    for v in e:                            # clamp(16, e, ie, 16)
        v = min(max(v, -32768.0), 32767.0)
        ie.append(int(v + 0.5) if v > 0.0 else int(v - 0.5))
    mx = 0
    for v in ie:
        if abs(v) > abs(mx): mx = v
    scale = 0
    while scale <= 12:
        if -8 <= mx <= 7: break
        mx = int(mx / 2); scale += 1       # C integer division truncates
    return quantise(inbuf, state, tabs[optp], order, scale - 1, True) + (optp,)

def quantise(inbuf, save, tab, order, scale, retry):
    """The do/while quantisation loop of vencodeframe (scale is pre-decremented as in the C)."""
    niter = 0
    while True:
        niter += 1; maxclip = 0; scale += 1
        if scale > 12: scale = 12
        iv = [0] * 16; pred = [0] * 16; st = [0] * 16; ix = [0] * 16
        for i in range(order): iv[i] = save[16 - order + i]
        for h in range(2):
            if h:
                for i in range(order): iv[i] = st[8 - order + i]
            for i in range(8):
                n = 8 * h + i
                pred[n] = inner_product(order + i, tab[i], iv)
                se = f32(float(inbuf[n]) - float(pred[n]))
                q = qsample(se, 1 << scale)
                q = ((q + 0x8000) & 0xFFFF) - 0x8000          # s16
                cv = clip(q, -8, 7) - q
                maxclip = max(maxclip, abs(cv))
                q += cv
                ix[n] = q
                iv[i + order] = q * (1 << scale)
                st[n] = pred[n] + iv[i + order]
        if not (retry and maxclip >= 2 and niter < 2):
            return scale, ix, st

def frame_bytes(hdr, ix):
    return bytes([hdr]) + bytes(((ix[2 * k] & 15) << 4) | (ix[2 * k + 1] & 15) for k in range(8))

def decode_frame(fr, tab, order, state):
    """Decoder (n_audio): returns (clamped pcm16, unclamped history) for one frame."""
    sc = 1 << (fr[0] >> 4)
    ins = []
    for b in fr[1:]:
        for nb in (b >> 4, b & 15):
            ins.append((nb - 16 if nb & 8 else nb) * sc)
    out, raw = [], []
    last = state[16 - order:]
    for h in range(2):
        vec = last + ins[8 * h:8 * h + 8]
        cur = []
        for i in range(8):
            acc = sum(a * b for a, b in zip(tab[i], vec)) >> 11
            cur.append(acc)
        raw += cur
        cur_c = [min(max(v, -32768), 32767) for v in cur]
        out += cur_c
        last = cur_c[8 - order:]
    return out, raw

def exact_residuals(pcm16, state, tab, order, scale):
    """Inverse of the decoder for a given header (as audio's adpcm_encode_check); None if not invertible."""
    sc = 1 << scale; ix = []; prev = state[16 - order:]
    for h in range(2):
        vec = prev + [0] * 8
        for i in range(8):
            partial = sum(a * b for a, b in zip(tab[i], vec))
            want = pcm16[8 * h + i]
            r = want - (partial >> 11)
            if r % sc: return None
            n = r // sc
            if n < -8 or n > 7: return None
            vec[order + i] = n * sc; ix.append(n)
        prev = pcm16[8 * h + 8 - order:8 * h + 8]
    return ix

def read_wav(path):
    with wave.open(path, 'rb') as w:
        n = w.getnframes()
        return list(struct.unpack('<%dh' % n, w.readframes(n)))

def samples(audio_out, game):
    for js in sorted(glob.glob(os.path.join(audio_out, game, 'bank*', 'w*.json'))):
        side = json.load(open(js))
        if side.get('book') is None: continue
        yield js, side

def game_of(rom):
    t = rom[0x20:0x34]
    return 'letter' if b'LETTER' in t.upper() else 'number'

def rule_frame(inb, pcm, f, state, tabs, order):
    """The header rule: the SDK vadpcm_enc choice if it reproduces the PCM exactly, else the first header in
    order (scale 0..12, predictor 0..n-1) that does. Residuals follow from the header."""
    s, ix, _, p = choose(inb, state, tabs, order)
    prev = [0] * 8 + (pcm[16 * f - 8:16 * f] if f else [0] * 8)
    sdk = s << 4 | p
    for h in [sdk] + [a << 4 | b for a in range(13) for b in range(len(tabs))]:
        r = exact_residuals(inb, prev, tabs[h & 15], order, h >> 4)
        if r is not None:
            return frame_bytes(h, r)
    return frame_bytes(sdk, ix)

def generate(pcm, side, hdrs):
    """Rebuild a sample's frames: header = SDK choice on the PCM unless hdrs gives it; residuals = the SDK
    quantiser with that header; frames the PCM cannot determine come from side['exceptions'] (audio's)."""
    bk = side['book']; order = bk['order']; tabs = expand_book(order, bk['npred'], bk['coefs'])
    state = [0] * 16; out = bytearray()
    exc = side.get('exceptions', {})
    for f in range(side['frames']):
        inb = pcm[16 * f:16 * f + 16]
        if str(f) in exc:
            fr = bytes.fromhex(exc[str(f)])
        elif str(f) in hdrs:
            h = int(hdrs[str(f)], 16); p = h & 15; s = h >> 4
            prev = pcm[16 * f - 8:16 * f] if f else [0] * 8
            ix = exact_residuals(inb, [0] * 8 + prev, tabs[p], order, s)
            fr = frame_bytes(h, ix)
        else:
            fr = rule_frame(inb, pcm, f, state, tabs, order)
        _, raw = decode_frame(fr, tabs[fr[0] & 15], order, state)
        state = [0] * 8 + raw[8:16]
        out += fr
    return bytes(out)

def run(rom, audio_out, outdir, check):
    game = game_of(rom)
    hp = os.path.join(outdir, game, 'headers.json')
    if check: allh = json.load(open(hp))['samples']
    else: allh = {}
    st = dict(frames=0, sdk_header_equal=0, sdk_frame_equal=0, header_field=0, audio_exceptions=0,
              sdk_scale_only_diff=0, sdk_pred_diff=0, mismatched_samples=0)
    for js, side in samples(audio_out, game):
        key = os.path.relpath(js, os.path.join(audio_out, game))[:-5]
        rawrom = rom[side['rom']:side['rom'] + 9 * side['frames']]
        pcm = read_wav(js[:-5] + '.wav')
        bk = side['book']; order = bk['order']; tabs = expand_book(order, bk['npred'], bk['coefs'])
        if check:
            g = generate(pcm, side, allh.get(key, {}))
            if g != rawrom:
                st['mismatched_samples'] += 1; print('MISMATCH', key)
            st['frames'] += side['frames']; st['header_field'] += len(allh.get(key, {}))
            continue
        hd = {}; state = [0] * 16; exc = side.get('exceptions', {})
        for f in range(side['frames']):
            fr = rawrom[9 * f:9 * f + 9]; st['frames'] += 1
            inb = pcm[16 * f:16 * f + 16]
            if str(f) in exc:
                st['audio_exceptions'] += 1
            else:
                s, ix, _, p = choose(inb, state, tabs, order)
                if (s << 4 | p) == fr[0]:
                    st['sdk_header_equal'] += 1
                    if frame_bytes(fr[0], ix) == fr: st['sdk_frame_equal'] += 1
                else:
                    if p == fr[0] & 15: st['sdk_scale_only_diff'] += 1
                    else: st['sdk_pred_diff'] += 1
                if rule_frame(inb, pcm, f, state, tabs, order) != fr:
                    hd[str(f)] = '%02x' % fr[0]
            _, raw = decode_frame(fr, tabs[fr[0] & 15], order, state)
            state = [0] * 8 + raw[8:16]
        if hd: allh[key] = hd
        st['header_field'] += len(hd)
    if not check:
        os.makedirs(os.path.join(outdir, game), exist_ok=True)
        json.dump(dict(game=game, note='per-frame VADPCM header (scale<<4|predictor) where the SDK vadpcm_enc '
                       'choice on the decoded PCM differs; key = bank/wave, frame index -> header hex',
                       stats=st, samples=allh), open(hp, 'w'), indent=0)
    print(game, json.dumps(st))
    return st['mismatched_samples']

if __name__ == '__main__':
    mode, rom, ao, od = sys.argv[1:5]
    sys.exit(1 if run(open(rom, 'rb').read(), ao, od, mode == 'check') else 0)
