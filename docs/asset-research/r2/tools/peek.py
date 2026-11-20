"""peek.py: lab inspection (console only, prints counts, never bytes): the slack after each gzip member.
For each lvlA gzip member: trailer ISIZE vs inflated size, slack length to the end of its files/ row, and how many
slack bytes are non-zero."""
import sys, os, re, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import evidence as E
S = E.S
for g in ('letter', 'number'):
    G = E.Game(g)
    files = []
    for l in open(S + '/files/FILES_%s.tsv' % g):
        c = l.split('\t')
        if c[0].isdigit(): files.append((int(c[1], 16), int(c[2], 16)))
    for l in open(S + '/lvlA/MAP.tsv'):
        c = l.rstrip('\n').split('\t')
        if c[0] != g or len(c) < 4 or c[3] != 'gzip_level': continue
        a, b = int(c[1], 16), int(c[2], 16)
        fend = [y for x, y in files if x == a][0]
        isize = struct.unpack_from('<I', G.rom, b - 4)[0]
        name = re.match(r'(\S+\.bif)', c[5]).group(1).lower()
        sl = G.rom[b:fend]
        print(g, name, 'member 0x%X-0x%X isize_ok=%s slack=%d nonzero=%d file_end_aligned16=%s' % (
            a, b, isize == len(G.img[name]), len(sl), sum(1 for x in sl if x), fend % 16 == 0))
