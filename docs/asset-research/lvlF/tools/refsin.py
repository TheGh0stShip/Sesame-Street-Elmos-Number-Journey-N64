import sys
B='${N64DK_LAB}/sesassets/'
L=B+'lvlF/out/uncovered.tsv'
rng=[l.split('\t') for l in open(L)]
refs={}
for g in ('letter','number'):
    refs[g]=[]
    for l in open(B+f'LEVEL_REFS_{g}.tsv'):
        p=l.rstrip().split('\t')
        if p[0].startswith('0x') and p[1].startswith('level_image'):
            refs[g].append((int(p[0],16)-0x80154000,p[3]))
for g,f,s,e,n in rng:
    s=int(s,16);e=int(e,16)
    if e-s<int(sys.argv[1]): continue
    inside=sorted({(o,r.split(':')[0][:13]) for o,r in refs[g] if s<=o<e})
    print(g,f,hex(s),hex(e),e-s,len(inside))
    agg={}
    for o,r in inside: agg.setdefault(r,[]).append(o)
    for r,os_ in agg.items(): print('   ',r,len(os_),hex(min(os_)),hex(max(os_)))
