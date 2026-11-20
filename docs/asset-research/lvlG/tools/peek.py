import sys
sys.path.insert(0,'${N64DK_LAB}/sesassets/lvlG/tools')
from gaps import *
cov=ranges()
for f in FILES:
    d=open(B+'lvlA/out/number/%s.bif.bin'%f,'rb').read()
    g,s=gaps(f,cov)
    for a,b in g:
        if not any(d[a:b]): continue
        if b-a>0x2000: continue
        print(f,'%05X %05X %4X'%(a,b,b-a),d[a:min(b,a+48)].hex(' ',4))
