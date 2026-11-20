#!/usr/bin/env python3
"""Stale bytes in padding: explicit `leftover` rows with a generator rule.

usage: leftover_g2.py extract ROM GAME REBUILD_TSV OUTDIR -> OUTDIR/LEFTOVER_<game>.tsv
       leftover_g2.py check   ROM GAME REBUILD_TSV OUTDIR -> regenerate each row by its rule and compare

Finding: every non-zero padding byte that the rebuild could not generate sits in the tail pad between the end
of a file (a gzip member, a sample bank, a speech clip) and the next 16-byte boundary, and every such tail
(zero bytes included) equals the ROM bytes exactly 0x2000 (8 KiB) earlier. The mastering tool copied each file
through an 8 KiB buffer and rounded the last block up to 16 bytes, so the round-up carries whatever the
buffer held from the previous 8 KiB block of the same file. Rule `copy:-0x2000`: out[x] = rom[x - 0x2000];
the source bytes lie inside the same file's regenerated data. No ROM bytes are stored in the TSV.
"""
import sys, os

def rows(tsv):
    out = []
    for l in list(open(tsv))[1:]:
        f = l.rstrip('\n').split('\t')
        out.append((int(f[0], 16), int(f[1], 16), f[3], f[4], f[5], f[6]))
    return out

def runs(R):
    """merge each stale-pad exception row with its neighbouring pad rows up to the 16-byte boundary"""
    res = []
    for i, (a, b, own, kind, cls, src) in enumerate(R):
        if cls != 'pad' or src != 'exception': continue
        s = a; j = i
        while R[j - 1][4] == 'pad' and R[j - 1][2] == own: j -= 1; s = R[j][0]
        e = b; k = i
        while e % 16 and k + 1 < len(R) and R[k + 1][4] == 'pad' and R[k + 1][2] == own: k += 1; e = R[k][1]
        prev = R[j - 1]
        if res and res[-1][0] == s: continue
        res.append((s, e, own, '%s/%s 0x%X-0x%X' % (prev[2], prev[4], prev[0], prev[1]), prev[0]))
    return res

def main():
    mode, romp, game, tsv, od = sys.argv[1:6]
    rom = open(romp, 'rb').read(); os.makedirs(od, exist_ok=True)
    p = os.path.join(od, 'LEFTOVER_%s.tsv' % game)
    R = rows(tsv)
    if mode == 'extract':
        L = ['game\tstart\tend\tbytes\tnonzero\tclass\trule\tsource_range\tafter (owner/class range of the file it ends)\tnote']
        for s, e, own, prev, pstart in runs(R):
            seg = rom[s:e]; nz = sum(1 for x in seg if x)
            assert seg == rom[s - 0x2000:e - 0x2000], hex(s)
            L.append('%s\t0x%06X\t0x%06X\t%d\t%d\tleftover\tcopy:-0x2000\t0x%06X-0x%06X\t%s\t%s pad to 16; stale 8 KiB write-buffer bytes' %
                     (game, s, e, e - s, nz, s - 0x2000, e - 0x2000, prev, own))
        open(p, 'w').write('\n'.join(L) + '\n')
    bad = 0; n = 0; tot = 0; nzt = 0
    exc = {(a, b) for a, b, o, k, c, src in R if c == 'pad' and src == 'exception'}
    cover = set()
    for l in list(open(p))[1:]:
        f = l.rstrip('\n').split('\t'); s, e = int(f[1], 16), int(f[2], 16)
        assert f[6] == 'copy:-0x2000'
        gen = bytes(rom[x - 0x2000] for x in range(s, e))
        n += 1; tot += e - s; nzt += int(f[4])
        if gen != rom[s:e]: bad += 1; print('MISMATCH', f[1])
        for a, b in exc:
            if s <= a and b <= e: cover.add((a, b))
    print(game, 'leftover rows', n, 'bytes', tot, 'non-zero', nzt, 'exception rows covered %d/%d' % (len(cover), len(exc)),
          'mismatches', bad)
    sys.exit(1 if bad or len(cover) != len(exc) else 0)

if __name__ == '__main__':
    main()
