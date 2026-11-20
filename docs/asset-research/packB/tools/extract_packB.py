#!/usr/bin/env python3
"""extract_packB.py ROM GAME OUTDIR [check] [map]
Mode-21 menu screen file ("menu21"): five 320x240 CI8 screens (0x200-byte RGBA16 TLUT + 76800-byte
image, 0x12E00 per slot) plus one unreferenced 320x240 CI4 screen (0x20-byte TLUT + 0x9600 image).
Writes one PNG (mode P, palette RGB from RGBA5551, alpha bit kept in tRNS) per screen; 'check' re-encodes
every PNG back to TLUT+image bytes and compares with the ROM; 'map' prints the MAP.tsv lines
(the whole menu21 file). Pure Python (zlib only): PNG is written and read by the minimal codec below."""
import sys, os
import zlib, struct

def png_write(path, idx, pal, trns):
    def chunk(t, d): return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d))
    raw = b''.join(b'\0' + idx[y*W:(y+1)*W] for y in range(H))
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', W, H, 8, 3, 0, 0, 0))
        + chunk(b'PLTE', bytes(pal)) + chunk(b'tRNS', bytes(trns)) + chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))

def png_read(path):
    """reads the 8-bit palette PNGs written above (filter type 0 rows only)"""
    d = open(path, 'rb').read(); o = 8; pal = trns = b''; idat = b''
    while o < len(d):
        n, t = struct.unpack('>I4s', d[o:o+8]); c = d[o+8:o+8+n]; o += 12 + n
        if t == b'PLTE': pal = c
        elif t == b'tRNS': trns = c
        elif t == b'IDAT': idat += c
    raw = zlib.decompress(idat); assert all(raw[y*(W+1)] == 0 for y in range(H))
    return b''.join(raw[y*(W+1)+1:(y+1)*(W+1)] for y in range(H)), pal, trns

FILES = {'letter': 0x0A54D0, 'number': 0x0A5940}   # menu21 ROM start (both 0x67C20 bytes)
SIZE = 0x67C20
# territory: the whole menu21 file
# (file offset, RAM address, bpp, palette entries, description)
SCREENS = [
    (0x00000, 0x80154000, 8, 256, ''),
    (0x12E00, 0x80166E00, 8, 256, ''),
    (0x25C00, 0x80179C00, 8, 256, ''),
    (0x38A00, 0x8018CA00, 8, 256, ''),
    (0x4B800, 0x8019F800, 4, 16,  'unreferenced CI4 320x240 screen (licence text; no code draws RAM 0x8019F800)'),
    (0x54E20, 0x801A8E20, 8, 256, ''),
]
# which slot the mode-21 draw switch shows for each value of the screen-state variable
STATE = {'letter': ('D_800745E4', 'func_8000B470 -> func_8001CAA8', {0: '4 and default (menu text drawn over it)', 1: '1', 2: '2', 3: '0', 5: '3'}),
         'number': ('D_80075094', 'func_8000BA3C -> func_8001D3AC', {0: '1', 1: '2', 2: '0', 3: '4 and default (menu text drawn over it)', 5: '3'})}
W, H = 320, 240

def c5(v): return (v << 3) | (v >> 2)

def decode(blob, bpp, n):
    pal_raw = blob[:n*2]; img = blob[n*2:]
    pal = []; trns = []
    for i in range(n):
        v = int.from_bytes(pal_raw[i*2:i*2+2], 'big')
        pal += [c5(v >> 11 & 31), c5(v >> 6 & 31), c5(v >> 1 & 31)]; trns.append(255 if v & 1 else 0)
    if bpp == 8: idx = bytes(img[:W*H])
    else: idx = bytes(x for b in img[:W*H//2] for x in (b >> 4, b & 15))
    return idx, pal, trns

def encode(png, bpp, n):
    px, pal, trns = png
    out = bytearray()
    for i in range(n):
        r, g, b = pal[i*3:i*3+3]
        out += ((r >> 3) << 11 | (g >> 3) << 6 | (b >> 3) << 1 | (1 if trns[i] else 0)).to_bytes(2, 'big')
    if bpp == 8: out += px
    else: out += bytes((px[i] << 4) | px[i+1] for i in range(0, len(px), 2))
    return bytes(out)

def main():
    rom, game, outdir = sys.argv[1:4]; mode = sys.argv[4] if len(sys.argv) > 4 else 'extract'
    base = FILES[game]; data = open(rom, 'rb').read()[base:base+SIZE]
    os.makedirs(outdir, exist_ok=True); bad = 0
    for k, (off, ram, bpp, n, desc) in enumerate(SCREENS):
        ln = n*2 + W*H*bpp//8; blob = data[off:off+ln]
        p = os.path.join(outdir, f'{game}_menu21_{k}_{off:05X}_ci{bpp}.png')
        if mode == 'extract':
            png_write(p, *decode(blob, bpp, n))
        elif mode == 'check':
            ok = encode(png_read(p), bpp, n) == blob; bad += not ok
            print('OK ' if ok else 'BAD', p)
    covered = sum(n*2 + W*H*bpp//8 for _, _, bpp, n, _ in SCREENS)
    assert covered == SIZE, hex(covered)
    if mode == 'map':
        ext = 'packB/tools/extract_packB.py'
        var, fn, st = STATE[game]
        for k, (off, ram, bpp, n, desc) in enumerate(SCREENS):
            if bpp == 8: desc = f'CI8 320x240 screen shown when {var} == {st[k]}'
            for (a, b, cls, fmt, what) in ((off, off+n*2, 'palette', 'TLUT RGBA16 x%d' % n, 'palette'),
                                          (off+n*2, off+n*2+W*H*bpp//8, 'texture', 'CI%d 320x240' % bpp, 'image')):
                a2, b2 = base+a, base+b
                ref = fn + '(img, tlut) in game mode 21' if bpp == 8 else 'none (unused)'
                print(f'{game}\t0x{a2:06X}\t0x{b2:06X}\t{cls}\t{fmt}\tmenu21 slot {k} {what}: {desc}, RAM 0x{ram+n*2*(what=="image"):08X}\t{ref}\t{ext}')
    sys.exit(1 if bad else 0)

if __name__ == '__main__': main()
