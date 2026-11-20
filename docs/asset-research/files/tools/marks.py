import sys,re
rom=open(sys.argv[1],'rb').read()
a,b=int(sys.argv[2],16),int(sys.argv[3],16)
seg=rom[a:b]
for m in re.finditer(rb'\x00\x00\x02\x15',seg):
    if (a+m.start())%4==0: print('song?',hex(a+m.start()), seg[m.start():m.start()+48].hex())
for m in re.finditer(rb'N64 ',seg): print('n64',hex(a+m.start()))
for m in re.finditer(rb'\x00{32,}',seg): print('zeros',hex(a+m.start()),hex(a+m.end()))
for m in re.finditer(rb'[\x20-\x7e]{12,}',seg): print('str',hex(a+m.start()),m.group()[:60])
