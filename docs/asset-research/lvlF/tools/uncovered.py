import sys,os,csv
B='${N64DK_LAB}/sesassets/'
FILES={'letter':['garden','space','planet','bonusabc'],'number':['maze','ski','river','bonus123']}
def cov(path,skip):
    d={}
    for l in open(path):
        p=l.rstrip('\n').split('\t')
        if p[0]=='game' or len(p)<4: continue
        if p[3] in skip: continue
        f,s=p[1].split(':'); _,e=p[2].split(':')
        d.setdefault((p[0],f.replace('.bif','')),[]).append((int(s,16),int(e,16)))
    return d
b=cov(B+'lvlB/MAP.tsv',{'level_data'}); c=cov(B+'lvlC/MAP.tsv',set())
for g,fs in FILES.items():
    for f in fs:
        n=os.path.getsize(B+f'files/out/{g}/files/{f}.bif')
        cv=bytearray(n)
        for s,e in b.get((g,f),[])+c.get((g,f),[]):
            cv[s:e]=b'\1'*(e-s)
        i=0;tot=0
        while i<n:
            if cv[i]: i+=1; continue
            j=i
            while j<n and not cv[j]: j+=1
            print(g,f,hex(i),hex(j),j-i,sep='\t'); tot+=j-i; i=j
        print('#',g,f,'size',hex(n),'uncovered',tot,file=sys.stderr)
