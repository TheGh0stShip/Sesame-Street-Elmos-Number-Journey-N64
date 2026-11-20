import sys,struct
B='${N64DK_LAB}/sesassets/'
refs=[l.rstrip('\n').split('\t') for l in open(B+'LEVEL_REFS_number.tsv')][1:]
unc=[l.split('\t') for l in open(B+'lvlH/out/uncovered.tsv')]
minsz=int(sys.argv[1]) if len(sys.argv)>1 else 64
data={}
for g,f,s,e,n in unc:
    if f not in data: data[f]=open(B+f'files/out/number/files/{f}.bif','rb').read()
for g,f,s,e,n in unc:
    s=int(s,16);e=int(e,16)
    if e-s<minsz: continue
    d=data[f]
    lo,hi=0x80154000+s,0x80154000+e
    cr=[r[0]+':'+r[3] for r in refs if lo<=int(r[0],16)<hi]
    pw=[]
    for i in range(0,len(d)-3,4):
        w=struct.unpack_from('>I',d,i)[0]
        if lo<=w<hi: pw.append('%x->%x'%(i,w-0x80154000))
    print(f,hex(s),hex(e),e-s,'CODE',' '.join(cr)[:300],'PTRS',' '.join(pw[:12]),len(pw),sep='\t')
