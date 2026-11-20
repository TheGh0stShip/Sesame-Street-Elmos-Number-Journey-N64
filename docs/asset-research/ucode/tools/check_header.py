#!/usr/bin/env python3
"""check_header.py ROM : describe the N64 header, identify the IPL3/CIC variant, recompute CRC1/CRC2."""
import sys, struct, zlib
M = 0xFFFFFFFF
# crc32 of IPL3 (ROM 0x40-0x1000) -> (cic, checksum seed); public constants
CIC = {0x6170A4A1: ('6101', 0xF8CA4DDC), 0x90BB6CB5: ('6102/7101', 0xF8CA4DDC),
       0x0B050EE0: ('6103/7103', 0xA3886759), 0x98BC2C86: ('6105/7105', 0xDF26F436),
       0xACC8580A: ('6106/7106', 0x1FEA617A), 0x009E9EA3: ('7102', 0xF8CA4DDC)}
def rol(v, n):
    n &= 31
    return ((v << n) | (v >> (32 - n))) & M if n else v
def crc(rom, cic, seed):
    t1 = t2 = t3 = t4 = t5 = t6 = seed
    for i in range(0x1000, 0x101000, 4):
        d = struct.unpack_from('>I', rom, i)[0]
        if (t6 + d) & M < t6: t4 = (t4 + 1) & M
        t6 = (t6 + d) & M
        t3 ^= d
        r = rol(d, d & 0x1F)
        t5 = (t5 + r) & M
        t2 = t2 ^ r if t2 > d else t2 ^ t6 ^ d
        if cic.startswith('6105'):
            t1 = (t1 + (struct.unpack_from('>I', rom, 0x0750 + (i & 0xFF))[0] ^ d)) & M
        else:
            t1 = (t1 + (t5 ^ d)) & M
    if cic.startswith('6103'): return (t6 ^ t4) + t3 & M, (t5 ^ t2) + t1 & M
    if cic.startswith('6106'): return (t6 * t4 + t3) & M, (t5 * t2 + t1) & M
    return t6 ^ t4 ^ t3, t5 ^ t2 ^ t1
def main(p):
    rom = open(p, 'rb').read()
    if rom[:4] != b'\x80\x37\x12\x40': sys.exit('not a big-endian .z64 image')
    pi, clk, entry, rel, c1, c2 = struct.unpack_from('>IIIIII', rom, 0)
    print(f'0x00 PI BSD DOM1 config     : 0x{pi:08X}')
    print(f'0x04 clock rate override    : 0x{clk:08X} (0 = default)')
    print(f'0x08 boot entry point (RAM) : 0x{entry:08X}')
    print(f'0x0C libultra release word  : 0x{rel:08X} (low byte {chr(rel & 0xFF)!r} -> SDK 2.0{chr(rel & 0xFF)})')
    print(f'0x10 CRC1                   : 0x{c1:08X}')
    print(f'0x14 CRC2                   : 0x{c2:08X}')
    print(f'0x18 reserved (8 bytes)     : {"zero" if not any(rom[0x18:0x20]) else "nonzero"}')
    print(f'0x20 internal name (20 ASCII): {rom[0x20:0x34].decode("ascii", "replace").rstrip()!r}')
    print(f'0x34 reserved (7 bytes)     : {"zero" if not any(rom[0x34:0x3B]) else rom[0x34:0x3B].hex()}')
    print(f'0x3B media category         : {chr(rom[0x3B])!r} (N = cartridge)')
    print(f'0x3C game id (2 ASCII)      : {rom[0x3C:0x3E].decode()!r}')
    print(f'0x3E region / destination   : {chr(rom[0x3E])!r} (E = North America)')
    print(f'0x3F revision               : {rom[0x3F]}')
    ipl = zlib.crc32(rom[0x40:0x1000])
    cic, seed = CIC.get(ipl, ('unknown', 0xF8CA4DDC))
    print(f'IPL3 0x40-0x1000 crc32      : 0x{ipl:08X} -> CIC-NUS-{cic}')
    r1, r2 = crc(rom, cic, seed)
    ok = (r1, r2) == (c1, c2)
    print(f'recomputed CRC1/CRC2        : 0x{r1:08X} 0x{r2:08X} -> {"MATCH" if ok else "MISMATCH"}')
    return 0 if ok else 1
if __name__ == '__main__':
    sys.exit(main(sys.argv[1]))
