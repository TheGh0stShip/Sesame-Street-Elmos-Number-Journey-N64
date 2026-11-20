#!/usr/bin/env python3
"""libmus song / effect-bank extractor for Elmo's Letter Adventure and Elmo's Number Journey.

    extract_songs.py ROM GAME OUTDIR          GAME = letter | number
        writes OUTDIR/<name>.txt  lossless text dump (one event per line)
               OUTDIR/<name>.mid  standard MIDI file approximating playback (songs only)
               OUTDIR/fxbank.txt  the default sound-effect bank (same command language)
    extract_songs.py check ROM GAME OUTDIR    re-assemble every .txt in OUTDIR and compare with the ROM
    extract_songs.py map ROM GAME             print MAP.tsv lines

The tool holds no ROM content: only file bounds (ROM-address constants found in the game code) and the
command table of the libmus player (see ../FORMAT.md). Text syntax is documented in FORMAT.md.
"""
import os, struct, sys

# ------------------------------------------------------------------ file bounds (from game code)
# (start, end, name, how referenced)
LETTER = [
    (0x2F9B60, 0x2FBD50, 'song_2F9B60', 'none (unused: end of bonusabc.bif in func_8001D9D0, no start reference)'),
    (0x4D9D80, 0x4DB3E0, 'song_4D9D80', 'func_80018A60 state 0 (title, mode 21); func_8003AB7C level 19 all-flags branch'),
    (0x4DB3E0, 0x4DD320, 'song_4DB3E0', 'func_80018A60 state 19 (mode 22 screen)'),
    (0x4DD320, 0x4E32C0, 'song_4DD320', 'none (unused: only its end address 0x4E32C0 appears, as the next start)'),
    (0x4E32C0, 0x4EA530, 'song_4E32C0', 'func_80018A60 level start, level 19 bonusabc'),
    (0x4EA530, 0x4EADF0, 'song_4EA530', 'func_8003AB7C level 19 (bonus end jingle, timer 370)'),
    (0x4EADF0, 0x4F1850, 'song_4EADF0', 'func_80018A60 level start, level 18 orchard'),
    (0x4F1850, 0x4F2030, 'song_4F1850', 'func_8003A650 level 18 (item-collected jingle)'),
    (0x4F2030, 0x4FA460, 'song_4F2030', 'func_80018A60 level start, level 17 garden'),
    (0x4FA460, 0x4FAC20, 'song_4FA460', 'func_8003A650 level 17 (item-collected jingle)'),
    (0x4FAC20, 0x501130, 'song_4FAC20', 'func_80018A60 level start, level 15 manor'),
    (0x501130, 0x501820, 'song_501130', 'func_8003A650 level 15 (item-collected jingle)'),
    (0x501820, 0x507070, 'song_501820', 'func_80018A60 level start, level 16 chute'),
    (0x507070, 0x507890, 'song_507070', 'func_8003A650 level 16 (item-collected jingle)'),
    (0x507890, 0x50C970, 'song_507890', 'func_80018A60 level start, levels 13 planet and 14 space'),
    (0x50C970, 0x50D000, 'song_50C970', 'func_8003A650 levels 13/14 (item-collected jingle)'),
    (0x50D000, 0x50F9E0, 'song_50D000', 'func_8003AB7C timer 270, every level except 11/19 (level-complete fanfare)'),
    (0x50F9E0, 0x510A10, 'song_50F9E0', 'none (unused: 0x50F9E0 is only the end symbol of the previous song)'),
]
NUMBER = [
    (0x4B00C0, 0x4B8E70, 'song_4B00C0', 'func_80019470 level start, level 10 bump'),
    (0x4B8E70, 0x4BA590, 'song_4B8E70', 'func_80019470 state 0 (title, mode 21); func_8003C494 level 11 task-7 branch'),
    (0x4BA590, 0x4BC400, 'song_4BA590', 'func_80019470 state 19 (mode 22 screen)'),
    (0x4BC400, 0x4BCB50, 'song_4BC400', 'func_8003BF64 level 10 (item-collected jingle)'),
    (0x4BCB50, 0x4C12F0, 'song_4BCB50', 'func_80019470 level start, level 9 dunk'),
    (0x4C12F0, 0x4C1970, 'song_4C12F0', 'func_8003BF64 level 9 (item-collected jingle)'),
    (0x4C1970, 0x4C6350, 'song_4C1970', 'func_80019470 level start, level 7 maze'),
    (0x4C6350, 0x4C69A0, 'song_4C6350', 'func_8003BF64 level 7 (item-collected jingle)'),
    (0x4C69A0, 0x4CB8D0, 'song_4C69A0', 'func_80019470 level start, level 8 mansion'),
    (0x4CB8D0, 0x4CBCD0, 'song_4CB8D0', 'func_8003BF64 level 8 (item-collected jingle)'),
    (0x4CBCD0, 0x4CFEC0, 'song_4CBCD0', 'func_80019470 level start, level 6 ski'),
    (0x4CFEC0, 0x4D0690, 'song_4CFEC0', 'func_8003BF64 level 6 (item-collected jingle)'),
    (0x4D0690, 0x4D7C90, 'song_4D0690', 'func_80019470 level start, level 5 river'),
    (0x4D7C90, 0x4D83F0, 'song_4D7C90', 'func_8003BF64 level 5 (item-collected jingle)'),
    (0x4D83F0, 0x4DF7C0, 'song_4D83F0', 'func_80019470 level start, level 11 bonus123'),
    (0x4DF7C0, 0x4E0870, 'song_4DF7C0', 'func_800287AC guide actor state 0x48 when task 7 is reached (hub event)'),
    (0x4E0870, 0x4E1090, 'song_4E0870', 'func_8003C494 level 11 (bonus end jingle, timer 470)'),
    (0x4E1090, 0x4E7620, 'song_4E1090', 'func_80019470 level start, level 1 street (hub)'),
    (0x4E7620, 0x4E9810, 'song_4E7620', 'func_8003C494 timer 270, every level except 11/19 (level-complete fanfare)'),
]
GAMES = {
    'letter': dict(songs=LETTER, fx=(0x3E3FB0, 0x3E4830), init='func_800170B0', load='func_8001892C'),
    'number': dict(songs=NUMBER, fx=(0x3CB0C0, 0x3CB880), init='func_80017520', load='func_8001933C'),
}

# ------------------------------------------------------------------ command table (jump table D_80076E10 / D_80077620)
# arg kinds: b = u8, s = s8, v = 7/15-bit varint, w = u16 big-endian
CMDS = {
    0x80: ('stop', ''), 0x81: ('wave', 'v'), 0x82: ('port', 'b'), 0x83: ('portoff', ''),
    0x84: ('envdef', 'bbbbbbb'), 0x85: ('tempo', 'b'), 0x86: ('cutoff', 'w'), 0x87: ('endit', 'b'),
    0x88: ('vibup', 'bbb'), 0x89: ('vibdown', 'bbb'), 0x8A: ('viboff', ''), 0x8B: ('length', 'v'),
    0x8C: ('ignore', ''), 0x8D: ('trans', 's'), 0x8E: ('ignoretrans', ''), 0x8F: ('distort', 's'),
    0x90: ('env', 'v'), 0x91: ('envoff', ''), 0x92: ('envon', ''), 0x93: ('troff', ''),
    0x94: ('tron', ''), 0x95: ('for', 'b'), 0x96: ('next', ''), 0x97: ('wobble', 'bbb'),
    0x98: ('wobbleoff', ''), 0x99: ('velon', ''), 0x9A: ('veloff', ''), 0x9B: ('velocity', 'b'),
    0x9C: ('pan', 'b'), 0x9D: ('stereo', 'bb'), 0x9E: ('drums', 'v'), 0x9F: ('drumsoff', ''),
    0xA0: ('print', 'b'), 0xA1: ('goto', 'www'), 0xA2: ('reverb', 'b'), 0xA3: ('randnote', 'bb'),
    0xA4: ('randvolume', 'bb'), 0xA5: ('randpan', 'bb'), 0xA6: ('volume', 'b'), 0xA7: ('startfx', 'v'),
    0xA8: ('bendrange', 'b'), 0xA9: ('sweep', 'b'), 0xAA: ('changefx', 'b'), 0xAB: ('marker', 'm'),
    0xAC: ('length0', ''),
}
BYNAME = {v[0]: (k, v[1]) for k, v in CMDS.items()}
REST = 0x60


class Bad(Exception):
    pass


def u32(d, o): return struct.unpack_from('>I', d, o)[0]
def u16(d, o): return struct.unpack_from('>H', d, o)[0]


def rd_var(d, p, end):
    if p >= end: raise Bad()
    c = d[p]
    if c < 0x80: return str(c), p + 1, c
    if p + 1 >= end: raise Bad()
    v = ((c & 0x7F) << 8) | d[p + 1]
    return (str(v) + ('L' if v < 0x80 else '')), p + 2, v


def asm_var(s):
    if s.endswith('L'):
        v = int(s[:-1]); return bytes([0x80, v])
    v = int(s)
    if v < 0x80: return bytes([v])
    if v > 0x7FFF: raise ValueError(s)
    return bytes([0x80 | (v >> 8), v & 0xFF])


# ------------------------------------------------------------------ disassembly
def dis_chan(d, p, end, velmode):
    """channel/effect command stream -> (lines, end position reached)"""
    out = []
    fixlen = 0; ign = 0
    safe_n, safe_p = 0, p           # last point after stop/goto/next: fall back to hex from there
    try:
        while p < end:
            p, velmode, fixlen, ign = _dis_one(d, p, end, out, velmode, fixlen, ign)
            if out and out[-1].split()[0] in ('stop', 'goto', 'next', 'pad'):
                safe_n, safe_p = len(out), p
    except Bad:
        if safe_n == 0: raise
        del out[safe_n:]
        out += hexlines(d[safe_p:end]); p = end
    return out, p


def _dis_one(d, p, end, out, velmode, fixlen, ign):
    if True:
        c = d[p]
        if c >= 0x80:
            if c not in CMDS: raise Bad()
            name, kinds = CMDS[c]; p += 1; args = []
            for k in kinds:
                if k == 'v':
                    s, p, v = rd_var(d, p, end); args.append(s)
                elif k == 'w':
                    if p + 2 > end: raise Bad()
                    args.append('0x%X' % u16(d, p)); p += 2
                elif k == 'm':
                    if p + 2 > end: raise Bad()
                    args += [str(d[p]), str(d[p + 1])]; b = d[p + 1]; p += 2
                    if b & 0x80:
                        if p >= end: raise Bad()
                        args.append(str(d[p])); p += 1
                else:
                    if p >= end: raise Bad()
                    v = d[p]; p += 1
                    args.append(str(v - 256 if k == 's' and v >= 0x80 else v))
            if c == 0x8B: fixlen = 1
            elif c == 0xAC: fixlen = 0
            elif c == 0x8C: ign = 1
            elif c == 0x99: velmode = 1
            elif c in (0x9A, 0x9B): velmode = 0
            out.append(' '.join([name] + args))
            if c in (0x80, 0xA1):
                rest = d[p:end]
                if rest and not any(rest):
                    out.append('pad %d' % len(rest)); p = end
            return p, velmode, fixlen, ign
        else:
            p += 1
            parts = ['rest' if c == REST else 'note %d' % c]
            if velmode:
                if p >= end: raise Bad()
                v = d[p]; p += 1
                if v >= 0x80: velmode = 0; parts.append('vel %d!' % (v & 0x7F))
                else: parts.append('vel %d' % v)
            if not fixlen or ign:
                ign = 0
                s, p, _ = rd_var(d, p, end); parts.append('len ' + s)
            out.append(' '.join(parts))
            return p, velmode, fixlen, ign


def dis_ctl(d, p, end):
    out = []
    while p < end:
        c = d[p]
        if c < 0x80:
            out.append('set %d' % c); p += 1
        else:
            s, p2, _ = rd_var(d, p + 1, end)
            out.append('hold %d %s' % (c & 0x7F, s)); p = p2
    return out


def hexlines(b):
    return ['hex ' + b[i:i + 32].hex() for i in range(0, len(b), 32)]


def safe(fn, d, p, end, *a):
    try:
        r = fn(d, p, end, *a)
        if isinstance(r, tuple): r = r[0]
        return r
    except Bad:
        return hexlines(d[p:end])


def song_header(s):
    w = struct.unpack_from('>11I', s, 0)
    return dict(version=w[0], tracks=w[1], waves=w[2], chan=w[3], vol=w[4], bend=w[5], env=w[6],
                drum=w[7], wavemap=w[8], master=w[9], reloc=w[10])


def song_regions(s):
    h = song_header(s); n = h['tracks']
    R = {0: ('header', None)}
    R[h['chan']] = ('ptrs', 'chan'); R[h['vol']] = ('ptrs', 'vol'); R[h['bend']] = ('ptrs', 'bend')
    R[h['wavemap']] = ('wavemap', None); R[h['drum']] = ('drums', None); R[h['env']] = ('envs', None)
    if h['master']: R.setdefault(h['master'], ('chan', 'master'))
    for kind, tab in (('chan', 'chan'), ('vol', 'vol'), ('bend', 'bend')):
        for i in range(n):
            o = u32(s, h[tab] + 4 * i)
            if o: R.setdefault(o, (kind, str(i)))
    if 0x2C not in R: R[0x2C] = ('raw', None)
    return h, sorted(R.items())


def dump_song(s, title):
    h, regs = song_regions(s)
    L = ['# ' + title, 'size 0x%X' % len(s)]
    for idx, (o, (kind, tag)) in enumerate(regs):
        e = regs[idx + 1][0] if idx + 1 < len(regs) else len(s)
        L.append('@%04X %s%s' % (o, kind, (' ' + tag) if tag else ''))
        if kind == 'header':
            L.append('header version=0x%X tracks=%d waves=%d chan=0x%X vol=0x%X bend=0x%X env=0x%X '
                     'drum=0x%X wavemap=0x%X master=0x%X reloc=%d' % tuple(
                         h[k] for k in 'version tracks waves chan vol bend env drum wavemap master reloc'.split()))
            if e > 0x2C: L += hexlines(s[0x2C:e])
        elif kind == 'ptrs':
            k = (e - o) // 4
            L.append('ptr ' + ' '.join('0x%X' % u32(s, o + 4 * i) for i in range(k)))
            L += hexlines(s[o + 4 * k:e])
        elif kind == 'wavemap':
            k = (e - o) // 2
            L.append('u16 ' + ' '.join(str(u16(s, o + 2 * i)) for i in range(k)))
            L += hexlines(s[o + 2 * k:e])
        elif kind == 'drums':
            k = (e - o) // 6
            for i in range(k):
                q = o + 6 * i
                L.append('drum %d %d %d %d' % (u16(s, q), u16(s, q + 2), s[q + 4], s[q + 5]))
            L += hexlines(s[o + 6 * k:e])
        elif kind == 'envs':
            k = (e - o) // 7
            for i in range(k):
                L.append('envelope ' + ' '.join(str(x) for x in s[o + 7 * i:o + 7 * i + 7]))
            L += hexlines(s[o + 7 * k:e])
        elif kind == 'chan':
            L += ['  ' + x for x in safe(dis_chan, s, o, e, 1)]
        elif kind in ('vol', 'bend'):
            L += ['  ' + x for x in safe(dis_ctl, s, o, e)]
        else:
            L += hexlines(s[o:e])
    return '\n'.join(L) + '\n'


def dump_fx(f, title):
    n, x1, nw, flags, pb, wm = struct.unpack_from('>6I', f, 0)
    R = {0: 'fxheader', 0x18: 'fxlist', wm: 'wavemap'}
    for i in range(n):
        o = u32(f, 0x18 + 8 * i)
        if o: R.setdefault(o, 'fx %d' % i)
    regs = sorted(R.items())
    L = ['# ' + title, 'size 0x%X' % len(f)]
    for idx, (o, kind) in enumerate(regs):
        e = regs[idx + 1][0] if idx + 1 < len(regs) else len(f)
        L.append('@%04X %s' % (o, kind))
        if kind == 'fxheader':
            L.append('fxheader count=%d unk4=%d waves=%d flags=%d ptrbank=%d wavemap=0x%X' % (n, x1, nw, flags, pb, wm))
        elif kind == 'fxlist':
            for i in range((e - o) // 8):
                L.append('fxent 0x%X %d' % (u32(f, o + 8 * i), u32(f, o + 8 * i + 4)))
            L += hexlines(f[o + (e - o) // 8 * 8:e])
        elif kind == 'wavemap':
            k = min(nw, (e - o) // 2)
            L.append('u16 ' + ' '.join(str(u16(f, o + 2 * i)) for i in range(k)))
            L += hexlines(f[o + 2 * k:e])
        else:
            L += ['  ' + x for x in safe(dis_chan, f, o, e, 0)]
    return '\n'.join(L) + '\n'


# ------------------------------------------------------------------ assembler
def asm(text):
    out = bytearray()
    for ln in text.splitlines():
        t = ln.split()
        if not t or t[0].startswith('#') or t[0] == 'size': continue
        k = t[0]
        if k.startswith('@'):
            if int(k[1:], 16) != len(out): raise ValueError('region %s at 0x%X' % (k, len(out)))
            continue
        a = t[1:]
        if k == 'header':
            kv = dict(x.split('=') for x in a)
            out += struct.pack('>11I', *[int(kv[n], 0) for n in
                                         'version tracks waves chan vol bend env drum wavemap master reloc'.split()])
        elif k == 'fxheader':
            kv = dict(x.split('=') for x in a)
            out += struct.pack('>6I', *[int(kv[n], 0) for n in 'count unk4 waves flags ptrbank wavemap'.split()])
        elif k == 'fxent': out += struct.pack('>II', int(a[0], 0), int(a[1]))
        elif k == 'hex': out += bytes.fromhex(a[0])
        elif k == 'ptr': out += b''.join(struct.pack('>I', int(x, 0)) for x in a)
        elif k == 'u16': out += b''.join(struct.pack('>H', int(x)) for x in a)
        elif k == 'drum': out += struct.pack('>HHBB', *map(int, a))
        elif k == 'envelope': out += bytes(map(int, a))
        elif k == 'set': out.append(int(a[0]))
        elif k == 'hold': out += bytes([0x80 | int(a[0])]) + asm_var(a[1])
        elif k == 'pad': out += bytes(int(a[0]))
        elif k in ('note', 'rest'):
            if k == 'note': out.append(int(a[0])); a = a[1:]
            else: out.append(REST)
            while a:
                if a[0] == 'vel':
                    v = a[1]; out.append(int(v.rstrip('!')) | (0x80 if v.endswith('!') else 0))
                else:
                    out += asm_var(a[1])
                a = a[2:]
        elif k in BYNAME:
            c, kinds = BYNAME[k]; out.append(c)
            if kinds == 'm': out += bytes(int(x) for x in a); continue
            for kd, x in zip(kinds, a):
                if kd == 'v': out += asm_var(x)
                elif kd == 'w': out += struct.pack('>H', int(x, 0))
                else: out.append(int(x) & 0xFF)
        else:
            raise ValueError('bad line: ' + ln)
    return bytes(out)


# ------------------------------------------------------------------ MIDI approximation
PPQN = 48   # tempo byte = bpm; player advances bpm*3*8192/120/256 = 0.8*bpm ticks/s -> 48 ticks per beat


def vlq(n):
    b = [n & 0x7F]; n >>= 7
    while n: b.append(0x80 | (n & 0x7F)); n >>= 7
    return bytes(reversed(b))


def play_chan(s, start, velmode, mch, cap):
    """interpret a channel stream; returns (events, tempo events, end tick, wave changes)"""
    ev = []; tempos = []
    p = start; t = 0; vel = 100; fixed = 0; ign = 0; trans = 0; drums = None
    stack = []; steps = 0
    while p is not None and p < len(s) and t < cap and steps < 200000:
        steps += 1
        c = s[p]
        if c >= 0x80:
            name, kinds = CMDS.get(c, ('?', ''))
            p += 1; args = []
            for k in kinds:
                if k == 'v':
                    _, p, v = rd_var(s, p, len(s)); args.append(v)
                elif k == 'w': args.append(u16(s, p)); p += 2
                elif k == 'm':
                    b = s[p + 1]; p += 2 + (1 if b & 0x80 else 0)
                else:
                    v = s[p]; p += 1; args.append(v - 256 if k == 's' and v >= 0x80 else v)
            if c == 0x80: break
            if c == 0xA1:                         # goto: loops back in practice -> stop rendering
                ev.append((t, 1, b'\xff\x06' + vlq(4) + b'loop')); break
            if c == 0x81: ev.append((t, 1, bytes([0xC0 | mch, args[0] & 0x7F])))
            elif c == 0x85: tempos.append((t, max(1, args[0])))
            elif c == 0x8B: fixed = args[0]
            elif c == 0xAC: fixed = 0
            elif c == 0x8C: ign = 1
            elif c == 0x8D: trans = args[0]
            elif c == 0x99: velmode = 1
            elif c == 0x9A: velmode = 0
            elif c == 0x9B: velmode = 0; vel = args[0]
            elif c == 0x9C: ev.append((t, 1, bytes([0xB0 | mch, 10, args[0] >> 1])))
            elif c == 0xA6: ev.append((t, 1, bytes([0xB0 | mch, 11, args[0] & 0x7F])))
            elif c == 0xA2: ev.append((t, 1, bytes([0xB0 | mch, 91, args[0] & 0x7F])))
            elif c == 0x9E: drums = args[0]
            elif c == 0x9F: drums = None
            elif c == 0x95: stack.append([p, args[0]])
            elif c == 0x96 and stack:
                top = stack[-1]
                if top[1] != 0xFF:
                    top[1] -= 1
                    if top[1] == 0: stack.pop(); continue
                    p = top[0]
                else:
                    stack.pop()                 # infinite loop: render one pass
            continue
        key = c; p += 1
        if velmode:
            v = s[p]; p += 1
            if v >= 0x80: velmode = 0; v &= 0x7F
            vel = v
        if fixed and not ign: ln = fixed
        else:
            ign = 0; _, p, ln = rd_var(s, p, len(s))
        if key != REST and ln > 0:
            k = max(0, min(127, key + trans + 12))
            ch = 9 if drums is not None else mch
            ev.append((t, 2, bytes([0x90 | ch, k, max(1, vel)])))
            ev.append((t + ln, 0, bytes([0x80 | ch, k, 0])))
        t += ln
    return ev, tempos, t


def play_ctl(s, start, mch, kind, cap):
    ev = []; p = start; t = 0; last = None
    while p < len(s) and t < cap:
        c = s[p]
        if c < 0x80: v, n = c, 1; p += 1
        else:
            v = c & 0x7F; _, p, n = rd_var(s, p + 1, len(s)); n += 2
        if v != last:
            if kind == 'vol': ev.append((t, 1, bytes([0xB0 | mch, 7, v])))
            else:
                pw = max(0, min(16383, 8192 + (v - 64) * 128))
                ev.append((t, 1, bytes([0xE0 | mch, pw & 0x7F, pw >> 7])))
            last = v
        t += n
    return ev


def song_midi(s, path):
    h = song_header(s)
    tracks = []; tempos = []; end = 0
    chans = []
    for i in range(h['tracks']):
        o = u32(s, h['chan'] + 4 * i)
        if o: chans.append((i, o))
    cap = 48 * 4 * 400
    res = []
    for n, (i, o) in enumerate(chans):
        mch = n if n < 9 else n + 1
        mch = min(mch, 15)
        try: ev, te, t = play_chan(s, o, 1, mch, cap)
        except (IndexError, Bad): ev, te, t = [], [], 0
        tempos += te; end = max(end, t); res.append((i, mch, ev))
    if h['master']:
        try: _, te, _ = play_chan(s, h['master'], 1, 15, cap)
        except (IndexError, Bad): te = []
        tempos += te
    for i, mch, ev in res:
        for kind in ('vol', 'bend'):
            o = u32(s, h[kind] + 4 * i)
            if o:
                try: ev += play_ctl(s, o, mch, kind, end)
                except (IndexError, Bad): pass
        tracks.append((i, ev))
    tempo_tr = [(0, 1, b'\xff\x51\x03' + (500000).to_bytes(3, 'big'))]
    for t, bpm in sorted(set(tempos)):
        tempo_tr.append((t, 1, b'\xff\x51\x03' + min(0xFFFFFF, 60000000 // bpm).to_bytes(3, 'big')))

    def chunk(evs, name=None):
        b = bytearray()
        if name: b += b'\x00\xff\x03' + vlq(len(name)) + name.encode()
        last = 0
        for t, _, m in sorted(evs, key=lambda e: (e[0], e[1])):
            b += vlq(t - last) + m; last = t
        b += b'\x00\xff\x2f\x00'
        return b'MTrk' + struct.pack('>I', len(b)) + bytes(b)
    data = b'MThd' + struct.pack('>IHHH', 6, 1, len(tracks) + 1, PPQN) + chunk(tempo_tr, 'tempo')
    for i, ev in tracks: data += chunk(ev, 'track %d' % i)
    open(path, 'wb').write(data)


# ------------------------------------------------------------------ driver
def items(rom, game):
    g = GAMES[game]
    for a, b, name, ref in g['songs']:
        yield name, a, b, 'song', ref
    fa, fb = g['fx']
    yield 'fxbank', fa, fb, 'fx', '%s copies it to RAM, musConfig default fx bank (relocated by func_8004D43C)' % g['init']


def extract(rom, game, outdir):
    os.makedirs(outdir, exist_ok=True)
    for name, a, b, kind, ref in items(rom, game):
        d = rom[a:b]
        title = '%s %s rom 0x%X-0x%X: %s' % (game, name, a, b, ref)
        txt = dump_song(d, title) if kind == 'song' else dump_fx(d, title)
        open(os.path.join(outdir, name + '.txt'), 'w').write(txt)
        if kind == 'song': song_midi(d, os.path.join(outdir, name + '.mid'))
        print(name, 'ok' if asm(txt) == d else 'MISMATCH')


def check(rom, game, outdir):
    bad = 0; n = 0; lines = 0; hexb = 0
    for name, a, b, kind, ref in items(rom, game):
        txt = open(os.path.join(outdir, name + '.txt')).read()
        lines += txt.count('\n')
        hexb += sum(len(l.split()[1]) // 2 for l in txt.splitlines() if l.strip().startswith('hex '))
        n += 1
        if asm(txt) != rom[a:b]:
            bad += 1; print('MISMATCH', name)
    print('%s: %d files, %d lines, %d bytes left as hex, %s' % (game, n, lines, hexb, 'OK' if not bad else '%d BAD' % bad))
    return bad


def map_lines(rom, game):
    L = []
    for name, a, b, kind, ref in items(rom, game):
        d = rom[a:b]
        if kind == 'song':
            h = song_header(d)
            desc = '%s: libmus song v0x%X, %d tracks, %d waves' % (name, h['version'], h['tracks'], h['waves'])
            fmt = 'libmus song (FORMAT.md)'
        else:
            desc = 'default sound-effect bank: %d effect streams, %d-wave map' % (u32(d, 0), u32(d, 8))
            fmt = 'libmus fx bank (FORMAT.md)'
        L.append('\t'.join([game, '0x%06X' % a, '0x%06X' % b, 'libmus_song' if kind == 'song' else 'libmus_fx_bank',
                            fmt, desc, ref, 'tools/extract_songs.py']))
    return L


def main():
    a = sys.argv[1:]
    if len(a) >= 3 and a[0] == 'map':
        print('\n'.join(map_lines(open(a[1], 'rb').read(), a[2]))); return
    if len(a) == 4 and a[0] == 'check':
        sys.exit(1 if check(open(a[1], 'rb').read(), a[2], a[3]) else 0)
    if len(a) == 3:
        extract(open(a[0], 'rb').read(), a[1], a[2]); return
    sys.exit(__doc__)


if __name__ == '__main__':
    main()
