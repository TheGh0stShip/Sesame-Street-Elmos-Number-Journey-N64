"""dis.py GAME SPACE START END: print F3DEX disassembly of a range (lab inspection only; prints commands, not raw bytes)."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import evidence as E
G = E.Game(sys.argv[1]); sp = sys.argv[2]; d = G.data(sp)
a, b = int(sys.argv[3], 16), int(sys.argv[4], 16)
for o in range(a, b, 8):
    w0, w1 = struct.unpack_from('>II', d, o)
    print('%06X %s' % (o, E.f3dex.disasm(w0, w1) if (w0 >> 24) in E.f3dex.VALID else 'word %08X %08X' % (w0, w1)))
