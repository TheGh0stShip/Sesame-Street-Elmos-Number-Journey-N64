import sys,struct
sys.path.insert(0,'${N64DK_LAB}/sesassets/lvlG/tools')
from gaps import *
cov=ranges()
R=[l.split('\t') for l in open(B+'LEVEL_REFS_number.tsv').read().splitlines()[1:]]
for f in FILES:
    d=open(B+'lvlA/out/number/%s.bif.bin'%f,'rb').read()
    ptr={}
    for o in range(0,len(d)-3,4):
        w=struct.unpack('>I',d[o:o+4])[0]
        if 0x80154000<=w<0x80154000+len(d): ptr.setdefault(w-0x80154000,[]).append(o)
    g,s=gaps(f,cov)
    for a,b in g:
        if not any(d[a:b]) and b-a<0x100: continue
        rr=[(r[0],r[3]) for r in R if r[1].startswith('level') and a<=int(r[0],16)-0x80154000<b]
        pp=[(t,ptr[t]) for t in sorted(ptr) if a<=t<b]
        print(f,'%05X %05X'%(a,b),' '.join('%s:%s'%x for x in rr),' | ',' '.join('%X<-%s'%(t,','.join('%X'%q for q in p[:4])) for t,p in pp[:12]))
