import sys,os,collections
B='${N64DK_LAB}/sesassets/'
FILES={'letter':['streetab','chute','manor','orchard'],'number':['street','dunk','bump','mansion']}  # inflated copies read from ../lvlA/out (coverage survey only)
def load():
    cov=collections.defaultdict(list)
    for src in ('lvlB','lvlC'):
        for l in open(B+src+'/MAP.tsv'):
            f=l.rstrip('\n').split('\t')
            if f[0]=='game' or len(f)<4: continue
            if src=='lvlB' and f[3]=='level_data': continue
            fn,s=f[1].split(':'); e=int(f[2].split(':')[1],16)
            cov[(f[0],fn)].append((int(s,16),e,src+':'+f[3]))
    return cov
def gaps(game,name,cov):
    size=os.path.getsize(B+'lvlA/out/%s/%s.bif.bin'%(game,name))
    r=sorted(cov[(game,name+'.bif')]);out=[];p=0
    for s,e,c in r:
        if s>p: out.append((p,s))
        p=max(p,e)
    if p<size: out.append((p,size))
    return out,size
if __name__=='__main__':
    cov=load()
    for g,ns in FILES.items():
        for n in ns:
            gs,size=gaps(g,n,cov)
            print(g,n,hex(size),'gaps',len(gs),'bytes',sum(e-s for s,e in gs))
            for s,e in gs:
                if e-s>=256: print('  %06X %06X %d'%(s,e,e-s))
