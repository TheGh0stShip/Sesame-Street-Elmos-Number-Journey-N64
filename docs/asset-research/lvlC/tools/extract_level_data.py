#!/usr/bin/env python3
"""Dump the non-graphics level structures of the Elmo .bif level images as JSON.

usage: extract_level_data.py ROM GAME OUTDIR          -> OUTDIR/<level>.json, OUTDIR/MAP.part.tsv
       extract_level_data.py check ROM GAME OUTDIR    -> re-serialise every JSON and compare to the bytes

The .bif files are gzip members at fixed ROM ranges (table below, from the level loader
Letter func_8001D9D0 / Number func_8001E2CC).  Each inflates to a RAM image loaded at 0x80154000
with absolute pointers; every pointer below is written both as RAM address and as file offset.
"""
import sys, os, json, struct, zlib

BASE = 0x80154000
LEVELS = {
    'letter': [('streetab', 0x1BF6F0, 0x204D00), ('chute', 0x204D00, 0x227BA0), ('manor', 0x227BA0, 0x243E20),
               ('orchard', 0x243E20, 0x2604B0), ('garden', 0x2604B0, 0x28FDB0), ('space', 0x28FDB0, 0x2ACFA0),
               ('planet', 0x2ACFA0, 0x2D1FB0), ('bonusabc', 0x2D1FB0, 0x2F9B60)],
    'number': [('street', 0x177680, 0x1BCE90), ('dunk', 0x1BCE90, 0x1D6CA0), ('bump', 0x1D6CA0, 0x206CC0),
               ('mansion', 0x206CC0, 0x22EC10), ('maze', 0x22EC10, 0x25AF80), ('ski', 0x25AF80, 0x27ADC0),
               ('river', 0x27ADC0, 0x2B5120), ('bonus123', 0x2B8FD0, 0x2E6FC0)],
}
OBJ_TYPES = {1: 'player_start (func_80021110, elmoact)', 4: 'world (func_80024E20, worldact: links the level BSP into the collision list)',
             5: 'camera (func_8002A120, cameraact)', 7: 'actor (func_80023118, muppetact; id selects model)',
             8: 'pickup (func_800327E0)', 9: 'effect emitter (func_80022C6C)', 10: 'func_80034CD0 object',
             12: 'map object (func_8003BB00, mapobjects; id selects BSP+collision shape)',
             13: 'speech trigger bank 0 (func_80026ABC)', 16: 'speech trigger bank 2 (func_80026B34)',
             17: 'speech trigger bank 1 (func_80026AF8)', 19: 'func_80030D8C object'}
OBJ_TYPES_NUMBER = {1: 'player_start (func_80021A20)', 4: 'world (func_80026110)', 5: 'camera (func_8002C800)',
             7: 'actor (func_80023A38)', 8: 'pickup (func_800347CC)', 9: 'effect emitter (func_80023594)',
             10: 'func_80036A90 object', 11: 'number object (func_800408D0; id 0x5001/0x5003/0x5005/0x6000 selects data)',
             12: 'map object (func_8003D3D0)', 13: 'speech trigger bank 0 (func_80027F2C)',
             14: 'speech trigger bank 1 (func_80027F68)', 15: 'speech trigger bank 2 (func_80027FA4)',
             18: 'type 18: dispatcher rewrites its id to 0x6000 at load (func_8001FBE0)', 19: 'func_80033368 object'}
TYPES = {'letter': OBJ_TYPES, 'number': OBJ_TYPES_NUMBER}

def inflate(rom, s, e):
    o = zlib.decompressobj(31)
    return o.decompress(rom[s:e])

class Img:
    def __init__(s, d): s.d = d; s.n = len(d)
    def ok(s, a, size=4): return BASE <= a and a - BASE + size <= s.n and (a & 3) == 0
    def u32(s, a): return struct.unpack_from('>I', s.d, a - BASE)[0]
    def ws(s, a, n): return list(struct.unpack_from('>%dI' % n, s.d, a - BASE))

def f32(u): return struct.unpack('>f', struct.pack('>I', u))[0]
def u_of(f): return struct.unpack('>I', struct.pack('>f', f))[0]
def off(a): return a - BASE if a else None

def walk_bsp(img, root, limit=20000):
    """returns (nodes, leaves) dicts addr->words or None if not a valid tree"""
    nodes, leaves, st = {}, {}, [root]
    while st:
        a = st.pop()
        if a in nodes or a in leaves: return None
        if not img.ok(a, 0x28): return None
        w = img.ws(a, 10)
        if w[0] & 0xFFFE: return None
        if w[0] & 1:
            w = w[:8]
            dl, vtx, nt, tr = w[4:8]
            if not (0 < nt < 4096 and img.ok(tr, 4 * nt) and img.ok(vtx) and (dl == 0 or img.ok(dl))): return None
            leaves[a] = w
        else:
            if not all(c == 0 or img.ok(c, 0x20) for c in w[8:10]) or (w[8] == 0 and w[9] == 0): return None
            nodes[a] = w
            st += [c for c in w[8:10] if c]
        if len(nodes) + len(leaves) > limit: return None
    return nodes, leaves

def find_shapes(img):
    out = []
    for o in range(0, img.n - 0x14, 4):
        a = BASE + o
        w = img.ws(a, 5)
        if w[0] == 12 and w[1] == 0 and w[3] == 0 and img.ok(w[4], 0x28):
            t = walk_bsp(img, w[4])
            if t and len(t[1]) > 0: out.append((a, w, t))
    return out

def find_object_list(img, OT):
    best = None
    for o in range(0, img.n - 8, 4):
        a = BASE + o

        ps = []
        p = a
        while img.ok(p) and img.u32(p) and img.ok(img.u32(p), 0x34):
            ps.append(img.u32(p)); p += 4
        if len(ps) < 3 or not img.ok(p) or img.u32(p) != 0: continue
        heads = {img.u32(x + 0x30) for x in ps}
        types = {img.u32(x) for x in ps}
        if len(heads) == 1 and types <= set(OT) and 4 in types:
            if best is None or len(ps) > len(best[1]): best = (a, ps)
    return best

def vec(w): return [f32(x) for x in w]

def extract(d, game, name):
    img = Img(d)
    res = {'game': game, 'file': name + '.bif', 'base': '0x%08X' % BASE, 'size': len(d), 'structures': []}
    S = res['structures']
    OT = TYPES[game]
    lst = find_object_list(img, OT)
    if not lst: raise SystemExit('no object list in ' + name)
    la, ps = lst
    world = img.u32(ps[0] + 0x30)
    S.append({'class': 'object_list', 'off': la - BASE, 'size': 4 * len(ps) + 4,
              'entries': [p - BASE for p in ps]})
    S.append({'class': 'world_list_head', 'off': world - BASE, 'size': 8, 'first': img.u32(world), 'last': img.u32(world + 4)})
    sp = sorted(ps)
    for i, p in enumerate(ps):
        w = img.ws(p, 13)
        rec = {'class': 'object', 'off': p - BASE, 'size': 0x34, 'type': w[0], 'type_name': OT.get(w[0], '?'),
               'id': '0x%08X' % w[1], 'param': w[2], 'pos': vec(w[3:6]), 'dir': vec(w[6:9]), 'up': vec(w[9:12]),
               'world': off(w[12])}
        if w[0] == 5:
            rec['size'] = 0x3C; rec['unused_tail'] = img.ws(p + 0x34, 2)
        S.append(rec)
    for a, w, (nodes, leaves) in find_shapes(img):
        item = a + 0x14
        it = img.ws(item, 3) if img.ok(item, 12) else None
        S.append({'class': 'coll_shape', 'off': a - BASE, 'size': 0x14, 'type': w[0], 'unk4': w[1], 'mask': '0x%08X' % w[2],
                  'unkC': w[3], 'root': off(w[4])})
        if it and it[2] == a:
            S.append({'class': 'coll_list_item', 'off': item - BASE, 'size': 12, 'next': it[0], 'prev': it[1], 'shape': off(it[2])})
        for n, nw in nodes.items():
            bb = struct.unpack('>6h', struct.pack('>3I', *nw[1:4]))
            S.append({'class': 'bsp_node', 'off': n - BASE, 'size': 0x28, 'index': nw[0] >> 16, 'flags': nw[0] & 0xFFFF,
                      'bbox': list(bb), 'plane': vec(nw[4:8]), 'front': off(nw[8]), 'back': off(nw[9]), 'tree': off(w[4])})
        for n, lw in leaves.items():
            bb = struct.unpack('>6h', struct.pack('>3I', *lw[1:4]))
            S.append({'class': 'bsp_leaf', 'off': n - BASE, 'size': 0x20, 'index': lw[0] >> 16, 'flags': lw[0] & 0xFFFF,
                      'bbox': list(bb), 'dl': off(lw[4]), 'vtx': off(lw[5]), 'ntri': lw[6], 'tris': off(lw[7]), 'tree': off(w[4])})
            tb = d[lw[7] - BASE: lw[7] - BASE + 4 * lw[6]]
            S.append({'class': 'coll_tris', 'off': lw[7] - BASE, 'size': 4 * lw[6],
                      'tris': [list(tb[i:i + 4]) for i in range(0, len(tb), 4)], 'leaf': n - BASE})
    # de-duplicate (a tree shared by two shapes)
    seen, out = set(), []
    for s in S:
        k = (s['class'], s['off'])
        if k not in seen: seen.add(k); out.append(s)
    out.sort(key=lambda s: s['off'])
    res['structures'] = out
    return res

def P(x): return BASE + x if x is not None else 0
def serialise(s):
    c = s['class']
    if c == 'object_list': return struct.pack('>%dI' % (len(s['entries']) + 1), *[BASE + e for e in s['entries']], 0)
    if c == 'world_list_head': return struct.pack('>2I', s['first'], s['last'])
    if c == 'object':
        b = struct.pack('>3I', s['type'], int(s['id'], 16), s['param']) + struct.pack('>9f', *(s['pos'] + s['dir'] + s['up'])) + struct.pack('>I', P(s['world']))
        if 'unused_tail' in s: b += struct.pack('>2I', *s['unused_tail'])
        return b
    if c == 'coll_shape': return struct.pack('>5I', s['type'], s['unk4'], int(s['mask'], 16), s['unkC'], P(s['root']))
    if c == 'coll_list_item': return struct.pack('>3I', s['next'], s['prev'], P(s['shape']))
    if c == 'bsp_node': return struct.pack('>I6h4f2I', s['index'] << 16 | s['flags'], *s['bbox'], *s['plane'], P(s['front']), P(s['back']))
    if c == 'bsp_leaf': return struct.pack('>I6h4I', s['index'] << 16 | s['flags'], *s['bbox'], P(s['dl']), P(s['vtx']), s['ntri'], P(s['tris']))
    if c == 'coll_tris': return bytes(sum(s['tris'], []))
    raise ValueError(c)

def main():
    check = sys.argv[1] == 'check'
    a = sys.argv[2:] if check else sys.argv[1:]
    rom, game, out = open(a[0], 'rb').read(), a[1], a[2]
    os.makedirs(out, exist_ok=True)
    bad = 0; mp = []
    for name, s, e in LEVELS[game]:
        d = inflate(rom, s, e)
        jp = os.path.join(out, name + '.json')
        if check:
            res = json.load(open(jp))
            n = 0
            for st in res['structures']:
                b = serialise(st)
                if b != d[st['off']:st['off'] + st['size']] or len(b) != st['size']:
                    bad += 1; print('MISMATCH', name, st['class'], hex(st['off']))
                n += 1
            print('%s: %d structures checked' % (name, n))
            continue
        res = extract(d, game, name)
        json.dump(res, open(jp, 'w'), indent=1)
        for st in res['structures']:
            mp.append('%s\t%s.bif:0x%05X\t%s.bif:0x%05X\t%s' % (game, name, st['off'], name, st['off'] + st['size'], st['class']))
        cnt = {}
        for st in res['structures']: cnt[st['class']] = cnt.get(st['class'], 0) + 1
        print(name, len(d), cnt)
    if check:
        print('FAIL' if bad else 'OK'); sys.exit(1 if bad else 0)
    open(os.path.join(out, 'MAP.part.tsv'), 'w').write('\n'.join(mp) + '\n')

if __name__ == '__main__':
    main()
