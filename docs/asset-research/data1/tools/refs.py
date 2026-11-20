#!/usr/bin/env python3
"""refs.py ROM GAME VERIFY.tsv LO HI OUT.tsv
Disassemble every matched function (VERIFY.tsv: start, size) and record each %hi/%lo reference
into RAM [LO,HI) with its access kind (addr / lw / lh / lbu / lwc1 / ldc1 / sw ..., '[]' = indexed),
plus aligned words anywhere in the program image (.text end .. .rodata end) whose value points into
[LO,HI) (kind ptr). Output: ram kind func_or_data insn_ram. Addresses only, no ROM content."""
import sys, struct, rabbitizer as R
OFF = 0x7FFFF400
I = R.InstrId
rom, game, verify, lo, hi, out = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4],16), int(sys.argv[5],16), sys.argv[6]
IMG = {'letter': (0x8006C9AC, 0x8007BAB0), 'number': (0x8006E31C, 0x8007BF20)}[game]
B = open(rom,'rb').read()
w = lambda a: struct.unpack('>I', B[a-OFF:a-OFF+4])[0]
rows = []
for ln in open(verify):
    f = ln.split('\t'); fs, fsz = int(f[0],16), int(f[1])
    ins = [R.Instruction(w(a), vram=a) for a in range(fs, fs+fsz, 4)]
    regs = {}
    for it in ins:
        u = it.uniqueId
        if u == I.cpu_lui:
            regs[it.rt.value] = [it.getProcessedImmediate() << 16, False]; continue
        if u in (I.cpu_addu, I.cpu_daddu) and it.rd.value in regs and it.rd.value in (it.rs.value, it.rt.value) and it.rs.value != it.rt.value:
            regs[it.rd.value][1] = True; continue
        try: rs = it.rs.value
        except RuntimeError: rs = None
        if it.isIType() and rs in regs:
            base, idx = regs[rs]; imm = it.getProcessedImmediate()
            a = (base | (imm & 0xFFFF)) if u == I.cpu_ori else (base + imm) & 0xFFFFFFFF
            kind = 'addr' if u in (I.cpu_addiu, I.cpu_ori, I.cpu_daddiu) else (it.getOpcodeName() + ('[]' if idx else '') if (it.doesLoad() or it.doesStore()) else None)
            if kind and lo <= a < hi: rows.append((a, kind, 'func_%08X' % fs, it.vram))
        if not it.doesStore():
            try: d = it.rt.value if it.modifiesRt() else (it.rd.value if it.modifiesRd() else None)
            except RuntimeError: d = None
            if d in regs: del regs[d]
for a in range(IMG[0] & ~3, IMG[1], 4):
    v = w(a)
    if lo <= v < hi: rows.append((v, 'ptr', 'data_%08X' % a, a))
rows.sort()
with open(out,'w') as f:
    for r in rows: f.write('%08X\t%s\t%s\t%08X\n' % r)
print(game, len(rows))
