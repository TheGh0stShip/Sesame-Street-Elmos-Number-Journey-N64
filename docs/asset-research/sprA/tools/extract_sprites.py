#!/usr/bin/env python3
"""Extract the 'sprite_anim' files of Elmo's Letter Adventure / Number Journey.

They are NOT sprites: each file is a vertex (morph-target) animation for one character
mesh, decoded by func_8000092C -> func_80000450 (Letter addresses). See FORMAT.md.

usage: extract_sprites.py ROM GAME OUTDIR        write JSON + OBJ per file
       extract_sprites.py ROM GAME OUTDIR check  repack from the decoded form and compare
       extract_sprites.py ROM GAME OUTDIR map    print MAP.tsv rows
GAME = letter | number. File list comes from FILES_<game>.tsv (class sprite_anim).
"""
import json, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
FILES = os.path.join(HERE, '..', '..', 'FILES_%s.tsv')

def files(game):
    out = []
    for line in open(FILES % game):
        f = line.rstrip('\n').split('\t')
        if len(f) > 4 and f[4] == 'sprite_anim':
            out.append((int(f[1], 16), int(f[2], 16)))
    return out

def is_hdr(d, o):
    he, off = struct.unpack_from('>HH', d, o); k = d[o + 5]
    return k > 1 and he == 8 + k and off == he + (-he) % 8 and d[o + 8] == 0 and all(d[o+8+i] < d[o+9+i] for i in range(k - 1)) and d[o + 7 + k] <= d[o + 4]

def split(d, s, e):
    # a FILES range may hold further unreferenced animations packed after the first one
    cuts = [s] + [o for o in range(s + 16, e, 16) if is_hdr(d, o)] + [e]
    return list(zip(cuts, cuts[1:]))

def sx(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v

def decode(b):
    hdr_end, off = struct.unpack_from('>HH', b, 0)
    ticks, nkey = b[4], b[5]
    flags = struct.unpack_from('>H', b, 6)[0]
    keys = list(b[8:8 + nkey])
    stride = 4 * nkey
    nvert = (len(b) - off) // stride
    tail = b[off + nvert * stride:]
    verts = []  # verts[k][i] = (x,y,z)
    for k in range(nkey):
        row = []
        for i in range(nvert):
            w = struct.unpack_from('>I', b, off + (i * nkey + k) * 4)[0]
            row.append((sx(w & 0x7FF, 11), sx((w >> 11) & 0x7FF, 11), sx(w >> 22, 10)))
        verts.append(row)
    return dict(hdr_end=hdr_end, data_off=off, ticks=ticks, nkey=nkey, flags=flags,
                keys=keys, nvert=nvert, pad=b[8 + nkey:off].hex(), tail=tail.hex(), verts=verts)

def encode(d):
    b = bytearray(struct.pack('>HHBBH', d['hdr_end'], d['data_off'], d['ticks'], d['nkey'], d['flags']))
    b += bytes(d['keys']) + bytes.fromhex(d['pad'])
    assert len(b) == d['data_off']
    for i in range(d['nvert']):
        for k in range(d['nkey']):
            x, y, z = d['verts'][k][i]
            b += struct.pack('>I', ((z & 0x3FF) << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF))
    return bytes(b + bytes.fromhex(d['tail']))

def main():
    rom, game, outdir = sys.argv[1:4]
    mode = sys.argv[4] if len(sys.argv) > 4 else 'extract'
    data = open(rom, 'rb').read()
    bad = 0; tot = dict(files=0, keys=0, verts=0, bytes=0)
    known = [x[0] for x in files(game)]
    for s, e in [p for a, z in files(game) for p in split(data, a, z)]:
        b = data[s:e]; d = decode(b)
        tot['files'] += 1; tot['keys'] += d['nkey']; tot['verts'] += d['nvert']; tot['bytes'] += len(b)
        if mode == 'check':
            ok = encode(json.loads(json.dumps(d))) == b
            bad += not ok
            print('%s 0x%06X %s nkey=%d nvert=%d ticks=%d flags=%04X hdr_end=%d off=%d pad=%s tail=%dB' % (
                game, s, 'OK' if ok else 'MISMATCH', d['nkey'], d['nvert'], d['ticks'], d['flags'],
                d['hdr_end'], d['data_off'], d['pad'] or '-', len(d['tail']) // 2))
        elif mode == 'map':
            t = len(d['tail']) // 2
            ref = ('Letter func_800272C8/func_80026B70 (Number func_800287AC/func_80027FE0) set D_80106874/D_800A75E0; func_80026920 DMAs it; func_8000092C/func_80000450 decode'
                   if s in known else 'UNREFERENCED: packed after the previous animation, no code symbol')
            _ = '/func_80000450 via actor anim ptr (loaded by streaming loader)'
            rows = [(s, s + 8, 'sprite_anim', 'vanim header', 'u16 hdr_end,u16 data_off,u8 ticks,u8 nkey,u16 flags (loop=0x8000)'),
                    (s + 8, s + 8 + d['nkey'], 'sprite_anim', 'u8[nkey] key ticks', 'keyframe tick numbers (%d keys, %d ticks)' % (d['nkey'], d['ticks']))]
            if d['data_off'] > 8 + d['nkey']:
                rows.append((s + 8 + d['nkey'], s + d['data_off'], 'pad', 'zero', 'align vertex data to 8'))
            rows.append((s + d['data_off'], e - t, 'sprite_anim', 'packed z:s10 y:s11 x:s11 words (MSB first)',
                         '%d vertices x %d keyframes, vertex-major' % (d['nvert'], d['nkey'])))
            if t:
                rows.append((e - t, e, 'pad', 'stale', 'file padding to 16: 1-3 non-zero words left over from the packer buffer (vertex-like values, never read: less than one vertex row)'))
            for a, z, c, f, desc in rows:
                print('\t'.join([game, '0x%06X' % a, '0x%06X' % z, c, f, desc, ref, 'tools/extract_sprites.py']))
        else:
            od = os.path.join(outdir, game); os.makedirs(od, exist_ok=True)
            json.dump(d, open(os.path.join(od, '%06X.json' % s), 'w'))
            for k in range(d['nkey']):
                with open(os.path.join(od, '%06X_k%02d_t%03d.obj' % (s, k, d['keys'][k])), 'w') as f:
                    for x, y, z in d['verts'][k]:
                        f.write('v %d %d %d\n' % (x, y, z))
    if mode != 'map':
        print(game, tot, 'mismatches=%d' % bad if mode == 'check' else '')
    sys.exit(1 if bad else 0)

main()
