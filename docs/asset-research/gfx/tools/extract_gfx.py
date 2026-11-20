#!/usr/bin/env python3
"""extract_gfx.py ROM OUTDIR          extract every graphics resource of Elmo's Letter Adventure / Number Journey
   extract_gfx.py check ROM OUTDIR    re-encode the extracted files and compare with the ROM bytes (lossless proof)

Writes OUTDIR/<game>/{textures,dl,models,vtx,anim}/..., manifest.json and map.tsv.
Needs gfx_<game>.json (code facts frozen from the matched sources by gen_tables.py) next to this script.
"""
import os, sys, json, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import blobs, f3dex, n64tex, gfxscan

HERE = os.path.dirname(os.path.abspath(__file__))
DATA_START = {'letter': 0x8006C9AC, 'number': 0x8006E31C}   # main .data start (DL scan is limited to data)
DATA_END = {'letter': 0x8007BAB0, 'number': 0x8007BF20}
# {charset string, Bitmap[]} used by the sprite text printer (letter func_8000DEBC, number func_8000E32C)
FONTS = {'letter': 0x80071CFC, 'number': 0x80072D7C}
# 1bpp 8x8 glyphs ' '..DEL for the debug printer (letter func_8000E7A0); it sits 0x164 after the font descriptor
DEBUG_FONTS = {g: (a + 0x164, 96 * 8) for g, a in FONTS.items()}
ANIM_LOADER = {'letter': 'func_80026920', 'number': 'func_80027D90'}


def u16(d, o): return struct.unpack_from('>H', d, o)[0]
def u32(d, o): return struct.unpack_from('>I', d, o)[0]


def game_of(rom):
    t = rom[0x20:0x34]
    if b'Number' in t: return 'number'
    if b'Letter' in t: return 'letter'
    raise SystemExit('not an Elmo ROM: %r' % t)


class Game:
    def __init__(self, rom):
        self.rom = rom
        self.game = game_of(rom)
        self.facts = json.load(open(os.path.join(HERE, 'gfx_%s.json' % self.game)))
        self.imgs, self.meta = gfxscan.load(rom, self.game)
        self.items = []          # dict(kind, img, addr, size, ...)
        self.tex = {}            # key -> texture dict
        self.dlset = {}          # (img, addr) -> size

    # ---------- address helpers
    def space(self, img):
        return gfxscan.Space(self.imgs, img if img and img.startswith('level') else None)

    def owner(self, img, a, n=1):
        w = self.space(img).where(a, n)
        return w[0] if w else None

    def rd(self, img, a, n):
        return self.space(img).read(a, n)

    def romloc(self, img, a):
        base = self.imgs[img][0]
        s, e, comp, extra = self.meta[img]
        if img == 'main':
            return '0x%06X' % (s + a - base)
        if comp:
            return '%s+0x%05X' % (extra['gz_name'], a - base)
        return '0x%06X' % (s + a - base)

    def add(self, kind, img, a, size, **kw):
        it = dict(kind=kind, img=img, addr=a, size=size, **kw)
        self.items.append(it)
        return it

    # ---------- display lists
    def scan_dls(self):
        for img, (base, data) in self.imgs.items():
            for s, e in gfxscan.find_dls(data, base):
                a = base + s
                if img == 'main' and not (DATA_START[self.game] <= a < DATA_END[self.game]):
                    continue
                self.dlset[(img, a)] = e - s

    def find_models(self):
        self.models = []
        for img, (base, data) in self.imgs.items():
            if img == 'main':
                continue
            starts = {a for (i, a) in self.dlset if i == img}
            for o in range(0, len(data) - 0x28, 4):
                v = struct.unpack_from('>10I', data, o)
                if v[2] in starts and base <= v[3] < base + len(data) and base <= v[9] < base + len(data) \
                        and v[9] == base + o - 0x18 and (v[0] & 0xFFFF) and (v[7] == 0 or base <= v[7] < base + len(data)):
                    self.models.append(dict(img=img, addr=base + o, flags=v[0] >> 16, nvtx=v[0] & 0xFFFF, ntri=v[1] >> 16,
                                            dl=v[2], vtx=v[3], vtx2=v[4], rebuild=v[5], seg4=v[7], vbytes=v[8], bbox=v[9]))

    def trace(self, img, start, segs=None):
        t = f3dex.Tracer(lambda a, n: self.rd(img, a, n), segs)
        t.run(start)
        return t

    def keep_tex(self, img, t, origin):
        keys = []
        for x in t.textures:
            if x.get('row') is not None:   # LoadTile sub-rectangles: only the code-built backgrounds use them
                keys.append(None); continue
            size = n64tex.nbytes(x['fmt'], x['bits'], x['w'], x['h'])
            own = self.owner(img, x['addr'], size)
            if not own or x['fmt'] not in ('RGBA', 'CI', 'IA', 'I'):
                keys.append(None); continue
            tl = x.get('tlut')
            if x['fmt'] == 'CI' and (not tl or not self.owner(img, tl[0], tl[1] * 2)):
                tl = None
            key = (own, x['addr'], x['fmt'], x['bits'], x['w'], x['h'], tl[0] if tl else None)
            if key not in self.tex:
                self.tex[key] = dict(img=own, addr=x['addr'], fmt=x['fmt'], bits=x['bits'], w=x['w'], h=x['h'], size=size,
                                     tlut=tl, tlut_img=self.owner(img, tl[0], tl[1] * 2) if tl else None, origin=origin, ctx=img)
            keys.append(key)
        return keys

    def run_all(self):
        self.scan_dls()
        self.find_models()
        self.vtx = collections.defaultdict(set)    # img -> {(addr, n)}
        self.objs = []
        reached = set()
        for m in self.models:
            t = self.trace(m['img'], m['dl'], {3: m['vtx'], 4: m['seg4'] or 0})
            keys = self.keep_tex(m['img'], t, 'model %08X' % m['addr'])
            reached.update((m['img'], a) for a in t.dls)
            for a, n in t.vtx:
                o = self.owner(m['img'], a, n * 16)
                if o: self.vtx[o].add((a, n))
            self.objs.append(('model', m['img'], m['addr'], t, keys))
        called = set()
        traces = {}
        for (img, a) in sorted(self.dlset):
            t = self.trace(img, a)
            traces[(img, a)] = t
            called.update((img, x) for x in t.dls)
        for (img, a), t in sorted(traces.items()):
            keys = self.keep_tex(img, t, 'dl %08X' % a)
            for va, n in t.vtx:
                o = self.owner(img, va, n * 16)
                if o: self.vtx[o].add((va, n))
            if (img, a) not in called and (img, a) not in reached and t.tris:
                self.objs.append(('dl', img, a, t, keys))
        self.roots = {k for k in self.dlset if k not in called}
        self.code_textures()
        self.font()
        self.anims()

    # ---------- textures the code builds display lists for
    def code_textures(self):
        main = self.imgs['main']
        for p in self.facts['programs']:
            img = p['level'] or 'common'
            tbls = [w1 for _, w1, _ in p['pairs'] if isinstance(w1, dict)]
            n = 1
            lim = {}
            for tb in tbls:
                b = tb['tbl']; end = self.facts['tables']['%08X' % b]
                cnt = 0
                while b + cnt * 8 + 8 <= end:
                    d = self.rd('main', b + cnt * 8, 8)
                    if not d: break
                    x, y = struct.unpack('>II', d)
                    if not (self.owner(img, x) and self.owner(img, y)): break
                    cnt += 1
                lim[b] = cnt
                n = max(n, cnt)
            for i in range(n):
                t = f3dex.Tracer(lambda a, k: self.rd(img, a, k))
                for w0, w1, ln in p['pairs']:
                    if isinstance(w1, dict):
                        if i >= lim[w1['tbl']]:
                            w1 = 0
                        else:
                            w1 = u32(self.rd('main', w1['tbl'] + i * 8 + w1['field'], 4), 0)
                    elif w1 is None:
                        w1 = 0
                    t.step(w0, w1, 0, 0)
                self.keep_tex(img, t, 'code %s' % p['func'])
        for im, pal in self.facts['backgrounds']:
            img = self.facts['background_level']
            key = (img, im, 'CI', 8, 320, 240, pal)
            self.tex[key] = dict(img=img, addr=im, fmt='CI', bits=8, w=320, h=240, size=320 * 240, tlut=(pal, 256),
                                 tlut_img=img, origin='code %s' % self.facts['background_func'], ctx=img)

    def font(self):
        FONT = FONTS[self.game]; FONT_SPRITE = FONT + 0xC
        """SDK sprite font: Bitmap[] {s16 width, width_img, s, t; u8 *buf; s16 actualHeight, LUToffset}."""
        self.fonts = []
        d = self.rd('main', FONT, 8)
        if not d: return
        chars_p, bm_p = struct.unpack('>II', d)
        chars = b''
        while True:
            c = self.rd('main', chars_p + len(chars), 1)
            if not c or c == b'\0': break
            chars += c
        sp = self.rd('main', FONT_SPRITE, 0x44)
        bmfmt, bmsiz = sp[0x30], sp[0x31]
        glyphs = []
        for i in range(len(chars)):
            w, wi, s, t, buf, ah, lo = struct.unpack('>hhhhIhh', self.rd('main', bm_p + 16 * i, 16))
            glyphs.append(dict(ch=chars[i:i + 1].decode('latin1'), width=w, width_img=wi, s=s, t=t, buf=buf, height=ah))
        bits = f3dex.BITS[bmsiz]
        buf = glyphs[0]['buf']; wimg = glyphs[0]['width_img']
        h = max(g['t'] + g['height'] for g in glyphs)
        # the atlas runs up to the charset string that follows it in .data
        if chars_p > buf:   # the atlas runs up to the charset string that follows it in .data
            h = (chars_p - buf) * 8 // bits // wimg
        fmt = f3dex.FMTS[bmfmt]
        size = wimg * h * bits // 8
        self.tex[('main', buf, fmt, bits, wimg, h, None)] = dict(img='main', addr=buf, fmt=fmt, bits=bits, w=wimg, h=h, size=size,
                                                                tlut=None, tlut_img=None, origin='sprite font', ctx='main')
        self.fonts.append(dict(charset=chars_p, charset_len=len(chars) + 1, bitmaps=bm_p, nbitmaps=len(chars), atlas=buf,
                               sprite=FONT_SPRITE, glyphs=glyphs))
        self.add('font_charset', 'main', chars_p, (len(chars) + 4) & ~3, note='charset string of the sprite font')
        self.add('font_bitmaps', 'main', bm_p, 16 * len(chars), note='SDK Bitmap[] glyph table')
        self.add('font_header', 'main', FONT, 8, note='{charset, Bitmap[]} font descriptor')

    # ---------- animations
    def anims(self):
        self.animfiles = []
        files = sorted(tuple(x) for x in self.facts['anim_files'])
        lo, hi = files[0][0], files[-1][1]
        known = set(files)
        # gaps inside the animation area that also hold animation files nobody loads
        cur = lo
        allf = []
        for s, e in files:
            if s > cur:
                allf.append((cur, s, False))
            allf.append((s, e, True))
            cur = max(cur, e)
        for s, e, used in allf:
            hdr = self.rom[s:s + 8]
            hl, off, last, nk, fl = struct.unpack('>HHBBH', hdr)
            if hl != 8 + nk or off != (hl + 7) & ~7:
                self.animfiles.append(dict(rom=s, end=e, used=used, ok=False)); continue
            keys = list(self.rom[s + 8:s + 8 + nk])
            nv = (e - s - off) // (nk * 4)
            # trailing zero padding after the packed vertices
            body = off + nv * nk * 4
            self.animfiles.append(dict(rom=s, end=e, used=used, ok=True, last=last, nkeys=nk, flags=fl, keys=keys,
                                       nverts=nv, data_off=off, body_end=s + body))

    # ---------- output
    def tex_bytes(self, t):
        d = self.rd(t['ctx'], t['addr'], t['size'])
        pal = self.rd(t['ctx'], t['tlut'][0], t['tlut'][1] * 2) if t['tlut'] else None
        return d, pal

    def write(self, out):
        DEBUG_FONT = DEBUG_FONTS[self.game]
        g = self.game
        root = os.path.join(out, g)
        for sub in ('textures', 'dl', 'models', 'vtx', 'anim'):
            os.makedirs(os.path.join(root, sub), exist_ok=True)
        man = dict(game=g, textures=[], dls=[], models=[], vtx=[], anims=[], fonts=[])
        tname = {}
        for key, t in sorted(self.tex.items(), key=lambda kv: (kv[0][0], kv[0][1], kv[0][2], kv[0][3], kv[0][4], kv[0][5], kv[0][6] or 0)):
            d, pal = self.tex_bytes(t)
            nm = '%s_%08X_%s%d_%dx%d%s.png' % (t['img'], t['addr'], t['fmt'], t['bits'], t['w'], t['h'],
                                              ('_pal%08X' % t['tlut'][0]) if t['tlut'] else '')
            n64tex.decode(t['fmt'], t['bits'], t['w'], t['h'], d, pal).save(os.path.join(root, 'textures', nm))
            tname[key] = nm
            man['textures'].append(dict(file='textures/' + nm, img=t['img'], ctx=t['ctx'], addr=t['addr'], size=t['size'], fmt=t['fmt'],
                                        bits=t['bits'], w=t['w'], h=t['h'], tlut=t['tlut'], tlut_img=t['tlut_img'],
                                        rom=self.romloc(t['img'], t['addr']), origin=t['origin']))
        for (img, a), n in sorted(self.dlset.items()):
            nm = '%s_%08X.txt' % (img, a)
            d = self.rd(img, a, n)
            with open(os.path.join(root, 'dl', nm), 'w') as f:
                f.write('/* %s %s 0x%08X (%s), %d commands%s */\n' % (g, img, a, self.romloc(img, a), n // 8,
                                                                     ', root' if (img, a) in self.roots else ''))
                f.write('Gfx dl_%s_%08X[] = {\n' % (img, a))
                for o in range(0, n, 8):
                    w0, w1 = struct.unpack_from('>II', d, o)
                    f.write('    %s, /* %08X %08X */\n' % (f3dex.disasm(w0, w1), w0, w1))
                f.write('};\n')
            man['dls'].append(dict(file='dl/' + nm, img=img, addr=a, size=n, rom=self.romloc(img, a), root=(img, a) in self.roots))
        for img, s in sorted(self.vtx.items()):
            for a, n in merge_vtx(s):
                d = self.rd(img, a, n * 16)
                vs = [list(struct.unpack_from('>hhhHhhBBBB', d, 16 * i)) for i in range(n)]
                nm = '%s_%08X.json' % (img, a)
                json.dump(dict(img=img, addr=a, count=n, fields='x y z flag s t r g b a (r g b may be a normal)', vtx=vs),
                          open(os.path.join(root, 'vtx', nm), 'w'))
                man['vtx'].append(dict(file='vtx/' + nm, img=img, addr=a, size=n * 16, rom=self.romloc(img, a)))
        for kind, img, a, t, keys in self.objs:
            nm = '%s_%s_%08X' % (kind, img, a)
            write_obj(os.path.join(root, 'models', nm), t, keys, tname, self, img)
            ent = dict(file='models/%s.obj' % nm, kind=kind, img=img, addr=a, tris=len(t.tris))
            if kind == 'model':
                m = [m for m in self.models if m['img'] == img and m['addr'] == a][0]
                ent.update(m)
            man['models'].append(ent)
        for af in self.animfiles:
            nm = 'rom_%06X.json' % af['rom']
            if af['ok']:
                raw = self.rom[af['rom']:af['end']]
                frames = []
                for k in range(af['nkeys']):
                    fr = []
                    for v in range(af['nverts']):
                        w = u32(raw, af['data_off'] + (v * af['nkeys'] + k) * 4)
                        fr.append([sx(w & 0x7FF, 11), sx((w >> 11) & 0x7FF, 11), sx(w >> 22, 10)])
                    frames.append(fr)
                json.dump(dict(rom=af['rom'], end=af['end'], loop=bool(af['flags'] & 0x8000), flags=af['flags'], last_frame=af['last'],
                               key_frames=af['keys'], nverts=af['nverts'], header_len=8 + af['nkeys'], data_off=af['data_off'],
                               tail_len=af['end'] - af['body_end'],
                               header_pad_hex=raw[8 + af['nkeys']:af['data_off']].hex(),
                               tail_hex=self.rom[af['body_end']:af['end']].hex(),
                               frames=frames), open(os.path.join(root, 'anim', nm), 'w'))
            man['anims'].append(dict(file='anim/' + nm, **{k: v for k, v in af.items() if k != 'keys'}))
        for fo in self.fonts:
            man['fonts'].append(fo)
            json.dump(fo, open(os.path.join(root, 'textures', 'font_glyphs.json'), 'w'), indent=1)
        if g == 'letter' or True:
            d = self.rd('main', DEBUG_FONT[0], DEBUG_FONT[1])
            from png import Img
            im = Img('1', (8 * 16, 8 * 6))
            for c in range(96):
                for y in range(8):
                    for x in range(8):
                        im.putpixel(((c % 16) * 8 + x, (c // 16) * 8 + y), (d[c * 8 + y] >> x) & 1)
            im.save(os.path.join(root, 'textures', 'main_%08X_debugfont_1bpp_8x8.png' % DEBUG_FONT[0]))
            man['debugfont'] = dict(file='textures/main_%08X_debugfont_1bpp_8x8.png' % DEBUG_FONT[0], addr=DEBUG_FONT[0], size=DEBUG_FONT[1],
                                    rom=self.romloc('main', DEBUG_FONT[0]))
        json.dump(man, open(os.path.join(root, 'manifest.json'), 'w'), indent=0)
        self.write_map(root, man)
        return man

    # ---------- coverage / map
    def write_map(self, root, man):
        DEBUG_FONT = DEBUG_FONTS[self.game]
        cov = collections.defaultdict(list)   # img -> [(start, end, class, fmt, desc, ref)]
        for t in man['textures']:
            cov[t['img']].append((t['addr'], t['addr'] + t['size'], 'texture', '%s%d %dx%d' % (t['fmt'], t['bits'], t['w'], t['h']),
                                  'texture', t['origin']))
            if t['tlut']:
                cov[t['tlut_img']].append((t['tlut'][0], t['tlut'][0] + t['tlut'][1] * 2, 'palette', 'RGBA16 TLUT x%d' % t['tlut'][1],
                                           'palette', t['origin']))
        for d in man['dls']:
            cov[d['img']].append((d['addr'], d['addr'] + d['size'], 'display_list', 'F3DEX 1.23 Gfx[]',
                                  'display list' + (' (root)' if d['root'] else ''), 'scan'))
        for v in man['vtx']:
            cov[v['img']].append((v['addr'], v['addr'] + v['size'], 'vertex', 'Vtx[16]', 'vertex buffer', 'G_VTX'))
        for m in self.models:
            cov[m['img']].append((m['addr'], m['addr'] + 0x28, 'model_header', 'model header 0x28', 'model header', 'actor tables / code'))
            cov[m['img']].append((m['bbox'], m['bbox'] + 0x18, 'model_bbox', 'f32[6]', 'model bounding box', 'model header +0x24'))
            if m['rebuild']:
                d = self.imgs[m['img']][1]; o = m['rebuild'] - self.imgs[m['img']][0]
                p = o
                while True:
                    a = u16(d, p); p += 2
                    if a == 0: break
                    b = u16(d, p); p += 2
                    if b == 0: break
                    p += 2 * b
                cov[m['img']].append((m['rebuild'], self.imgs[m['img']][0] + ((p + 3) & ~3), 'model_vtxmap', 'u16 runs',
                                      'animated-vertex rebuild list', 'model header +0x14'))
        for it in self.items:
            cov[it['img']].append((it['addr'], it['addr'] + it['size'], 'font', it['kind'], it.get('note', ''), 'func_8000D810'))
        cov['main'].append((DEBUG_FONT[0], DEBUG_FONT[0] + DEBUG_FONT[1], 'font', '1bpp 8x8 x96', 'debug font', 'func_8000E7A0'))
        lines = []
        g = self.game
        for img, (base, data) in self.imgs.items():
            s, e, comp, extra = self.meta[img]
            iv = sorted(set(cov.get(img, [])))
            # resolve overlaps: earlier start wins; trim later ones
            merged = []
            pos = base
            for a, b, c, f, dsc, ref in iv:
                if b <= pos: continue
                a = max(a, pos)
                merged.append((a, b, c, f, dsc, ref)); pos = b
            if img == 'main':
                gaps = []
            else:
                gaps = []
                pos = base
                for a, b, *_ in merged + [(base + len(data), base + len(data))]:
                    if a > pos: gaps.append((pos, a))
                    pos = max(pos, b)
            for a, b in gaps:
                chunk = data[a - base:b - base]
                cls = 'pad' if not any(chunk) else 'level_data'
                merged.append((a, b, cls, 'zero' if cls == 'pad' else '-', 'zero fill' if cls == 'pad' else
                               'non-graphics bytes of the level image (objects, collision, scripts; or unidentified)', '-'))
            merged.sort()
            # merge runs of same class
            runs = []
            for r in merged:
                if runs and runs[-1][2] == r[2] and runs[-1][1] == r[0] and r[2] in ('texture', 'palette', 'vertex', 'display_list', 'pad', 'level_data'):
                    x = runs[-1]
                    runs[-1] = (x[0], r[1], x[2], x[3] if x[3] == r[3] else 'mixed', x[4], x[5], x[6] + 1)
                else:
                    runs.append(r + (1,))
            for a, b, c, f, dsc, ref, cnt in runs:
                if comp:
                    rs = '%s+0x%05X' % (extra['gz_name'], a - base); re_ = '%s+0x%05X' % (extra['gz_name'], b - base)
                else:
                    rs = '0x%06X' % (s + a - base); re_ = '0x%06X' % (s + b - base)
                desc = '%s %s RAM 0x%08X-0x%08X%s' % (img, dsc, a, b, (' (%d items)' % cnt) if cnt > 1 else '')
                lines.append((g, rs, re_, c, f, desc, ref, 'extract_gfx.py'))
            if comp:
                lines.append((g, '0x%06X' % s, '0x%06X' % e, 'container', 'gzip (deflate, FNAME %s)' % extra['gz_name'],
                              '%s compressed image, %d bytes used + %d pad; inflates to 0x%X bytes at 0x%08X' % (img, extra['gz_used'], e - s - extra['gz_used'], len(data), base),
                              'func_8001D9D0/func_8001E2CC -> inflate', 'extract_gfx.py (blobs.gunzip)'))
        for af in self.animfiles:
            lines.append((g, '0x%06X' % af['rom'], '0x%06X' % (af.get('body_end', af['end'])), 'animation',
                          'packed vertex keyframes' if af['ok'] else '?',
                          'vertex animation, %s keys, %s verts%s' % (af.get('nkeys'), af.get('nverts'), '' if af['used'] else ' (UNUSED: no code reference)'),
                          ('%s stream via D_80106874' % ANIM_LOADER[g]) if af['used'] else 'none', 'extract_gfx.py'))
            if af.get('body_end', af['end']) < af['end']:
                tail = self.rom[af['body_end']:af['end']]
                lines.append((g, '0x%06X' % af['body_end'], '0x%06X' % af['end'], 'pad' if not any(tail) else 'unknown',
                              'zero' if not any(tail) else '-', 'animation file tail', '-', '-'))
        with open(os.path.join(root, 'map.tsv'), 'w') as f:
            for l in lines:
                f.write('\t'.join(l) + '\n')


def sx(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def merge_vtx(s):
    """merge vertex loads into contiguous buffers"""
    iv = sorted((a, a + 16 * n) for a, n in s)
    out = []
    for a, b in iv:
        if out and a <= out[-1][1]:
            out[-1][1] = max(out[-1][1], b)
        else:
            out.append([a, b])
    return [(a, (b - a) // 16) for a, b in out]


def write_obj(path, t, keys, tname, gm, img):
    vidx = {}
    vl = []; vt = []; fl = []
    cur = 'x'
    for (tri, tk) in t.tris:
        if any(v is None for v in tri) or any(gm.rd(img, a + 16 * i, 16) is None for a, i in tri):
            continue
        key = keys[tk] if tk is not None and tk < len(keys) else None
        mat = tname[key][:-4] if key in tname else 'untextured'
        if mat != cur:
            fl.append('usemtl %s' % mat); cur = mat
        tw, th = (key[4], key[5]) if key else (32, 32)
        idx = []
        for (a, i) in tri:
            k = (a, i)
            if k not in vidx:
                d = gm.rd(img, a + 16 * i, 16)
                x, y, z, _, s_, t_ = struct.unpack('>hhhHhh', d[:12])
                vl.append('v %d %d %d' % (x, y, z))
                vt.append('vt %.5f %.5f' % (s_ / 32.0 / tw, 1 - t_ / 32.0 / th))
                vidx[k] = len(vl)
            n = vidx[k]
            idx.append('%d/%d' % (n, n))
        fl.append('f ' + ' '.join(idx))
    with open(path + '.obj', 'w') as f:
        f.write('mtllib %s.mtl\n' % os.path.basename(path))
        f.write('\n'.join(vl + vt + fl) + '\n')
    with open(path + '.mtl', 'w') as f:
        for k in sorted(set(keys) - {None}):
            if k in tname:
                f.write('newmtl %s\nmap_Kd ../textures/%s\n' % (tname[k][:-4], tname[k]))
        f.write('newmtl untextured\n')


# ---------- lossless check
def check(rom_path, out):
    import png
    rom = open(rom_path, 'rb').read()
    gm = Game(rom)
    root = os.path.join(out, gm.game)
    man = json.load(open(os.path.join(root, 'manifest.json')))
    bad = 0; n = 0
    for t in man['textures']:
        im = png.open_png(os.path.join(root, t['file']))
        texel, tl = n64tex.encode(t['fmt'], t['bits'], im)
        d = gm.rd(t['ctx'], t['addr'], t['size'])
        ok = texel[:t['size']] == d
        if t['tlut']:
            pal = gm.rd(t['ctx'], t['tlut'][0], t['tlut'][1] * 2)
            ok = ok and tl[:len(pal)] == pal
        n += 1
        if not ok:
            bad += 1; print('MISMATCH texture', t['file'])
    for v in man['vtx']:
        j = json.load(open(os.path.join(root, v['file'])))
        b = b''.join(struct.pack('>hhhHhhBBBB', *x) for x in j['vtx'])
        n += 1
        if b != gm.rd(v['img'], v['addr'], v['size']):
            bad += 1; print('MISMATCH vtx', v['file'])
    for d in man['dls']:
        words = []
        for line in open(os.path.join(root, d['file'])):
            if line.startswith('    gs'):
                words.append(struct.pack('>II', *f3dex.assemble(line)))   # macros re-assembled, not the comment
        n += 1
        if b''.join(words) != gm.rd(d['img'], d['addr'], d['size']):
            bad += 1; print('MISMATCH dl', d['file'])
    for a in man['anims']:
        if not a['ok']: continue
        j = json.load(open(os.path.join(root, a['file'])))
        nk = len(j['key_frames'])
        hdr = struct.pack('>HHBBH', j['header_len'], j['data_off'], j['last_frame'], nk, j['flags']) + bytes(j['key_frames'])
        hdr += bytes.fromhex(j['header_pad_hex'])
        body = bytearray()
        for v in range(j['nverts']):
            for k in range(nk):
                x, y, z = j['frames'][k][v]
                body += struct.pack('>I', ((z & 0x3FF) << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF))
        blob = hdr + bytes(body) + bytes.fromhex(j['tail_hex'])
        n += 1
        if blob != rom[a['rom']:a['end']]:
            bad += 1; print('MISMATCH anim', a['file'])
    print('%s: %d items checked, %d mismatches' % (gm.game, n, bad))
    return bad


def main(argv):
    if argv and argv[0] == 'check':
        sys.exit(1 if check(argv[1], argv[2]) else 0)
    rom = open(argv[0], 'rb').read()
    gm = Game(rom)
    gm.run_all()
    man = gm.write(argv[1])
    c = collections.Counter('%s%d' % (t['fmt'], t['bits']) for t in man['textures'])
    print(gm.game, 'textures', len(man['textures']), dict(c), 'dls', len(man['dls']), 'roots', sum(d['root'] for d in man['dls']),
          'vtx', sum(v['size'] // 16 for v in man['vtx']), 'models', len(gm.models), 'objs', len(man['models']),
          'anims', len(man['anims']), 'unused anims', sum(not a['used'] for a in man['anims']))


if __name__ == '__main__':
    main(sys.argv[1:])
