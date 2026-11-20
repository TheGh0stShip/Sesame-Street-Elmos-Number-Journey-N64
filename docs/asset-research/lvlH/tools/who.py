import sys,struct
# who.py LEVEL START END : aligned words in the level image pointing into [START,END)
f,s,e=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3],16)
d=open(f'${N64DK_LAB}/sesassets/files/out/number/files/{f}.bif','rb').read()
for i in range(0,len(d)-3,4):
    w=struct.unpack_from('>I',d,i)[0]-0x80154000
    if s<=w<e: print(f'0x{i:X} -> 0x{w:X}')
