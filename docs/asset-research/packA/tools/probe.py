import sys,collections
for rom,base in ((sys.argv[1],0xA54D0),(sys.argv[2],0xA5940)):
    d=open(rom,'rb').read()[base:base+0x67C20]
    for a,b in ((0x4B800,0x54E20),):
        g=d[a:b]; c=collections.Counter(g)
        print(hex(a),hex(b),'zeros',c[0],'distinct',len(c), g[:64].hex())
        # find where nonzero runs
        nz=[i for i in range(0,len(g),0x200) if any(g[i:i+0x200])]
        print('nonzero 0x200 blocks',[hex(a+i) for i in nz][:10], len(nz))
