#!/usr/bin/env python3
"""gen_data.py: emit typed C definitions for data2's territory, driven by types_<game>.tsv.

  gen_data.py ROM GAME OUTDIR            write OUTDIR/data2_<game>.c (values read from the ROM)
  gen_data.py check ROM GAME OUTDIR      parse OUTDIR/data2_<game>.c back, re-serialise every
                                         initialiser to big-endian bytes, compare with the ROM, and
                                         verify the type table tiles the territory with no gap
  gen_data.py strings ROM GAME OUTDIR    write OUTDIR/strings_<game>.txt (address, users, text)

The type table (next to this script) holds addresses, sizes, C types and evidence only.
Function bounds for jump-table labels come from the matched-code VERIFY.tsv (path below or $VERIFY).
"""
import sys, os, re, struct, math, bisect

HERE = os.path.dirname(os.path.abspath(__file__))
ROM2RAM = 0x7FFFF400
VERIFY = '${N64DK_LAB}/sesverify/v8L/%s/VERIFY.tsv'
INT = {'u8': ('B', 1), 'u16': ('H', 2), 'u32': ('I', 4), 's16': ('h', 2), 's32': ('i', 4),
       's64': ('q', 8), 'pad': ('B', 1), 'rsp_data': ('I', 4)}
CDECL = {'u8': 'u8', 'u16': 'u16', 'u32': 'u32', 's16': 's16', 's32': 's32', 's64': 's64',
         'pad': 'u8', 'rsp_data': 'u32', 'f32': 'f32', 'f64': 'f64', 'jtbl': 'void *const'}

def load_types(game):
    rows, hdr = [], []
    for ln in open(os.path.join(HERE, 'types_%s.tsv' % game)):
        if ln.startswith('#'):
            hdr.append(ln)
            continue
        a, sz, ct, nm, ev = ln.rstrip('\n').split('\t')
        rows.append((int(a, 16), int(sz, 16), ct, nm, ev))
    m = re.search(r'ROM ([0-9A-F]+)-([0-9A-F]+)', ''.join(hdr))
    return rows, int(m.group(1), 16), int(m.group(2), 16)

def load_funcs(game):
    fs = []
    for ln in open(os.environ.get('VERIFY', VERIFY % game)):
        f = ln.split('\t')
        fs.append((int(f[0], 16), int(f[1])))
    fs.sort()
    return fs

def cstr(b):
    out = []
    for c in b:
        if c == 0x22: out.append('\\"')
        elif c == 0x5C: out.append('\\\\')
        elif c == 10: out.append('\\n')
        elif c == 9: out.append('\\t')
        elif 0x20 <= c < 0x7F: out.append(chr(c))
        else: out.append('\\%03o' % c)
    s = ''.join(out)
    # an octal escape followed by a digit would merge: split the literal
    return '"' + re.sub(r'(\\[0-7]{3})(?=[0-9])', r'\1" "', s) + '"'

def split_strings(blob):
    parts, p = [], 0
    while p < len(blob):
        e = blob.index(b'\0', p)
        parts.append((p, blob[p:e]))
        p = e + 1
    return parts

def fmt_float(v, bits):
    if not math.isfinite(v):
        return None
    t = repr(v)
    if 'e' not in t and '.' not in t:
        t += '.0'
    return t + ('f' if bits == 32 else '')

def generate(rom, game, outdir):
    rows, _, _ = load_types(game)
    fs = load_funcs(game)
    starts = [f[0] for f in fs]
    L = ['/* data2: typed C definitions for %s, generated from the ROM by gen_data.py. */' % game,
         '/* Every object is preceded by a marker line @ROM <start> <size> <ctype> used by check mode. */',
         'typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;',
         'typedef short s16; typedef int s32; typedef long long s64; typedef float f32; typedef double f64;',
         '#define F32BITS(x) /* non-finite float, raw bits */ x', '']
    for a, sz, ct, nm, ev in rows:
        b = rom[a:a + sz]
        ram = a + ROM2RAM
        L.append('/* @ROM %X %X %s  RAM %08X  %s */' % (a, sz, ct, ram, ev))
        if ct == 'char[]':
            parts = split_strings(b)
            for i, (off, s) in enumerate(parts):
                n = nm if len(parts) == 1 else '%s_%d' % (nm, i)
                L.append('const char %s[] = %s; /* %08X */' % (n, cstr(s), ram + off))
            continue
        if ct in ('f32', 'f64'):
            bits, w = (32, 4) if ct == 'f32' else (64, 8)
            vals = struct.unpack('>%d%s' % (sz // w, 'f' if w == 4 else 'd'), b)
            raws = struct.unpack('>%d%s' % (sz // w, 'I' if w == 4 else 'Q'), b)
            items = []
            for v, r in zip(vals, raws):
                t = fmt_float(v, bits)
                items.append(t if t is not None else 'F32BITS(0x%X)' % r)
            body = ', '.join(items)
        elif ct == 'jtbl':
            items = []
            for (w,) in struct.iter_unpack('>I', b):
                i = bisect.bisect_right(starts, w) - 1
                items.append('func_%08X + 0x%X' % (starts[i], w - starts[i]))
            body = ',\n    '.join(items)
        else:
            f, w = INT[ct]
            vals = struct.unpack('>%d%s' % (sz // w, f), b)
            if ct == 'pad' and not any(vals):
                body = '0'
            elif ct in ('u32', 'rsp_data'):
                body = ', '.join('0x%08X' % v for v in vals)
            elif ct == 's64':
                body = ', '.join('%dLL' % v for v in vals)
            else:
                body = ', '.join(str(v) for v in vals)
        n = sz // (INT.get(ct, ('', 4 if ct in ('f32', 'jtbl') else 8))[1])
        L.append('%s %s[%d] = {%s};' % (CDECL[ct], nm, n, body))
    os.makedirs(outdir, exist_ok=True)
    p = os.path.join(outdir, 'data2_%s.c' % game)
    open(p, 'w').write('\n'.join(L) + '\n')
    print(game, 'wrote', p, len(rows), 'objects')

def unescape(lit):
    out = bytearray()
    for piece in re.findall(r'"((?:[^"\\]|\\.)*)"', lit):
        i = 0
        while i < len(piece):
            c = piece[i]
            if c == '\\':
                d = piece[i + 1]
                if d in '01234567':
                    out.append(int(piece[i + 1:i + 4], 8)); i += 4; continue
                out.append({'n': 10, 't': 9, '"': 0x22, '\\': 0x5C}[d]); i += 2; continue
            out.append(ord(c)); i += 1
    return bytes(out)

def check(rom, game, outdir):
    rows, t0, t1 = load_types(game)
    fs = dict((f'func_{a:08X}', a) for a, _ in load_funcs(game))
    text = open(os.path.join(outdir, 'data2_%s.c' % game)).read()
    blocks = re.split(r'^/\* @ROM ', text, flags=re.M)[1:]
    bad, pos, total = 0, t0, 0
    if len(blocks) != len(rows):
        print('object count mismatch', len(blocks), len(rows)); bad += 1
    for blk in blocks:
        a, sz, ct = blk.split()[:3]
        a, sz = int(a, 16), int(sz, 16)
        if a != pos:
            print('gap/overlap at %X (expected %X)' % (a, pos)); bad += 1
        pos = a + sz
        body = blk.split('*/', 1)[1]
        if ct == 'char[]':
            out = b''.join(unescape(m) + b'\0' for m in re.findall(r'= ((?:"(?:[^"\\]|\\.)*"\s*)+);', body))
        else:
            init = body[body.index('{') + 1:body.rindex('}')]
            items = [x.strip() for x in init.split(',') if x.strip()]
            if ct == 'pad':
                out = bytes(sz) if items == ['0'] else bytes(int(x) for x in items)
            elif ct == 'f32':
                out = b''.join(struct.pack('>I', int(x[8:-1], 16)) if x.startswith('F32BITS')
                               else struct.pack('>f', float(x.rstrip('f'))) for x in items)
            elif ct == 'f64':
                out = b''.join(struct.pack('>d', float(x)) for x in items)
            elif ct == 'jtbl':
                out = b''.join(struct.pack('>I', fs[x.split('+')[0].strip()] + int(x.split('+')[1], 16))
                               for x in items)
            else:
                f, w = INT[ct]
                out = b''.join(struct.pack('>' + f, int(x.rstrip('LL'), 0)) for x in items)
        total += len(out)
        if out != rom[a:a + sz]:
            print('MISMATCH at ROM %X (%s, %d bytes, got %d)' % (a, ct, sz, len(out))); bad += 1
    if pos != t1:
        print('territory not covered: ends at %X, expected %X' % (pos, t1)); bad += 1
    print('%s check: %d objects, %d bytes re-serialised, ROM %X-%X: %s'
          % (game, len(blocks), total, t0, t1, 'OK' if not bad else '%d FAILURES' % bad))
    return bad

def strings(rom, game, outdir):
    rows, _, _ = load_types(game)
    users = {}
    rp = os.path.join(outdir, 'refs_%s.tsv' % game)
    for ln in open(rp):
        x = ln.split('\t')
        users.setdefault(int(x[0], 16), set()).add(x[2])
    L = ['# strings in data2 territory (%s): ROM\tRAM\tused_by\ttext (C escaped)' % game]
    for a, sz, ct, nm, ev in rows:
        if ct != 'char[]':
            continue
        for off, s in split_strings(rom[a:a + sz]):
            ram = a + off + ROM2RAM
            u = ','.join(sorted(users.get(ram, []))) or '-'
            L.append('%X\t%08X\t%s\t%s' % (a + off, ram, u, cstr(s)))
    p = os.path.join(outdir, 'strings_%s.txt' % game)
    open(p, 'w').write('\n'.join(L) + '\n')
    print(game, 'wrote', p, len(L) - 1, 'strings')

def main():
    av = sys.argv[1:]
    mode = 'gen'
    if av[0] in ('check', 'strings'):
        mode = av.pop(0)
    rom = open(av[0], 'rb').read()
    game, outdir = av[1], av[2]
    if mode == 'gen':
        generate(rom, game, outdir)
    elif mode == 'check':
        sys.exit(1 if check(rom, game, outdir) else 0)
    else:
        strings(rom, game, outdir)

main()
