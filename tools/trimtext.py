#!/usr/bin/env python3
"""trimtext.py OBJECT SIZE: cut the assembler's end-of-section padding from a one-function object.
KMC GNU as rounds every .text to 16 bytes. The original translation units held several functions, so only
the last function of a unit was followed by padding; with one function per file the padding of the others
must go, or the linker could not place the next function at its address. Only zero bytes past SIZE with no
symbol or relocation in them are removed, and the section alignment is lowered to 4 (a function's own).
IDO without -non_shared marks an object PIC/CPIC (abicalls) even when, as here, its code has no GOT relocation;
GNU ld would then add call stubs. The two header flag bits are cleared (refused if a GOT/CALL relocation exists)."""
import sys, struct
path, size = sys.argv[1], int(sys.argv[2], 0)
b = bytearray(open(path, 'rb').read())
assert b[:4] == b'\x7fELF' and b[4] == 1 and b[5] == 2, 'ELF32 big-endian expected'
shoff, = struct.unpack_from('>I', b, 0x20); shentsize, shnum, shstrndx = struct.unpack_from('>HHH', b, 0x2E)
def sh(i): return list(struct.unpack_from('>IIIIIIIIII', b, shoff + i * shentsize))
strtab = sh(shstrndx)
def name(h): o = strtab[4] + h[0]; return b[o:b.index(b'\0', o)].decode()
for i in range(shnum):
    h = sh(i)
    if name(h) in ('.rodata', '.rdata', '.data', '.sdata', '.bss', '.lit4', '.lit8'):
        # KMC as aligns every section to 16; a unit's data starts at the address the program placed it
        h[8] = min(h[8], 4); struct.pack_into('>IIIIIIIIII', b, shoff + i * shentsize, *h); continue
    if name(h) != '.text': continue
    if h[5] > size:
        assert not any(b[h[4] + size:h[4] + h[5]]), 'non-zero bytes past the function end'
        h[5] = size
    h[8] = min(h[8], 4)
    struct.pack_into('>IIIIIIIIII', b, shoff + i * shentsize, *h)
flags, = struct.unpack_from('>I', b, 0x24)
if flags & 6:
    for i in range(shnum):
        h = sh(i)
        if h[1] in (9, 4):                                   # SHT_REL / SHT_RELA: no GOT16(9)/CALL16(11)/GPREL(7)
            ents = h[5] // h[9]
            for k in range(ents):
                info, = struct.unpack_from('>I', b, h[4] + k * h[9] + 4)
                assert info & 0xFF not in (7, 9, 11, 29, 30, 20, 21), 'PIC relocation present'
    struct.pack_into('>I', b, 0x24, flags & ~6)
open(path, 'wb').write(b)
