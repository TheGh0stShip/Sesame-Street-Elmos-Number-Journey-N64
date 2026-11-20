import sys,struct
sys.path.insert(0,'${N64DK_LAB}/sesassets/lvlG/tools')
from gaps import *
cov=ranges()
def hdr(d,o):
    if o+8>len(d): return None
    he,do,ti,nk,fl=struct.unpack('>HHBBH',d[o:o+8])
    if nk<2 or he!=8+nk or do!=(he+7)//8*8 or fl&0x7fff: return None
    k=d[o+8:o+8+nk]
    if k[0]!=0 or any(k[i]>=k[i+1] for i in range(nk-1)) or k[-1]>ti: return None
    if any(d[o+he:o+do]): return None
    return nk,ti,fl,do
for f in FILES:
    d=open(B+'lvlA/out/number/%s.bif.bin'%f,'rb').read()
    g,s=gaps(f,cov)
    print('==',f)
    for a,b in g:
        hits=[(o,hdr(d,o)) for o in range(a,b,4) if hdr(d,o)]
        print('  %05X %05X %5X'%(a,b,b-a),' '.join('%05X:nk%d,t%d,f%x'%(o,h[0],h[1],h[2]) for o,h in hits))
