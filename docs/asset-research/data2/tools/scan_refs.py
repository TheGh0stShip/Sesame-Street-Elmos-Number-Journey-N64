#!/usr/bin/env python3
"""scan_refs.py ROM GAME VERIFY.tsv OUT.tsv

Disassembles every function listed in VERIFY.tsv (start, size) from the ROM and records each
%hi/%lo reference into the main segment's .data/.rodata (RAM 0x8006C000..0x8007C000), with the
access kind (addr = address taken, lb/lh/lw/lwc1/ldc1/sb/... = load/store width, jtbl = jump-table
load followed by jr, with its case count from the guarding sltiu). Also records every aligned word
inside .data/.rodata whose value points into .rodata (pointer tables). Output columns:
ram  kind  func  insn_ram  extra
No ROM content is written: only addresses and instruction kinds.
"""
import sys, struct, rabbitizer as R

ROM2RAM = 0x7FFFF400
LAYOUT = {  # .data start, .rodata start, .rodata end (RAM)
    'letter': (0x8006C9AC, 0x800785B0, 0x8007BAB0),
    'number': (0x8006E31C, 0x80078DC0, 0x8007BF20),
}
I = R.InstrId
R.config.pseudos_enablePseudos = False

def main():
    rom, game, verify, out = sys.argv[1:5]
    data = open(rom, 'rb').read()
    d0, r0, r1 = LAYOUT[game]
    funcs = []
    for ln in open(verify):
        f = ln.split('\t')
        funcs.append((int(f[0], 16), int(f[1])))
    funcs.sort()
    rows = []
    def word(ram):
        o = ram - ROM2RAM
        return struct.unpack('>I', data[o:o+4])[0]
    for fs, fsz in funcs:
        hi = {}      # reg -> (value, indexed)
        ins = [R.Instruction(word(a), vram=a) for a in range(fs, fs + fsz, 4)]
        for k, it in enumerate(ins):
            u = it.uniqueId
            if u == I.cpu_lui:
                hi[it.rt.value] = (it.getProcessedImmediate() << 16, False)
                continue
            if u in (I.cpu_addu, I.cpu_daddu) and it.rd.value in (it.rs.value, it.rt.value) \
                    and it.rd.value in hi and it.rs.value != it.rt.value:
                v, _ = hi[it.rd.value]
                hi[it.rd.value] = (v, True)
                continue
            if it.isIType() and it.rs.value in hi and u != I.cpu_lui:
                base, indexed = hi[it.rs.value]
                imm = it.getProcessedImmediate()
                if u == I.cpu_ori:
                    addr = base | (imm & 0xFFFF)
                else:
                    addr = (base + imm) & 0xFFFFFFFF
                if u in (I.cpu_addiu, I.cpu_ori, I.cpu_daddiu):
                    kind = 'addr'
                elif it.doesLoad() or it.doesStore():
                    kind = it.getOpcodeName()
                    if indexed:
                        kind += '[]'
                    if u == I.cpu_lw and indexed:
                        # jump table?  lw rX ; (maybe nop) ; jr rX
                        rt = it.rt.value
                        for j in range(k + 1, min(k + 4, len(ins))):
                            if ins[j].uniqueId == I.cpu_jr and ins[j].rs.value == rt:
                                kind = 'jtbl'
                                break
                else:
                    kind = None
                if kind and d0 - 0x2000 <= addr < r1:
                    extra = ''
                    if kind == 'jtbl':
                        for j in range(k - 1, max(k - 16, -1), -1):
                            if ins[j].uniqueId == I.cpu_sltiu:
                                extra = 'n=%d' % ins[j].getProcessedImmediate()
                                break
                    rows.append((addr, kind, 'func_%08X' % fs, it.vram, extra))
            # register clobbering
            if it.uniqueId != I.cpu_lui:
                dst = None
                if it.modifiesRt():
                    dst = it.rt.value
                elif it.modifiesRd():
                    dst = it.rd.value
                if dst is not None and dst in hi and not (it.doesStore()):
                    if not (it.isIType() and u in (I.cpu_addiu,) and it.rs.value == dst and False):
                        del hi[dst]
            if it.isJump() and not it.isFunctionCall() and it.uniqueId != I.cpu_jal:
                pass
    # pointer words in .data/.rodata pointing into .rodata
    for a in range(d0 & ~3, r1, 4):
        v = word(a)
        if r0 <= v < r1:
            rows.append((v, 'ptr', 'data_%08X' % a, a, ''))
    rows.sort()
    with open(out, 'w') as f:
        for r in rows:
            f.write('%08X\t%s\t%s\t%08X\t%s\n' % r)
    print(game, len(rows), 'refs')

main()
