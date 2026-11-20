import sys,struct
rom=open(sys.argv[1],'rb').read()
code=rom[0x1000:0x7CB20]
targets=[int(x,16) for x in sys.argv[2:]]
for t in targets:
    hits=[]
    for o in range(0,len(code)-3,4):
        w=struct.unpack('>I',code[o:o+4])[0]
        if w==t: hits.append('word@%X'%(o+0x80000400))
        if (w>>26)==0xF and (w&0xffff)==((t+0x8000)>>16)&0xffff:
            rt=(w>>16)&31
            for j in range(1,12):
                q=o+4*j
                if q+4>len(code):break
                v=struct.unpack('>I',code[q:q+4])[0]
                if (v>>26) in (9,13,0x23,0x2b,0x21,0x25) and ((v>>21)&31)==rt and (v&0xffff)==(t&0xffff):
                    hits.append('lui@%X'%(o+0x80000400))
    print('%08X'%t,hits)
