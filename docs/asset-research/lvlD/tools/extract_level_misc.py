#!/usr/bin/env python3
"""Mesh headers, their non-graphics companions, camera models and the program's .data pointer tables
that point into the Elmo level images (Letter / Number).

usage: extract_level_misc.py ROM GAME OUTDIR          -> OUTDIR/<level>.json, common.json, data_tables.json, MAP.part.tsv
       extract_level_misc.py check ROM GAME OUTDIR    -> re-serialise every JSON structure, compare with the bytes

GAME = letter | number.  The ROM is only read.  Every root address below is taken from the game code
(the switch on the object id in the type 7 / 11 / 12 handlers, the switch on the level id in the camera
handler, and .data tables); see ../FORMAT.md.  Structures are then followed through their pointers.
Pointers in the JSON are RAM addresses (hex strings); `off` fields are offsets in the image file.
"""
import sys, os, json, struct, zlib

LBASE, CBASE = 0x80154000, 0x8024B000
ROMDATA = 0x7FFFF400                      # RAM - ROM inside the program segment

LEVELS = {
    'letter': [('streetab', 0x1BF6F0, 0x204D00, 2), ('chute', 0x204D00, 0x227BA0, 16), ('manor', 0x227BA0, 0x243E20, 15),
               ('orchard', 0x243E20, 0x2604B0, 18), ('garden', 0x2604B0, 0x28FDB0, 17), ('space', 0x28FDB0, 0x2ACFA0, 14),
               ('planet', 0x2ACFA0, 0x2D1FB0, 13), ('bonusabc', 0x2D1FB0, 0x2F9B60, 19)],
    'number': [('street', 0x177680, 0x1BCE90, 1), ('dunk', 0x1BCE90, 0x1D6CA0, 9), ('bump', 0x1D6CA0, 0x206CC0, 10),
               ('mansion', 0x206CC0, 0x22EC10, 8), ('maze', 0x22EC10, 0x25AF80, 7), ('ski', 0x25AF80, 0x27ADC0, 6),
               ('river', 0x27ADC0, 0x2B8FD0, 5), ('bonus123', 0x2B8FD0, 0x2E6FC0, 11)],
}
COMMON = {'letter': (0x10D0F0, 0x1BF6F0), 'number': (0x10D560, 0x177680)}
# object list root of each level (passed by Letter func_80018A60 / Number func_80019470 to the dispatcher)
OBJLIST = {'streetab': 0x801A9F5C, 'chute': 0x801934D4, 'manor': 0x8019411C, 'orchard': 0x8019906C, 'garden': 0x801AC70C,
           'space': 0x8018FD80, 'planet': 0x801A87FC, 'bonusabc': 0x801650D4, 'street': 0x801AA2A4, 'dunk': 0x8017D82C,
           'bump': 0x80174B50, 'mansion': 0x80177D44, 'maze': 0x8018F46C, 'ski': 0x80176BAC, 'river': 0x80197E10,
           'bonus123': 0x80167F40}

# ---- roots from the code -------------------------------------------------------------------------
# type 7 (Letter func_80023118 / Number func_80023A38): id -> (mesh header, collision-quad block or 0)
T7 = {
 'letter': {0x1502: (0x8029E244, 0), 0x3002: (0x8027E330, 0x80276850), 0x5501: (0x80272A88, 0x80271460),
            0x5505: (0x80292EF8, 0), 0x1015: (0x801AA230, 0x801AA110), 0x1501: (0x801AA640, 0x801A9C8C),
            0x2002: (0x801566E0, 0), 0x2003: (0x801576B0, 0), 0x2004: (0x80158680, 0), 0x2005: (0x801AA640, 0),
            0x2006: (0x801622D0, 0), 0x2009: (0x80173120, 0), 0x2107: (0x80296AC0, 0), 0x2503: (0x80154B70, 0),
            0x2504: (0x80155710, 0), 0x2505: (0x801791A0, 0), 0x3003: (0x8015ACC8, 0), 0x3004: (0x80156428, 0),
            0x4001: (0x8017DCA8, 0), 0x4002: (0x80182DAC, 0), 0x4003: (0x80188E20, 0), 0x4609: (0x8018CEC0, 0),
            0x5504: (0x80158878, 0), 0x5506: (0x8015D118, 0), 0x3503: (0x80155CB0, 0), 0x3505: (0x801580A0, 0)},
 'number': {0x1502: (0x80271CE4, 0), 0x3002: (0x80267E70, 0x80260390), 0x5501: (0x80274E88, 0x80273860),
            0x5505: (0x8025C5C8, 0), 0x1015: (0x801AA570, 0x801AA450), 0x5004: (0x80190F84, 0), 0x5006: (0x801900E8, 0),
            0x5503: (0x8017E810, 0), 0x5500: (0x80183188, 0), 0x5504: (0x801862A0, 0), 0x5506: (0x801893C0, 0),
            0x5502: (0x8018DFF8, 0), 0x6000: (0x80188B90, 0), 0x6001: (0x8017EC00, 0), 0x6002: (0x8019CAB0, 0),
            0x6503: (0x80190E50, 0), 0x6507: (0x80198AA8, 0), 0x6508: (0x801968F0, 0), 0x6509: (0x8019E040, 0),
            0x7000: (0x80179078, 0), 0x7002: (0x801826D8, 0), 0x7003: (0x801866A8, 0), 0x7004: (0x8017CA80, 0x8017C62C),
            0x7005: (0x8017E800, 0x8017E3AC), 0x7006: (0x80180580, 0x8018012C), 0x7009: (0x80184A38, 0),
            0x7011: (0x8017AD08, 0), 0x7016: (0x801868F0, 0x801867CC), 0x7502: (0x8019AB80, 0), 0x7503: (0x8019D388, 0),
            0x8001: (0x80178590, 0), 0x8002: (0x80178CB0, 0), 0x8003: (0x8017FD80, 0), 0x8004: (0x80179B00, 0),
            0x8005: (0x8017B618, 0), 0x8006: (0x80177E78, 0), 0x8888: (0x8019C450, 0), 0x8889: (0x8017CEE0, 0)},
}
# type 12 (Letter func_8003BB00 / Number func_8003D3D0) and Letter func_8003E3E0, Number type 11
# (func_800408D0 -> func_800409A0, anims switched in func_80040E34): id -> (mesh, [morph anims]);
# a string names the .data anim table the code indexes instead.
T12 = {
 'letter': {0x6601: (0x802870F8, [0x80287120]), 0x5601: (0x802806F0, [0x80280718]), 0x5602: (0x80284938, [0x80284960]),
            0x2106: (0x80157BA8, [0x80157BD0]), 0x7602: (0x8015F418, [0x8015F440]), 0x2606: (0x8015D950, [0x8015D978]),
            0x4605: (0x80160A88, [0x80160AB0]), 0x4606: (0x80163308, [0x80163330]), 0x4607: (0x80165B88, [0x80165BB0]),
            0x2104: (0x8028ED70, [0x8028ED98]), 0x2601: (0x80163458, [0x80163480]), 0x3103: (0x802A7F70, ('D_80076B20', 0)),
            0x4604: (0x8016E460, [0x8016E488]), 0x4601: (0x80173F40, [0x80173F68]), 0x3607: (0x80160058, [0x80160080]),
            0x2108: (0x8015EA10, [0x8015EA38]), 0x2101: (0x802990C0, [0x80299110]), 0x2109: (0x8015C0A0, [0x8015C0C8]),
            0x4603: (0x80169F00, [0x80169F28]), 0x2605: (0x8015B0D8, [0x8015B100]), 0x4101: (0x8015DD20, [0x8015DD48])},
 'number': {0x6601: (0x80270C38, [0x80270C60]), 0x5601: (0x8026A230, [0x8026A258]), 0x5602: (0x8026E478, [0x8026E4A0]),
            0x8102: (0x801B55A0, [0x801B55C8]), 0x8103: (0x801B7408, [0x801B7430]), 0x6102: (0x801AC870, [0x801AC898]),
            0x6104: (0x801B1B30, [0x801B1B58]), 0x7103: (0x8018C0C0, [0x8018C0E8]), 0x7601: (0x8018C8C8, ('D_800774A4', 0)),
            0x7101: (0x80188640, ('D_800774A4', 1))},
}
E3E0 = {'letter': {'case 0': (0x8029CA80, [0x8029CAD0]), 'default': (0x802990C0, [0x80299110])}, 'number': {}}
T11 = {'number': {0x5001: (0x801811A8, [0x801811D0, 0x80184FA8]), 0x5003: (0x80177360, [0x80177388, 0x801792B4]),
                  0x5005: (0x8016DA28, [0x8016DA50, 0x80170E34]), 0x6000: (0x801A4304, [0x801A2BD8, 0x801A2E58])},
       'letter': {}}
# camera (Letter func_8002A120 switch on D_8012EFD0 / Number func_8002C800 on D_8012F154): level id -> mesh
CAM = {'letter': {18: 0x80168D60, 15: 0x802E3118, 16: 0x802E3118, 14: 0x8015B7C8, 13: 0x8015F3A8, None: 0x802AEAC0},
       'number': {10: 0x801AA700, 5: 0x801BFF98, 6: 0x80187430, None: 0x8027B9D0}}
FUNCS = {
 'letter': dict(t7='func_80023118', t12='func_8003BB00', e3e0='func_8003E3E0', cam='func_8002A120', copy='func_80018530',
                camupd='func_8002AB50', t11='-'),
 'number': dict(t7='func_80023A38', t12='func_8003D3D0', e3e0='-', cam='func_8002C800', copy='func_800189A0',
                camupd='func_8002D0A4', t11='func_800408D0/func_800409A0/func_80040E34'),
}

# ---- .data tables (RAM start, n words, layout, user function(s), what) ------------------------------
DATA_TABLES = {
 'letter': [
  (0x8006DDF0, 14, 'pair', 'func_80004010', 'common', 'HUD icons {TLUT, CI texels} x7, index D_8013EB38[i].m4 (collected-item slots)'),
  (0x8006DE28, 104, 'pair', 'func_80004010', 'common', 'HUD letter glyphs {TLUT, CI texels} x52 (A-Z, a-z), index D_8012FD3C (target letter), drawn when D_8006EDD8 & 32'),
  (0x8006DFC8, 104, 'pair', 'func_80004010', 'common', 'HUD big letter glyphs {TLUT, CI texels} x52, index D_8012FD3C, drawn when D_8006EDD8 & 16 (entry 0 also in mode 22)'),
  (0x8006E5A0, 68, 'pair', 'func_80004010', 'bonusabc', 'bonus-game panels {TLUT, CI texels} x34, index D_800745F0, level 19 only, D_80106870 != 0 branch'),
  (0x8006E6B0, 68, 'pair', 'func_80004010', 'bonusabc', 'bonus-game panels x34, index D_800745F4, level 19, D_80106870 != 0'),
  (0x8006E7C0, 68, 'pair', 'func_80004010', 'bonusabc', 'bonus-game panels x34, index D_800745F5, level 19, D_80106870 == 0'),
  (0x8006E8D0, 106, 'pair', 'func_80004010', 'bonusabc', 'bonus-game panels x53, index D_800745F0, level 19, second branch'),
  (0x8006EA78, 106, 'pair', 'func_80004010', 'bonusabc', 'bonus-game panels x53, index D_800745F4, level 19, second branch'),
  (0x8006EC20, 106, 'pair', 'func_80004010', 'bonusabc', 'bonus-game panels x53, index D_800745F5, level 19, second branch'),
  (0x80074664, 44, 'pair', 'func_8001A318', 'menu22', 'mode-22 menu Elmo animation A {TLUT, CI4 64x64} x22 (packA)'),
  (0x80074714, 56, 'pair', 'func_8001A318', 'menu22', 'mode-22 menu Elmo animation B x28 (packA)'),
  (0x800747F4, 52, 'pair', 'func_8001A318', 'menu22', 'mode-22 menu Elmo animation C x26 (packA)'),
  (0x800748C4, 6, 'pair', 'func_8001A318', 'menu22', 'mode-22 menu difficulty buttons x3 (packA)'),
  (0x80075C34, 10, 'ptr', 'func_800243AC', 'space', 'texture-block frames (10 x 0xA00 bytes) of type-7 actor 0x4002 (space, mesh 0x80182DAC): entry [actor+0x1D4] (stepped every 8 frames, 0..9) is written into mesh header +0x1C, the segment-4 texture base'),
  (0x80075D00, 20, 'ptr', 'func_8002AB50', 'level+common', 'camera-actor morph animations, index actor+0x1D4 (anim for the camera model of func_8002A120)'),
  (0x80076360, 1, 'ptr', '(no code reference found)', 'common', 'pointer to the morph anim of map object 0x6601 (0x80287120); no instruction addresses this word'),
  (0x80076B18, 2, 'ptr', 'via D_80076B20', 'common', 'anim list for map object 0x3103: anims [actor+0x1DA]'),
  (0x80076B20, 1, 'ptr', 'func_8003BB00, func_8003CF50', 'program', 'table of anim lists, index actor+0x1DB (only list: D_80076B18)'),
 ],
 'number': [
  (0x8006F760, 14, 'pair', 'func_80003E18', 'common', 'HUD icons {TLUT, CI texels} x7'),
  (0x8006F798, 18, 'pair', 'func_80003E18', 'common', 'HUD digit glyphs {TLUT, CI texels} x9'),
  (0x8006F7E0, 18, 'pair', 'func_80003E18', 'common', 'HUD big digit glyphs x9'),
  (0x8006FC60, 18, 'pair', 'func_80003E18', 'bonus123', 'bonus-game panels {TLUT, CI texels} x9, index D_8006FE5C / D_8006FE5D (level 11)'),
  (0x8006FCA8, 20, 'pair', 'func_80003E18', 'bonus123', 'bonus-game panels x10, index D_8006FE5C / D_8006FE5E (level 11)'),
  (0x80075240, 44, 'pair', 'func_8001AC1C', 'menu22', 'mode-22 menu Elmo animation A x22 (packA)'),
  (0x800752F0, 56, 'pair', 'func_8001AC1C', 'menu22', 'mode-22 menu Elmo animation B x28 (packA)'),
  (0x800753D0, 52, 'pair', 'func_8001AC1C', 'menu22', 'mode-22 menu Elmo animation C x26 (packA)'),
  (0x800754A0, 6, 'pair', 'func_8001AC1C', 'menu22', 'mode-22 menu difficulty buttons x3 (packA)'),
  (0x800768A0, 29, 'ptr', 'func_8002D0A4', 'level+common', 'camera-actor morph animations, index actor+0x1D4'),
  (0x80076A00, 27, 'ptr', 'func_80035260', 'common', 'model headers, one picked at random (func_800217C4(9) selects the index)'),
  (0x80076CE0, 1, 'ptr', '(no code reference found)', 'common', 'pointer to the morph anim of map object 0x6601 (0x80270C60); no instruction addresses this word'),
  (0x80077494, 4, 'ptr', 'via D_800774A4', 'level', 'two anim lists of 2 for map objects 0x7601 / 0x7101: anims [actor+0x1DA]'),
  (0x800774A4, 2, 'ptr', 'func_8003D3D0, func_8003E078', 'program', 'table of anim lists, index actor+0x1DB'),
  (0x800774B0, 9, 'ptr', 'func_8003FC40', 'common', 'model headers (list of 9)'),
 ],
}

ROMNAME = None
def u32(b, o): return struct.unpack_from('>I', b, o)[0]
def u16(b, o): return struct.unpack_from('>H', b, o)[0]
def hx(a): return '0x%08X' % a
def f32(u): return struct.unpack('>f', struct.pack('>I', u))[0]


class Image:
    def __init__(s, name, base, data):
        s.name, s.base, s.d = name, base, data
    def has(s, a, n=1): return s.base <= a and a + n <= s.base + len(s.d)
    def w(s, a): return u32(s.d, a - s.base)
    def h(s, a): return u16(s.d, a - s.base)
    def b(s, a, n): return s.d[a - s.base:a - s.base + n]


def load_images(rom, game):
    imgs = {}
    for name, a, b, lid in LEVELS[game]:
        z = zlib.decompressobj(31)
        imgs[name] = Image(name, LBASE, z.decompress(rom[a:b]))
        if not z.eof: raise SystemExit('%s: gzip member does not end inside ROM 0x%X-0x%X' % (name, a, b))
        imgs[name].lid = lid
    a, b = COMMON[game]
    imgs['common'] = Image('common', CBASE, rom[a:b])
    return imgs


def objects(img):
    """(type, id) of every placed object of the level (record: +0 u32 type, +6 u16 id)"""
    out, a = [], OBJLIST[img.name]
    while img.w(a):
        r = img.w(a); out.append((img.w(r), img.h(r + 6))); a += 4
    return out


# ---- structure parsers: each returns (json, start, end) and has a packer back to bytes ----------------
def p_mesh(img, a):
    w = [img.w(a + 4 * i) for i in range(10)]
    j = dict(kind='mesh_header', ram=hx(a), off=a - img.base, flags=w[0] >> 16, field2=w[0] & 0xFFFF,
             nverts=w[1] >> 16, field6=w[1] & 0xFFFF, dl=hx(w[2]), vtx=hx(w[3]), rt_vtx_copy=hx(w[4]),
             rebuild_list=hx(w[5]), rt_vtx_end=hx(w[6]), tex_base=hx(w[7]), work_size=w[8], bbox=hx(w[9]))
    return j, a, a + 0x28

def k_mesh(j):
    I = lambda k: int(j[k], 16)
    return struct.pack('>10I', j['flags'] << 16 | j['field2'], j['nverts'] << 16 | j['field6'], I('dl'), I('vtx'),
                       I('rt_vtx_copy'), I('rebuild_list'), I('rt_vtx_end'), I('tex_base'), j['work_size'], I('bbox'))

def valid_mesh(img, a):
    if not img.has(a, 0x28) or a & 3: return False
    w = [img.w(a + 4 * i) for i in range(10)]
    if not (w[1] >> 16) or w[4] or w[6]: return False
    if not (img.has(w[2], 8) and img.has(w[3], 16)): return False
    for p in (w[5], w[9]):
        if p and not img.has(p, 2): return False
    return True

def p_bbox(img, a):
    ws = [img.w(a + 4 * i) for i in range(6)]
    return dict(kind='bbox', ram=hx(a), off=a - img.base, words=['%08X' % x for x in ws],
                min=[f32(x) for x in ws[:3]], max=[f32(x) for x in ws[3:]]), a, a + 24

def k_bbox(j): return b''.join(bytes.fromhex(x) for x in j['words'])

def p_rebuild(img, a, nverts):
    """u16 list: {n; m; idx[m]} ... ends at a 0 count (n or m) or when n+m reaches nverts (func_80000450)"""
    runs, p, tot, uniq, term = [], a, 0, 0, None
    while True:
        n = img.h(p); p += 2
        if n == 0: term = 'n'; break
        m = img.h(p); p += 2
        uniq += n; tot += n
        if m == 0: runs.append([n, []]); term = 'm'; break
        idx = [img.h(p + 2 * i) for i in range(m)]; p += 2 * m; tot += m
        runs.append([n, idx])
        if tot >= nverts and img.h(p) != 0: term = None; break   # (never happens: a 0 always follows)
    return dict(kind='rebuild_list', ram=hx(a), off=a - img.base, runs=runs, terminator=term,
                unique=uniq, total=tot), a, p

def k_rebuild(j):
    out = b''
    for i, (n, idx) in enumerate(j['runs']):
        last = i == len(j['runs']) - 1
        out += struct.pack('>H', n)
        if last and j['terminator'] == 'm':
            out += b'\0\0'
        else:
            out += struct.pack('>H%dH' % len(idx), len(idx), *idx)
    if j['terminator'] == 'n': out += b'\0\0'
    return out

def valid_anim(img, a):
    if not img.has(a, 16) or a & 3: return False
    he, do, ticks, nk, fl = img.h(a), img.h(a + 2), img.d[a + 4 - img.base], img.d[a + 5 - img.base], img.h(a + 6)
    if nk < 1 or (he & 0xFF) != 8 + nk or do < (he & 0xFF) or do > (he & 0xFF) + 15 or do & 7 or fl & 0x7FFF: return False
    keys = list(img.b(a + 8, nk))
    return keys[0] == 0 and all(x < y for x, y in zip(keys, keys[1:])) and keys[-1] <= ticks

def p_anim(img, a, nvert):
    he, do = img.h(a), img.h(a + 2)
    ticks, nk, fl = img.d[a + 4 - img.base], img.d[a + 5 - img.base], img.h(a + 6)
    n = nvert * nk
    words = struct.unpack_from('>%dI' % n, img.d, a - img.base + do)
    verts = []
    for i in range(nvert):
        row = []
        for k in range(nk):
            x = words[i * nk + k]
            sx = lambda v, b: v - (1 << b) if v >> (b - 1) else v
            row.append([sx(x & 0x7FF, 11), sx((x >> 11) & 0x7FF, 11), sx(x >> 22, 10)])
        verts.append(row)
    return dict(kind='morph_anim', ram=hx(a), off=a - img.base, hdr_end=he, data_off=do, ticks=ticks, nkey=nk,
                flags=fl, keys=list(img.b(a + 8, nk)), hdr_pad=img.b(a + 8 + nk, do - 8 - nk).hex(), nvert=nvert,
                verts=verts), a, a + do + 4 * n

def k_anim(j):
    out = struct.pack('>HHBBH', j['hdr_end'], j['data_off'], j['ticks'], j['nkey'], j['flags']) + bytes(j['keys'])
    out += bytes.fromhex(j['hdr_pad'])
    for row in j['verts']:
        for x, y, z in row:
            out += struct.pack('>I', (z & 0x3FF) << 22 | (y & 0x7FF) << 11 | (x & 0x7FF))
    return out

def p_quadhdr(img, a):
    return dict(kind='coll_quad_header', ram=hx(a), off=a - img.base, count=img.w(a), table=hx(img.w(a + 4))), a, a + 8

def k_quadhdr(j): return struct.pack('>II', j['count'], int(j['table'], 16))

def p_ptrs(img, a, n, kind):
    return dict(kind=kind, ram=hx(a), off=a - img.base, ptrs=[hx(img.w(a + 4 * i)) for i in range(n)]), a, a + 4 * n

def k_ptrs(j): return b''.join(struct.pack('>I', int(x, 16)) for x in j['ptrs'])

def p_quad(img, a):
    ws = [img.w(a + 4 * i) for i in range(20)]
    return dict(kind='coll_quad', ram=hx(a), off=a - img.base, words=['%08X' % x for x in ws],
                head=[f32(x) for x in ws[:8]], corners=[[f32(x) for x in ws[8 + 3 * i:11 + 3 * i]] for i in range(4)]), a, a + 0x50

def k_words(j): return b''.join(bytes.fromhex(x) for x in j['words'])

PACK = {'mesh_header': k_mesh, 'bbox': k_bbox, 'rebuild_list': k_rebuild, 'morph_anim': k_anim,
        'coll_quad_header': k_quadhdr, 'coll_quad_ptrs': k_ptrs, 'coll_quad': k_words}


class Collector:
    def __init__(s, img): s.img, s.items, s.refs = img, {}, {}
    def add(s, j, a, e, ref):
        key = (j['kind'], a)
        if key not in s.items:
            j['size'] = e - a; s.items[key] = j; s.refs[key] = []
        if ref not in s.refs[key]: s.refs[key].append(ref)
        return s.items[key]

    def model(s, a, ref, anims=(), quad=0):
        img = s.img
        if not valid_mesh(img, a): return None
        j, _, e = p_mesh(img, a); m = s.add(j, a, e, ref)
        uniq = m['nverts']
        if int(m['bbox'], 16):
            b = int(m['bbox'], 16); s.add(*p_bbox(img, b), ref='mesh %s +0x24' % hx(a))
        if int(m['rebuild_list'], 16):
            r = int(m['rebuild_list'], 16); rj = s.add(*p_rebuild(img, r, m['nverts']), ref='mesh %s +0x14' % hx(a))
            uniq = rj['unique']
        for an, aref in anims:
            if img.has(an, 8) and valid_anim(img, an) and img.has(an, img.h(an + 2) + 4 * uniq * img.d[an + 5 - img.base]):
                s.add(*p_anim(img, an, uniq), ref=aref + ' (mesh %s)' % hx(a))
            else:
                print('  ! anim %s (%s) not valid in %s' % (hx(an), aref, img.name))
        if quad:
            qj = s.add(*p_quadhdr(img, quad), ref=ref + ' quad block')
            t = int(qj['table'], 16)
            pj = s.add(*p_ptrs(img, t, qj['count'], 'coll_quad_ptrs'), ref='quad header %s +4' % hx(quad))
            for q in pj['ptrs']:
                s.add(*p_quad(img, int(q, 16)), ref='quad table %s' % hx(t))
        return m


def anim_lists(rom, game):
    """.data anim tables -> {name: [[anims of list 0], ...]}"""
    def dw(a): return u32(rom, a - ROMDATA)
    if game == 'letter':
        return {'D_80076B20': [[dw(dw(0x80076B20)), dw(dw(0x80076B20) + 4)]]}
    return {'D_800774A4': [[dw(dw(0x800774A4 + 4 * i)), dw(dw(0x800774A4 + 4 * i) + 4)] for i in range(2)]}


def build(rom, game):
    imgs = load_images(rom, game)
    F = FUNCS[game]
    cols = {n: Collector(i) for n, i in imgs.items()}
    objs = {n: objects(i) for n, i in imgs.items() if n != 'common'}
    unplaced = []

    def target(a, typ, oid):
        if a >= CBASE: return ['common']
        hits = [n for n, o in objs.items() if (typ, oid) in o]
        return hits

    tl = anim_lists(rom, game)
    groups = []
    for oid, (mesh, quad) in T7[game].items():
        groups.append((7, oid, mesh, [], quad, '%s case 0x%04X' % (F['t7'], oid)))
    for oid, (mesh, an) in T12[game].items():
        if isinstance(an, tuple):
            lst = [(x, '%s[%d][%d] (actor+0x1DB=%d, +0x1DA=%d)' % (an[0], an[1], k, an[1], k)) for k, x in enumerate(tl[an[0]][an[1]])]
        else:
            lst = [(x, '%s case 0x%04X actor+0x1BC' % (F['t12'], oid)) for x in an]
        groups.append((12, oid, mesh, lst, 0, '%s case 0x%04X' % (F['t12'], oid)))
    for oid, (mesh, an) in T11[game].items():
        groups.append((11, oid, mesh, [(x, '%s anim for 0x%04X' % (F['t11'], oid)) for x in an], 0,
                       '%s case 0x%04X' % (F['t11'], oid)))
    for k, (mesh, an) in E3E0[game].items():
        groups.append(('e3e0', k, mesh, [(x, '%s %s actor+0x1BC' % (F['e3e0'], k)) for x in an], 0, '%s %s' % (F['e3e0'], k)))
    for typ, oid, mesh, anims, quad, ref in groups:
        if typ == 'e3e0':
            tg = ['common']
        else:
            tg = target(mesh, typ, oid)
            if not tg:
                tg = [n for n in objs if valid_mesh(imgs[n], mesh)] if mesh < CBASE else ['common']
                unplaced.append('%s: id 0x%04X mesh %s has no placed type-%s object; valid as a mesh in %s'
                                % (ref, oid, hx(mesh), typ, tg))
        for n in tg:
            # anims and quads at level addresses live in the same level image; common ones in common
            an_l = [(x, r) for x, r in anims if (x >= CBASE) == (n == 'common')]
            an_c = [(x, r) for x, r in anims if x >= CBASE and n != 'common']
            if quad and (quad >= CBASE) != (n == 'common'):
                cols['common'].model(mesh, ref, quad=quad) if False else None
            m = cols[n].model(mesh, ref, an_l, quad if (quad >= CBASE) == (n == 'common') else 0)
            if m is None: print('  ! %s: %s is not a valid mesh in %s' % (ref, hx(mesh), n))
            if an_c or (quad and quad >= CBASE and n != 'common'):
                print('  ! cross-image companions for %s' % ref)
    # camera: level id -> mesh; anims from the .data table used by the camera update function
    camtab = [t for t in DATA_TABLES[game] if t[3] == F['camupd']][0]
    canims = [u32(rom, camtab[0] + 4 * i - ROMDATA) for i in range(camtab[1])]
    cammesh = {}
    for name, img in imgs.items():
        if name == 'common': continue
        mesh = CAM[game].get(img.lid, CAM[game][None])
        cammesh[name] = mesh
        ref = '%s level %d' % (F['cam'], img.lid)
        c = cols['common'] if mesh >= CBASE else cols[name]
        m = c.model(mesh, ref)
        if m is None: print('  ! camera mesh %s invalid for %s' % (hx(mesh), name)); continue
        uniq = m['nverts']
        if int(m['rebuild_list'], 16):
            uniq = c.items[('rebuild_list', int(m['rebuild_list'], 16))]['unique']
        for i, an in enumerate(canims):
            if not an: continue
            ai = imgs['common'] if an >= CBASE else img
            if not valid_anim(ai, an): continue
            if (an >= CBASE) != (mesh >= CBASE) and an >= CBASE: continue  # common anim, level mesh: other size
            if not ai.has(an, ai.h(an + 2) + 4 * uniq * ai.d[an + 5 - ai.base]):
                print('  ! %s D_%08X[%d] %s: anim would run past the end of %s with %d verts' % (F['camupd'], camtab[0], i, hx(an), name, uniq))
                continue
            j, a, e = p_anim(ai, an, uniq)
            cols[ai.name].add(j, a, e, '%s D_%08X[%d] (camera mesh %s, level %s)' % (F['camupd'], camtab[0], i, hx(mesh), name))
    # an anim may not run into the next structure: the only case is Number river 0x801AA088 (one vertex row short)
    for name, c in cols.items():
        st = sorted(c.items.values(), key=lambda j: j['off'])
        for j, nx in zip(st, st[1:]):
            if j['kind'] == 'morph_anim' and j['off'] + j['size'] > nx['off']:
                nv = (nx['off'] - j['off'] - j['data_off']) // (4 * j['nkey'])
                a = int(j['ram'], 16)
                k = ('morph_anim', a); refs = c.refs[k]
                nj, _, e = p_anim(c.img, a, nv); nj['size'] = e - a
                nj['note'] = 'nvert cut from %d (mesh) to %d: the next anim starts there' % (j['nvert'], nv)
                c.items[k] = nj
                unplaced.append('%s %s: %s' % (name, j['ram'], nj['note']))
    return imgs, cols, unplaced, cammesh


def data_tables(rom, game, imgs, cols):
    out = []
    for ram, n, lay, fn, tgt, what in DATA_TABLES[game]:
        words = [u32(rom, ram + 4 * i - ROMDATA) for i in range(n)]
        ents = []
        for w in words:
            hit = []
            for c in cols.values():
                if tgt not in ('level', 'level+common') and c.img.name != tgt: continue
                if tgt == 'level' and c.img.name == 'common': continue
                for (k, a), j in c.items.items():
                    if a == w: hit.append('%s:%s' % (c.img.name, k))
            ents.append(dict(word=hx(w), structures=hit))
        out.append(dict(ram=hx(ram), rom=hx(ram - ROMDATA), rom_end=hx(ram - ROMDATA + 4 * n), nwords=n, layout=lay,
                        used_by=fn, target=tgt, what=what, entries=ents))
    return out


def extract(rom, game, outdir):
    os.makedirs(outdir, exist_ok=True)
    imgs, cols, unplaced, cammesh = build(rom, game)
    mp = []
    for name, c in cols.items():
        items = sorted(c.items.values(), key=lambda j: j['off'])
        for j in items: j['referenced_by'] = c.refs[(j['kind'], int(j['ram'], 16))]
        fname = name + ('.bif' if name != 'common' else '')
        json.dump(dict(game=game, image=fname, base=hx(c.img.base), structures=items), open(os.path.join(outdir, name + '.json'), 'w'), indent=1)
        for j in items:
            if name == 'common': continue
            mp.append((game, '%s:0x%05X' % (fname, j['off']), '%s:0x%05X' % (fname, j['off'] + j['size']), j['kind'],
                       FMT[j['kind']], desc(j), '; '.join(j['referenced_by']), 'tools/extract_level_misc.py'))
    dt = data_tables(rom, game, imgs, cols)
    json.dump(dict(game=game, tables=dt, unplaced=unplaced, camera_mesh={k: hx(v) for k, v in cammesh.items()}),
              open(os.path.join(outdir, 'data_tables.json'), 'w'), indent=1)
    for t in dt:
        mp.append((game, t['rom'], t['rom_end'], 'data_ptr_table', 'u32 RAM pointers' + (' {TLUT, texels} pairs' if t['layout'] == 'pair' else ''),
                   'program .data RAM %s (%d words) -> %s: %s' % (t['ram'], t['nwords'], t['target'], t['what']), t['used_by'],
                   'tools/extract_level_misc.py (data_tables.json)'))
    with open(os.path.join(outdir, 'MAP.part.tsv'), 'w') as f:
        for r in mp: f.write('\t'.join(r) + '\n')
    for u in unplaced: print('  unplaced:', u)
    cnt = {}
    for name, c in cols.items():
        for (k, a), j in c.items.items():
            cnt.setdefault(name, {}).setdefault(k, [0, 0]); cnt[name][k][0] += 1; cnt[name][k][1] += j['size']
    print(json.dumps(cnt))
    print('%s: %d MAP rows' % (game, len(mp)))


FMT = {'mesh_header': '0x28: u16 flags,u16,u16 nverts,u16,ptr dl,vtx,0,rebuild,0,tex,u32 work,ptr bbox',
       'bbox': 'f32[6] min xyz, max xyz', 'rebuild_list': 'u16 runs {n, m, idx[m]}', 'morph_anim': 'sprA morph anim (u16,u16,u8,u8,u16,keys,pad,u32[nvert][nkey])',
       'coll_quad_header': 'u32 count, ptr table', 'coll_quad_ptrs': 'u32 ptr[count]', 'coll_quad': 'f32[20]: 8 f32 + 4 corners xyz'}

def desc(j):
    k = j['kind']
    if k == 'mesh_header': return 'mesh header: %d verts, flags 0x%04X, dl %s' % (j['nverts'], j['flags'], j['dl'])
    if k == 'morph_anim': return 'morph animation: %d keys, %d ticks, %d verts' % (j['nkey'], j['ticks'], j['nvert'])
    if k == 'rebuild_list': return 'animated-vertex rebuild list: %d unique / %d total' % (j['unique'], j['total'])
    if k == 'bbox': return 'mesh bounding box'
    if k == 'coll_quad_header': return 'actor collision quads: %d' % j['count']
    if k == 'coll_quad_ptrs': return 'pointers to the collision quads'
    return 'collision quad'


def check(rom, game, outdir):
    imgs = load_images(rom, game)
    bad = n = 0
    for name, img in imgs.items():
        p = os.path.join(outdir, name + '.json')
        for j in json.load(open(p))['structures']:
            b = PACK[j['kind']](j); n += 1
            if len(b) != j['size'] or b != img.d[j['off']:j['off'] + j['size']]:
                bad += 1; print('MISMATCH', name, j['kind'], j['ram'])
    for t in json.load(open(os.path.join(outdir, 'data_tables.json')))['tables']:
        b = b''.join(struct.pack('>I', int(e['word'], 16)) for e in t['entries']); n += 1
        s = int(t['rom'], 16)
        if b != rom[s:s + len(b)]: bad += 1; print('MISMATCH data table', t['ram'])
    print('%s: %d structures re-serialised, %d mismatches -> %s' % (game, n, bad, 'OK' if not bad else 'FAIL'))
    return bad == 0


if __name__ == '__main__':
    a = sys.argv[1:]
    if a and a[0] == 'check':
        sys.exit(0 if check(open(a[1], 'rb').read(), a[2], a[3]) else 1)
    if len(a) != 3: sys.exit(__doc__)
    extract(open(a[0], 'rb').read(), a[1], a[2])
