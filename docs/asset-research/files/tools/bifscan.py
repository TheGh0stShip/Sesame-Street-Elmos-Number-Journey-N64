import sys,struct,collections
for fn in sys.argv[2:]:
    d=open(fn,'rb').read(); base=int(sys.argv[1],16)
    n=len(d)//4; w=struct.unpack('>%dI'%n,d[:n*4])
    inr=[i for i,x in enumerate(w) if base<=x<base+len(d)]
    ops=collections.Counter(x>>24 for i,x in enumerate(w) if i%2==0)
    dl=sum(1 for i in range(0,n-1,2) if w[i]>>24 in (0x04,0xBF,0xB8,0x06,0xFD,0xF5,0xBB,0xB6,0xB7) )
    print(fn.split('/')[-1],len(d),'ptrs',len(inr),'first',[hex(x) for x in w[:16]])
    print('  dl-like dwords',dl,'top ops',[(hex(k),v) for k,v in ops.most_common(8)])
    # pointer targets histogram by 64K
    print('  ptr pos sample', [hex(i*4) for i in inr[:10]])
