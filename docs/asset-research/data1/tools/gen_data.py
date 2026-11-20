#!/usr/bin/env python3
"""gen_data.py: emit the program's initialised .data (data1 territory) as typed C definitions.

  gen_data.py ROM GAME OUTDIR          write OUTDIR/data1_<game>.c (and OUTDIR/MAP_<game>.tsv)
  gen_data.py check ROM GAME OUTDIR    parse OUTDIR/data1_<game>.c back (no compiler), re-serialise every
                                       object from its initialiser, compare with the ROM bytes and check
                                       that the objects tile the territory exactly.

The object list and C types come from the hand-written type table tools/types_<game>.tsv
(columns: ram, size, ctype, name, class, comment, evidence). The table holds NO data values; every
value is read from the ROM given on the command line. Byte ranges between table rows are emitted
automatically as `pad_` (all zero) or `unk_` (u8 array) objects so that the whole territory is covered.

ctype grammar:  BASE | BASE[] | struct NAME{TYPE field;TYPE field[N];...}
  BASE = s8 u8 s16 u16 s32 u32 f32 f64 word ptr char Gfx Vtx Vp Mtx Bitmap Sprite | a struct NAME defined
  earlier in the table.  The element count is size / sizeof(BASE); a single element without [] is a scalar.
  `word` is a u32 that is written as a symbol when its value points into the program image; `ptr` is a
  void * (symbol or NULL or raw RAM address outside the image); `char` arrays are written as string literals
  when that is lossless.
"""
import sys, os, re, struct, math

OFF = 0x7FFFF400  # RAM = ROM + OFF inside the main segment
GAMES = {
    # territory pieces (RAM, half-open), .text range, end of program image (bss end)
    'letter': dict(pieces=[(0x8000043C, 0x80000450), (0x8006C9AC, 0x80073160)],
                   text=(0x80000400, 0x8006C9AC), image_end=0x80140FA0),
    'number': dict(pieces=[(0x8000043C, 0x80000450), (0x8006E31C, 0x800741E0)],
                   text=(0x80000400, 0x8006E31C), image_end=0x80140F60),
}
HERE = os.path.dirname(os.path.abspath(__file__))

PRIM = {  # name: (size, struct fmt, C type)
    's8': (1, 'b', 's8'), 'u8': (1, 'B', 'u8'), 's16': (2, 'h', 's16'), 'u16': (2, 'H', 'u16'),
    's32': (4, 'i', 's32'), 'u32': (4, 'I', 'u32'), 'f32': (4, 'f', 'f32'), 'f64': (8, 'd', 'f64'),
    'word': (4, 'I', 'u32'), 'ptr': (4, 'I', 'void *'), 'char': (1, 'B', 'char'),
}
BUILTIN_STRUCTS = {
    # Gfx as its two words; w1 is symbolised when it points into the program image
    'Gfx': 'u32 w0;word w1',
    'Vtx': 's16 ob[3];u16 flag;s16 tc[2];u8 cn[4]',
    'Vp': 's16 vscale[4];s16 vtrans[4]',
    'Mtx': 's32 m[16]',
    # libultra sp.h
    'Bitmap': 's16 width;s16 width_img;s16 s;s16 t;ptr buf;s16 actualHeight;s16 LUToffset',
    'Sprite': ('s16 x;s16 y;s16 width;s16 height;f32 scalex;f32 scaley;s16 expx;s16 expy;u16 attr;'
               's16 zdepth;u8 red;u8 green;u8 blue;u8 alpha;s16 startTLUT;s16 nTLUTs;ptr LUT;s16 istart;'
               's16 istep;s16 nbitmaps;s16 ndisplist;s16 bmheight;s16 bmHreal;u8 bmfmt;u8 bmsiz;ptr bitmap;'
               'ptr rsp_dl;ptr rsp_dl_next;s16 frac_s;s16 frac_t'),
}


class Types:
    def __init__(self):
        self.structs = {}   # name -> [(fname, base, count, offset)], size, align
        self.order = []
        for n, body in BUILTIN_STRUCTS.items():
            self.define(n, body)

    def info(self, base):
        if base in PRIM:
            return PRIM[base][0], PRIM[base][0]
        s = self.structs[base]
        return s['size'], s['align']

    def define(self, name, body):
        fields, off, al = [], 0, 1
        for f in [x.strip() for x in body.split(';') if x.strip()]:
            m = re.fullmatch(r'(\w+)\s+(\w+)(?:\[(\d+)\])?', f)
            if not m:
                raise SystemExit('bad field %r in struct %s' % (f, name))
            base, fn, cnt = m.group(1), m.group(2), int(m.group(3) or 0)
            sz, a = self.info(base)
            off = (off + a - 1) // a * a
            fields.append((fn, base, cnt, off))
            off += sz * max(cnt, 1)
            al = max(al, a)
        size = (off + al - 1) // al * al
        self.structs[name] = dict(fields=fields, size=size, align=al)
        self.order.append(name)

    def ctype(self, base):
        return PRIM[base][2] if base in PRIM else base


def load_table(game, T):
    rows = []
    for ln in open(os.path.join(HERE, 'types_%s.tsv' % game)):
        if not ln.strip() or ln.startswith('#'):
            continue
        f = ln.rstrip('\n').split('\t')
        f += [''] * (7 - len(f))
        ram, size, ctype, name, cls, comment, ev = f[:7]
        ram, size = int(ram, 16), int(size, 16)
        m = re.fullmatch(r'struct (\w+)\{(.*)\}(\[\])?', ctype)
        if m:
            if m.group(1) not in T.structs:
                T.define(m.group(1), m.group(2))
            base, arr = m.group(1), bool(m.group(3))
        else:
            m = re.fullmatch(r'(\w+)(\[\])?', ctype)
            base, arr = m.group(1), bool(m.group(2))
        esz = T.info(base)[0]
        if size % esz:
            raise SystemExit('%08X: size 0x%X not a multiple of sizeof(%s)=0x%X' % (ram, size, base, esz))
        rows.append(dict(ram=ram, size=size, base=base, arr=arr or size != esz, count=size // esz,
                         name=name or 'D_%08X' % ram, cls=cls or 'data', comment=comment, ev=ev))
    rows.sort(key=lambda r: r['ram'])
    return rows


def tile(game, rows, rom):
    """Add pad_/unk_ objects for bytes no table row covers; reject overlaps and rows outside the territory."""
    out = []
    for lo, hi in GAMES[game]['pieces']:
        cur = lo
        mine = [r for r in rows if lo <= r['ram'] < hi]
        for r in mine + [None]:
            nxt = r['ram'] if r else hi
            if nxt < cur:
                raise SystemExit('overlap at %08X' % nxt)
            while cur < nxt:
                # split a gap into all-zero runs and other bytes (4-byte granularity where possible)
                b = rom[cur - OFF:nxt - OFF]
                if b[0] == 0:
                    n = len(b) - len(b.lstrip(b'\0'))
                    cls, nm, base = 'pad', 'pad_%08X', 'u8'
                else:
                    z = b.find(b'\0\0\0\0', 1)
                    n = len(b) if z < 0 else z
                    while n < len(b) and b[n] == 0 and (cur + n) % 4:
                        n += 1  # keep trailing alignment zeros out of the pad
                    cls, nm, base = 'unknown', 'unk_%08X', 'u8'
                out.append(dict(ram=cur, size=n, base=base, arr=True, count=n, name=nm % cur, cls=cls,
                                comment='not referenced by any matched function', ev='gap between table objects'))
                cur += n
            if r:
                if r['ram'] + r['size'] > hi:
                    raise SystemExit('%08X runs past territory end %08X' % (r['ram'], hi))
                out.append(r)
                cur = r['ram'] + r['size']
    return out


class Emitter:
    def __init__(self, game, T, rom, defined):
        self.g, self.T, self.rom, self.defined = GAMES[game], T, rom, defined
        self.ext = set()

    def sym(self, v):
        if v == 0:
            return None
        if self.g['text'][0] <= v < self.g['text'][1]:
            s = 'func_%08X' % v
            if s not in self.defined:
                self.ext.add(('func', s))
            return s
        if 0x80000400 <= v < self.g['image_end']:
            s = 'D_%08X' % v
            if s not in self.defined:
                self.ext.add(('data', s))
            return '&' + s
        return None

    def prim(self, base, a):
        sz, fmt, _ = PRIM[base]
        v = struct.unpack('>' + fmt, self.rom[a - OFF:a - OFF + sz])[0]
        if base in ('f32', 'f64'):
            if not math.isfinite(v):
                raise SystemExit('%08X: non-finite %s' % (a, base))
            t = ('%.9g' if base == 'f32' else '%.17g') % v
            if '.' not in t and 'e' not in t and 'n' not in t:
                t += '.0'
            if v == 0 and math.copysign(1, v) < 0:
                t = '-0.0'
            return t + ('f' if base == 'f32' else '')
        if base == 'word':
            s = self.sym(v)
            return '(u32)' + s.lstrip('&') if s and s.startswith('func') else ('(u32)' + s if s else '0x%08X' % v)
        if base == 'ptr':
            s = self.sym(v)
            return s if s else ('NULL' if v == 0 else '(void *)0x%08X' % v)
        if base in ('u8', 'u16', 'u32', 'char'):
            return '0x%0*X' % (sz * 2, v)
        return str(v)

    def chars(self, a, n):
        b = self.rom[a - OFF:a - OFF + n]
        z = b.find(b'\0')
        body = b if z < 0 else b[:z]
        if (z < 0 and False) or (z >= 0 and any(b[z:])) or not all(32 <= c < 127 for c in body):
            return '{ ' + ', '.join('0x%02X' % c for c in b) + ' }'
        if z < 0:
            return None  # exactly full, no NUL: not expressible as literal without losing length intent
        s = body.decode('ascii').replace('\\', '\\\\').replace('"', '\\"')
        return '"%s"' % s

    def value(self, base, a, cnt=0):
        if cnt:
            if base == 'char':
                c = self.chars(a, cnt)
                if c:
                    return c
                base = 'u8'
            sz = self.T.info(base)[0]
            return '{ ' + ', '.join(self.value(base, a + i * sz) for i in range(cnt)) + ' }'
        if base in PRIM:
            return self.prim(base, a)
        st = self.T.structs[base]
        return '{ ' + ', '.join(self.value(b, a + o, c) for (_, b, c, o) in st['fields']) + ' }'

    def obj(self, r):
        ct = self.T.ctype(r['base'])
        head = '/* ram 0x%08X rom 0x%06X size 0x%X class %s: %s */\n' % (
            r['ram'], r['ram'] - OFF, r['size'], r['cls'], r['comment'])
        if r['cls'] == 'pad':
            return head + '%s %s[%d] = { 0 };\n' % (ct, r['name'], r['count'])
        if not r['arr']:
            return head + '%s %s = %s;\n' % (ct, r['name'], self.value(r['base'], r['ram']))
        esz = self.T.info(r['base'])[0]
        if r['base'] == 'char':
            return head + 'char %s[%d] = %s;\n' % (r['name'], r['count'], self.value('char', r['ram'], r['count']))
        per = 16 if esz == 1 else (8 if esz == 2 else (4 if r['base'] in PRIM else 1))
        lines = []
        for i in range(0, r['count'], per):
            lines.append('    ' + ', '.join(self.value(r['base'], r['ram'] + j * esz)
                                             for j in range(i, min(i + per, r['count']))) + ',')
        return head + '%s %s[%d] = {\n%s\n};\n' % (ct, r['name'], r['count'], '\n'.join(lines))


def struct_c(T, name):
    st = T.structs[name]
    fl = ' '.join('%s %s%s;' % (T.ctype(b), fn, '[%d]' % c if c else '') for (fn, b, c, _) in st['fields'])
    return 'typedef struct { %s } %s; /* size 0x%X */' % (fl, name, st['size'])


def generate(rom_path, game, outdir):
    rom = open(rom_path, 'rb').read()
    T = Types()
    objs = tile(game, load_table(game, T), rom)
    defined = {o['name'] for o in objs}
    E = Emitter(game, T, rom, defined)
    body = [E.obj(o) for o in objs]
    used = {o['base'] for o in objs}
    need, seen = [], set()

    def dep(n):
        if n in seen or n in PRIM:
            return
        seen.add(n)
        for (_, b, _, _) in T.structs[n]['fields']:
            dep(b)
        need.append(n)
    for n in T.order:
        if n in used:
            dep(n)
    hdr = ['/* data1 territory of the %s program .data, generated by gen_data.py from the ROM. */' % game,
           'typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;',
           'typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;',
           '#define NULL ((void *)0)']
    hdr += [struct_c(T, n) for n in need]
    hdr += ['extern void %s(void);' % s for k, s in sorted(E.ext) if k == 'func']
    hdr += ['extern u8 %s[];' % s for k, s in sorted(E.ext) if k == 'data']
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, 'data1_%s.c' % game), 'w') as f:
        f.write('\n'.join(hdr) + '\n\n' + '\n'.join(body))
    with open(os.path.join(outdir, 'MAP_%s.tsv' % game), 'w') as f:
        for o in objs:
            ct = T.ctype(o['base']) + ('[%d]' % o['count'] if o['arr'] else '')
            fmt = 'C %s' % ct if o['cls'] != 'pad' else 'zero fill'
            f.write('\t'.join([game, '0x%06X' % (o['ram'] - OFF), '0x%06X' % (o['ram'] + o['size'] - OFF),
                               o['cls'], fmt, '%s %s' % (o['name'], o['comment']), o['ev'],
                               'tools/gen_data.py' if o['cls'] != 'pad' else 'tools/gen_data.py (pad)']) + '\n')
    print('%s: %d objects, %d table rows, %d bytes' % (game, len(objs), sum(1 for o in objs if o['ev'] != 'gap between table objects'),
                                                       sum(o['size'] for o in objs)))


# ---------------- check: parse the emitted C back without a compiler ----------------
TOK = re.compile(r'"(?:\\.|[^"\\])*"|\(u32\)|\(void \*\)|&?(?:D|func)_[0-9A-F]{8}|NULL|'
                 r'-?0x[0-9A-Fa-f]+|-?\d+(?:\.\d*)?(?:[eE][-+]?\d+)?f?|[{}]')


def check(rom_path, game, outdir):
    rom = open(rom_path, 'rb').read()
    src = open(os.path.join(outdir, 'data1_%s.c' % game)).read()
    T = Types()
    for m in re.finditer(r'typedef struct \{ (.*?) \} (\w+);', src):
        body = ';'.join(re.sub(r'void \*', 'ptr ', f).replace('char ', 'char ') for f in m.group(1).split(';'))
        rev = {v[2]: k for k, v in PRIM.items() if k not in ('word', 'ptr')}
        fields = []
        for f in [x.strip() for x in body.split(';') if x.strip()]:
            t, rest = f.split(None, 1)
            fields.append('%s %s' % ('ptr' if t == 'ptr' else rev.get(t, t), rest))
        if m.group(2) == 'Gfx':
            fields = ['u32 w0', 'word w1']
        T.structs.pop(m.group(2), None)
        T.define(m.group(2), ';'.join(fields))
    objs = re.findall(r'/\* ram 0x([0-9A-F]{8}) rom 0x[0-9A-F]+ size 0x([0-9A-F]+) class (\w+).*?\*/\n'
                      r'([\w ]+?\*?)\s*(\w+)(?:\[(\d+)\])? = (.*?);\n(?=\n*/\*|\s*\Z)', src, re.S)
    bad, total, cover = 0, 0, []
    for ram, size, cls, ctype, name, cnt, init in objs:
        ram, size = int(ram, 16), int(size, 16)
        ctype = ctype.strip()
        base = {'void *': 'ptr', 'char': 'char'}.get(ctype, ctype)
        n = int(cnt) if cnt else 1
        toks = [t for t in TOK.findall(init) if t not in ('{', '}', '(u32)', '(void *)')]
        out = bytearray()

        def ser(b, cnt):
            if cnt:
                if b == 'char' and toks and toks[0].startswith('"'):
                    s = bytes(toks.pop(0)[1:-1], 'ascii').decode('unicode_escape').encode('latin-1')
                    out.extend(s + bytes(cnt - len(s)))
                    return
                for _ in range(cnt):
                    ser(b, 0)
                return
            if b in PRIM:
                sz, fmt, _ = PRIM[b]
                t = toks.pop(0) if toks else '0'
                if t == 'NULL':
                    v = 0
                elif re.fullmatch(r'&?(D|func)_[0-9A-F]{8}', t):
                    v = int(t[-8:], 16)
                elif b in ('f32', 'f64'):
                    v = float(t.rstrip('f'))
                else:
                    v = int(t, 0)
                    if fmt.isupper():
                        v &= (1 << (8 * sz)) - 1
                out.extend(struct.pack('>' + fmt, v))
                return
            st = T.structs[b]
            start = len(out)
            for (_, fb, fc, fo) in st['fields']:
                out.extend(bytes(start + fo - len(out)))
                ser(fb, fc)
            out.extend(bytes(start + st['size'] - len(out)))
        ser(base, n if cnt else 0)
        if toks:
            print('%s: %d initialiser tokens left over' % (name, len(toks)))
            bad += 1
        ref = rom[ram - OFF:ram - OFF + size]
        if bytes(out) != ref:
            d = next((i for i in range(min(len(out), len(ref))) if out[i] != ref[i]), min(len(out), len(ref)))
            print('MISMATCH %s at +0x%X (built %d bytes, want %d)' % (name, d, len(out), len(ref)))
            bad += 1
        total += size
        cover.append((ram, ram + size))
    cover.sort()
    want = GAMES[game]['pieces']
    merged = []
    for a, b in cover:
        if merged and merged[-1][1] == a:
            merged[-1] = (merged[-1][0], b)
        else:
            if merged and a < merged[-1][1]:
                print('overlap at %08X' % a); bad += 1
            merged.append((a, b))
    if merged != list(want):
        print('coverage %s != territory %s' % ([(hex(a), hex(b)) for a, b in merged], [(hex(a), hex(b)) for a, b in want]))
        bad += 1
    print('%s: %d objects, %d bytes re-serialised, %s' % (game, len(objs), total, 'OK' if not bad else '%d FAILURES' % bad))
    return bad


if __name__ == '__main__':
    a = sys.argv[1:]
    if a and a[0] == 'check':
        sys.exit(1 if check(*a[1:4]) else 0)
    generate(*a[:3])
