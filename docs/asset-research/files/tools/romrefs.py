"""List ROM-offset constants built in .text by lui/addiu|ori pairs (value range lo..hi)."""
import sys,struct,collections
rom=open(sys.argv[1],'rb').read(); t0,t1,lo,hi=(int(x,16) for x in sys.argv[2:6])
refs=collections.defaultdict(list); last={}
for o in range(t0,t1,4):
    w=struct.unpack('>I',rom[o:o+4])[0]; op=w>>26; rs=(w>>21)&31; rt=(w>>16)&31; imm=w&0xffff
    if op==0x0f: last[rt]=(imm<<16,o); continue
    if op in (9,0x0d) and rs in last and o-last[rs][1]<64:
        v=last[rs][0]+((imm-0x10000 if imm&0x8000 else imm) if op==9 else imm)
        if lo<=v<hi: refs[v].append(o+0x7FFFF400)
    if op==2 or op==3: pass
for v in sorted(refs): print('%08X'%v, ' '.join('%08X'%x for x in sorted(set(refs[v]))))
