import sys,os
B='${N64DK_LAB}/sesassets/'
FILES=['street','dunk','bump','mansion']
def ranges():
    cov={f:[] for f in FILES}
    for src,skip in (('lvlB/MAP.tsv',True),('lvlC/MAP.tsv',False)):
        for l in open(B+src):
            p=l.rstrip('\n').split('\t')
            if p[0]!='number' or len(p)<4: continue
            if skip and p[3]=='level_data': continue
            f,a=p[1].split(':'); _,b=p[2].split(':'); f=f.replace('.bif','')
            if f in cov: cov[f].append((int(a,16),int(b,16)))
    return cov
def gaps(f,cov):
    size=os.path.getsize(B+'lvlA/out/number/%s.bif.bin'%f)
    r=sorted(cov[f]);out=[];pos=0
    for a,b in r:
        if a>pos: out.append((pos,a))
        pos=max(pos,b)
    if pos<size: out.append((pos,size))
    return out,size
if __name__=='__main__':
    cov=ranges()
    for f in FILES:
        g,s=gaps(f,cov); t=sum(b-a for a,b in g)
        print(f,hex(s),t,'%.1f%%'%(100*t/s),len(g))
        for a,b in g:
            if b-a>=int(sys.argv[1] if len(sys.argv)>1 else '0x400',16): print('  %05X %05X %X'%(a,b,b-a))
