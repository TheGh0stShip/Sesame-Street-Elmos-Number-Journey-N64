#!/usr/bin/env python3
"""extract_hud.py ROM GAME OUTDIR        -> OUTDIR/<game>/{tex/*.png, anim/*.json, pad/}, index.json, MAP_<game>.tsv
   extract_hud.py check ROM GAME OUTDIR  -> re-encode every PNG/JSON, rebuild the territory, compare with the ROM

Code-only textures of the `common` block (RAM 0x8024B000), i.e. the ranges agent packC left as raw .bin because
only code-built display lists draw them. Every format/size below is taken from the G_SETTILE / G_LOADBLOCK /
G_SETTILESIZE words the HUD code writes (see ../FORMAT.md); no ROM bytes are stored here."""
import sys, os, json, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import n64tex, png

BASE = 0x8024B000
GAME = {
    'letter': dict(rom=0x10D0F0, size=0xB2600, data=(0x8006C9AC, 0x6D5AC)),
    'number': dict(rom=0x10D560, size=0x6A120, data=(0x8006E31C, 0x6EF1C)),
}
# packC ranges handed to this agent (RAM start, RAM end): the territory
TERR = {
 'letter': [(0x8024B000, 0x8026F910), (0x80287DD0, 0x8028BDB0), (0x8029E270, 0x802A45B8), (0x802B2DC8, 0x802C6B60)],
 'number': [(0x8024B000, 0x80259630), (0x8027FCD8, 0x80293A70)],
}
# vertex morph animations that packC took for textures: start addresses (pointer words in .data anim tables,
# Letter D_80076B20[type] -> Anim*[], read by func_8003CF50 / func_8003BB00 into actor+0x1BC)
ANIMS = {
 'letter': [(0x80287DD0, 0x80075D34), (0x8029E270, 0x80076B18), (0x802A0178, 0x80076B1C),
            (0x802B2DC8, 0x80075D14), (0x802B641C, 0x80075D1C), (0x802BE598, 0x80075D20),
            (0x802C2878, None)],  # None: unreferenced, found as a valid header right after the previous one
 'number': [(0x8027FCD8, 0x800768B4), (0x8028332C, 0x800768BC), (0x8028B4A8, 0x800768C0), (0x8028F788, 0x800768C4)],
}
# RGBA32 32x32 tiles placed in the same "8 zero bytes + gsSPEndDisplayList + 0x1000 texels" layout as the
# referenced ones but referenced by nothing (Number only)
UNUSED = {'letter': [], 'number': [0x80250058, 0x80251068]}
END_DL = bytes.fromhex('B800000000000000')

def tables(game):
    """(name, ram, fmt, bits, w, h, tlut_ram, tlut_bytes, drawn_by)"""
    T = []
    def add(name, ram, fmt, bits, w, h, tl, tlsz, by):
        T.append(dict(name=name, ram=ram, fmt=fmt, bits=bits, w=w, h=h, tlut=tl, tlsz=tlsz, by=by))
    hudf = 'func_80004010' if game == 'letter' else 'func_80003E18'
    fade = 'func_80018278' if game == 'letter' else 'func_800186E8'
    add('fade_tile', 0x8024B008, 'RGBA', 32, 32, 32, None, 0, fade + ' (screen fade tile)')
    for i, a in enumerate((0x8024C018, 0x8024D028, 0x8024E038, 0x8024F048)):
        add('pic64_%s' % ('tl', 'tr', 'bl', 'br')[i], a, 'RGBA', 32, 32, 32, None, 0,
            hudf + ' HUD bit 2: quarter %d of a 64x64 picture, pulsing alpha' % i)
    for i, a in enumerate(UNUSED[game]):
        add('unused_rgba32_%d' % i, a, 'RGBA', 32, 32, 32, None, 0, 'UNUSED: no code, data or display-list reference')
    if game == 'letter':
        ci8_24 = [(0x80250050, 0x80250250), (0x80250490, 0x80250690), (0x802508D0, 0x80250AD0), (0x80250D10, 0x80250F10)]
        ci4_16 = [(0x80251150, 0x80251170), (0x802511F0, 0x80251210), (0x80251290, 0x802512B0)]
        lists = [('glyph16', 0x8006DE28, 52, 'CI', 8, 16, 0x200), ('glyph32', 0x8006DFC8, 52, 'CI', 8, 32, 0x200),
                 ('icon32', 0x8006DDF0, 7, 'CI', 4, 32, 0x20)]
        single16 = (0x8026E730, 0x8026E930)
    else:
        ci8_24 = [(0x80258350, 0x80258550), (0x80258790, 0x80258990), (0x80258BD0, 0x80258DD0), (0x80259010, 0x80259210)]
        ci4_16 = [(0x80259450, 0x80259470), (0x802594F0, 0x80259510), (0x80259590, 0x802595B0)]
        lists = [('glyph16', 0x8006F798, 9, 'CI', 8, 16, 0x200), ('glyph32', 0x8006F7E0, 9, 'CI', 8, 32, 0x200),
                 ('icon32', 0x8006F760, 7, 'CI', 4, 32, 0x20)]
        single16 = (0x80257170, 0x80257370)
    for i, (t, a) in enumerate(ci8_24):
        add('pic48_%s' % ('tl', 'tr', 'bl', 'br')[i], a, 'CI', 8, 24, 24, t, 0x200, hudf + ' HUD bit 2: quarter %d of a 48x48 picture' % i)
    bits4 = ('4/0x200/0x400', '8', '0x800/0x1000')
    for i, (t, a) in enumerate(ci4_16):
        add('icon16_%d' % i, a, 'CI', 4, 16, 16, t, 0x20, hudf + ' HUD bits %s: small 16x16 icon' % bits4[i])
    add('glyph16_empty', single16[1], 'CI', 8, 16, 16, single16[0], 0x200, hudf + ' HUD bit 32: placeholder for a not-yet-collected glyph slot')
    return T, lists

def label(game, name, i):
    if name.startswith('glyph'):
        if game == 'letter':
            return 'letter %s' % (chr(65 + i) if i < 26 else chr(97 + i - 26))
        return 'digit %d' % (i + 1)
    return 'icon %d' % i

def build(rom, game):
    g = GAME[game]
    blob = rom[g['rom']:g['rom'] + g['size']]
    def rd(a, n): return blob[a - BASE:a - BASE + n]
    def dword(a):  # .data word
        o = a - g['data'][0] + g['data'][1]
        return struct.unpack('>I', rom[o:o + 4])[0]
    T, lists = tables(game)
    hudf = 'func_80004010' if game == 'letter' else 'func_80003E18'
    for name, tab, n, fmt, bits, wh, tls in lists:
        for i in range(n):
            tl, im = dword(tab + 8 * i), dword(tab + 8 * i + 4)
            T.append(dict(name='%s_%02d' % (name, i), ram=im, fmt=fmt, bits=bits, w=wh, h=wh, tlut=tl, tlsz=tls,
                          by='%s via TexPair table 0x%08X[%d] (%s)' % (hudf, tab, i, label(game, name, i))))
    objs = []
    for t in T:
        t['size'] = n64tex.nbytes(t['fmt'], t['bits'], t['w'], t['h'])
        objs.append((t['ram'], t['ram'] + t['size'], 'texture', t))
        if t['tlut']:
            objs.append((t['tlut'], t['tlut'] + t['tlsz'], 'tlut', t))
    for k, (a, word) in enumerate(ANIMS[game]):
        nxt = ANIMS[game][k + 1][0] if k + 1 < len(ANIMS[game]) else None
        end = [e for s_, e in TERR[game] if s_ <= a < e][0]
        if nxt and nxt < end: end = nxt
        h = rd(a, 8); he, do, tk, nk, fl = struct.unpack('>HHBBH', h)
        nu = (end - a - do) // (4 * nk)
        objs.append((a, a + do + 4 * nk * nu, 'morph_anim', dict(name='anim_%08X' % a, ram=a, hdr_end=he, data_off=do, ticks=tk,
                     nkey=nk, flags=fl, nunique=nu, by=('.data anim table word 0x%08X -> actor+0x1BC (func_%s)' % (word,
                     '8003CF50/func_8003BB00' if game == 'letter' else '8003D3D0')) if word else
                     'UNUSED: no pointer anywhere; follows the anim at 0x%08X with the same vertex count' % ANIMS[game][k - 1][0])))
    objs.sort(key=lambda o: o[0])
    # fill the territory: gaps are pad (zero), an 8-byte end-DL stub, or unknown
    full = []
    for ts, te in TERR[game]:
        cur = ts
        for o in [o for o in objs if ts <= o[0] < te] + [(te, te, None, None)]:
            a = o[0]
            assert a >= cur, ('overlap', hex(a), hex(cur))
            while cur < a:
                g = rd(cur, a - cur)
                j = g.find(END_DL)
                if j >= 0 and j % 8 == 0 and (cur + j) % 8 == 0:
                    if j: full.append((cur, cur + j, 'pad' if not any(g[:j]) else 'unknown', None))
                    full.append((cur + j, cur + j + 8, 'dl', None)); cur += j + 8
                else:
                    full.append((cur, a, 'pad' if not any(g) else 'unknown', None)); cur = a
            if o[2]:
                full.append(o); cur = o[1]
        assert cur == te
    return blob, full, rd

def out_name(o):
    a, e, k, t = o
    if k == 'texture': return 'tex/%08X_%s.png' % (a, t['name'])
    if k == 'tlut': return None  # stored in the PNG of its texture
    if k == 'morph_anim': return 'anim/%08X.json' % a
    if k == 'dl': return 'dl/%08X_enddl.gbi.txt' % a
    return None

def anim_decode(b, t):
    do, nk, nu = t['data_off'], t['nkey'], t['nunique']
    s10 = lambda v, n: v - (1 << n) if v & (1 << (n - 1)) else v
    verts = []
    for v in range(nu):
        row = []
        for k in range(nk):
            w = struct.unpack('>I', b[do + 4 * (v * nk + k):do + 4 * (v * nk + k) + 4])[0]
            row.append([s10(w & 0x7FF, 11), s10((w >> 11) & 0x7FF, 11), s10(w >> 22, 10)])
        verts.append(row)
    return dict(format='sprA vertex morph animation (see FORMAT.md)', ram='0x%08X' % t['ram'], hdr_end=t['hdr_end'],
                data_off=do, ticks=t['ticks'], nkey=nk, flags=t['flags'], key_ticks=list(b[8:8 + nk]),
                hdr_pad=b[8 + nk:do].hex(), nunique=nu, verts_keys_xyz=verts)

def anim_encode(j):
    b = bytearray(struct.pack('>HHBBH', j['hdr_end'], j['data_off'], j['ticks'], j['nkey'], j['flags']))
    b += bytes(j['key_ticks']) + bytes.fromhex(j['hdr_pad'])
    for row in j['verts_keys_xyz']:
        for x, y, z in row:
            b += struct.pack('>I', ((z & 0x3FF) << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF))
    return bytes(b)

def rom_of(game, ram):
    return ram - BASE + GAME[game]['rom']

def extract(rom, game, outdir):
    blob, full, rd = build(rom, game)
    od = os.path.join(outdir, game)
    for d in ('tex', 'anim', 'dl'): os.makedirs(os.path.join(od, d), exist_ok=True)
    index = []; rows = []
    tl_owner = {}
    for o in full:
        if o[2] == 'texture' and o[3]['tlut']: tl_owner[o[3]['tlut']] = o[3]
    for o in full:
        a, e, k, t = o
        fn = out_name(o)
        if k == 'texture':
            pal = rd(t['tlut'], t['tlsz']) if t['tlut'] else None
            n64tex.decode(t['fmt'], t['bits'], t['w'], t['h'], rd(a, e - a), pal).save(os.path.join(od, fn))
            fmt = '%s%d %dx%d' % (t['fmt'], t['bits'], t['w'], t['h'])
            desc = '%s %s%s' % (t['name'], fmt, ' with TLUT 0x%08X' % t['tlut'] if t['tlut'] else '')
            ref = t['by']; ex = 'extract_hud.py -> ' + fn
        elif k == 'tlut':
            ow = tl_owner[a]
            fmt = 'RGBA16 palette[%d]' % (t['tlsz'] // 2)
            desc = 'TLUT of %s (G_LOADTLUT loads 256 entries%s)' % (ow['name'], '; CI4 renders with palette 0 = these 16 colours' if t['tlsz'] == 0x20 else '')
            ref = t['by']; ex = 'extract_hud.py -> PLTE/tRNS of tex/%08X_%s.png' % (ow['ram'], ow['name'])
        elif k == 'morph_anim':
            j = anim_decode(rd(a, e - a), t)
            json.dump(j, open(os.path.join(od, fn), 'w'))
            fmt = 'MorphAnim(nkey=%d, nvert=%d)' % (t['nkey'], t['nunique'])
            desc = 'vertex morph animation (sprA format, %d ticks); packC had it as a code-set texture' % t['ticks']
            ref = t['by']; ex = 'extract_hud.py -> ' + fn
        elif k == 'dl':
            open(os.path.join(od, fn), 'w').write('gsSPEndDisplayList()\n')
            fmt = 'f3dex_dl'; desc = 'empty display list (gsSPEndDisplayList) that the build places before each RGBA32 tile'
            ref = 'none (0x8024B000 is the common block load address in the level loader)' if a == BASE else 'none (separator)'
            ex = 'extract_hud.py -> ' + fn
        elif k == 'pad':
            fmt = 'zero'; desc = 'zero padding'; ref = '-'; ex = 'extract_hud.py check (zero fill)'
        else:
            fmt = 'raw'; desc = 'unidentified'; ref = '-'; ex = 'none'
        rows.append('%s\t0x%06X\t0x%06X\t%s\t%s\t%s [RAM 0x%08X]\t%s\t%s' % (game, rom_of(game, a), rom_of(game, e), k, fmt, desc, a, ref, ex))
        index.append(dict(ram=a, end=e, cls=k, file=fn, tex=(dict((x, t[x]) for x in ('fmt', 'bits', 'w', 'h', 'tlut', 'tlsz')) if k == 'texture' else None)))
    json.dump(index, open(os.path.join(od, 'index.json'), 'w'), indent=0)
    open(os.path.join(od, 'MAP_%s.tsv' % game), 'w').write('\n'.join(rows) + '\n')
    from collections import Counter
    c = Counter(); n = Counter()
    for a, e, k, t in full: c[k] += e - a; n[k] += 1
    print(game, dict(n), dict(c))

def check(rom, game, outdir):
    od = os.path.join(outdir, game)
    index = json.load(open(os.path.join(od, 'index.json')))
    g = GAME[game]
    img = {}
    for it in index:
        a, e, k = it['ram'], it['end'], it['cls']
        if k == 'texture':
            t = it['tex']; im = png.open_png(os.path.join(od, it['file']))
            assert im.size == (t['w'], t['h'])
            tex, tl = n64tex.encode(t['fmt'], t['bits'], im)
            img[a] = tex
            if t['tlut']:
                img[t['tlut']] = tl[:t['tlsz']]
        elif k == 'morph_anim':
            img[a] = anim_encode(json.load(open(os.path.join(od, it['file']))))
        elif k == 'dl':
            assert open(os.path.join(od, it['file'])).read().strip() == 'gsSPEndDisplayList()'
            img[a] = END_DL
        elif k == 'pad':
            img[a] = bytes(e - a)
    bad = 0; tot = 0
    for it in index:
        a, e = it['ram'], it['end']
        if it['cls'] == 'unknown': continue
        want = rom[rom_of(game, a):rom_of(game, e)]
        got = img.get(a)
        tot += e - a
        if got != want:
            bad += 1; print('MISMATCH', game, it['cls'], hex(a))
    # territory coverage: index tiles TERR exactly
    spans = sorted((it['ram'], it['end']) for it in index)
    for (a, e), (a2, e2) in zip(spans, spans[1:]):
        assert e <= a2
    covered = sum(e - a for a, e in spans); terr = sum(e - a for a, e in TERR[game])
    assert covered == terr, (covered, terr)
    print(game, 'check: %d objects, %d bytes compared, %d mismatches; territory %d bytes fully tiled' % (len(index), tot, bad, terr))
    return bad == 0

if __name__ == '__main__':
    a = sys.argv[1:]
    if a[0] == 'check':
        ok = check(open(a[1], 'rb').read(), a[2], a[3]); sys.exit(0 if ok else 1)
    extract(open(a[0], 'rb').read(), a[1], a[2])


