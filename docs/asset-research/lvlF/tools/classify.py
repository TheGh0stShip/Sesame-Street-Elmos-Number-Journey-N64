# analysis: classify uncovered bytes of Letter level files -> MAP rows (stdout) ; uses other agents' MAPs
import sys,struct,collections
B='${N64DK_LAB}/sesassets/'
GAME=sys.argv[1]; FILES=sys.argv[2].split(',')
BASE=0x80154000
HT={0x8006E5A0:32,0x8006E6B0:64,0x8006E7C0:64,0x8006E8D0:32,0x8006EA78:64,0x8006EC20:64}
def cov(path,skip):
    d=collections.defaultdict(list)
    for l in open(path):
        p=l.rstrip('\n').split('\t')
        if p[0]!=GAME or len(p)<4 or p[3] in skip: continue
        f,s=p[1].split(':'); e=p[2].split(':')[1]
        d[f.replace('.bif','')].append((int(s,16),int(e,16),p[3],p))
    return d
LB=cov(B+'lvlB/MAP.tsv',{'level_data'}); LC=cov(B+'lvlC/MAP.tsv',set())
refs=collections.defaultdict(set)
for l in open(B+f'LEVEL_REFS_{GAME}.tsv'):
    p=l.rstrip().split('\t')
    if p[0].startswith('0x') and p[1].startswith('level_image'):
        refs[int(p[0],16)-BASE].add(p[3])
def sprA(d,o,end):
    if o+8>end: return None
    he,do,ticks,nk,fl=struct.unpack('>HHBBH',d[o:o+8])
    if nk<2 or he!=8+nk or do not in ((he+7)&~7,(he+8)&~7) or fl&0x7fff: return None
    k=d[o+8:o+8+nk]
    if k[0]!=0 or any(k[i]>=k[i+1] for i in range(nk-1)) or k[-1]>ticks: return None
    nv=(end-o-do)//(4*nk)
    return do+nv*nk*4,nv,nk
for f in FILES:
    d=open(B+f'files/out/{GAME}/files/{f}.bif','rb').read(); n=len(d)
    c=bytearray(n)
    for s,e,_,_ in LB[f]+LC[f]: c[s:e]=b'\1'*(e-s)
    lab=[None]*n
    def setr(s,e,cls,fmt,desc,ref):
        for i in range(s,e):
            if lab[i] is None and not c[i]: lab[i]=(cls,fmt,desc,ref)
    # uncovered ranges
    rngs=[];i=0
    while i<n:
        if c[i]: i+=1; continue
        j=i
        while j<n and not c[j]: j+=1
        rngs.append((i,j)); i=j
    def rend(o):
        for s,e in rngs:
            if s<=o<e: return e
    # internal pointers
    R={o:set(v) for o,v in refs.items() if 0<=o<n and not c[o]}
    for i in range(0,n-3,4):
        w=struct.unpack('>I',d[i:i+4])[0]-BASE
        if 0<=w<n and not c[w]: R.setdefault(w,set()).add('ptr@%X'%(i+BASE))
    Rs=sorted(R)
    def nxt(o,e):
        for r in Rs:
            if r>o: return min(r,e)
        return e
    def nxtA(o,e):
        for r in Rs:
            if o<r<e and sprA(d,r,e): return r
        return e
    # model headers (lvlB) -> +0x24 bounds ptr
    for s,e,cl,p in LB[f]:
        if cl!='model_header': continue
        vo=struct.unpack('>I',d[s+0x14:s+0x18])[0]-BASE
        if 0<=vo<n and not c[vo]:
            q=vo
            while True:
                a_=struct.unpack('>H',d[q:q+2])[0]; q+=2
                if a_==0: break
                m_=struct.unpack('>H',d[q:q+2])[0]; q+=2
                if m_==0: break
                q+=2*m_
            setr(vo,q,'morph_vertex_order','u16 list {n, m, idx[m]}... 0','vertex expansion list for morph animation of model %X'%(s+BASE),'model_header %X +0x14 (func_80000450)'%(s+BASE))
        h=e  # header is 0x24 in lvlB; +0x24 word follows
        if h+4<=n and not c[h]:
            bp=struct.unpack('>I',d[h:h+4])[0]
            setr(h,h+4,'model_header_ext','u32 ptr','model header +0x24: pointer to its bounds box (0 if none); header is 0x28 bytes (func_80018530 copies 10 words)','model_header %X'%(s+BASE))
            if bp and 0<=bp-BASE<n and not c[bp-BASE]:
                b=bp-BASE
                setr(b,b+24,'model_bounds','f32[6]','bounding box min x,y,z / max x,y,z','model_header %X +0x24'%(s+BASE))
    for o in Rs:
        if lab[o] is not None: continue
        e=rend(o); why=','.join(sorted(R[o]))[:120]
        a=sprA(d,o,nxtA(o,e))
        if a and a[1]>0:
            setr(o,o+a[0],'morph_anim','sprA keyframed vertex morph','%d verts x %d keys'%(a[1],a[2]),why); continue
        if f=='space' and any(x.startswith('data_word@') and 0x80075C34<=int(x[10:],16)<=0x80075C58 for x in R[o]):
            setr(o,o+0x200,'texframe_palette','TLUT RGBA5551 256 colours','actor 0x4002 texture frame: palette',why)
            setr(o+0x200,o+0xA00,'texframe_texture','CI8 32x64','actor 0x4002 texture frame: texels',why); continue
        if f=='bonusabc':
            for x in R[o]:
                if x.startswith('func_80004010'): w=32;tb='func_80004010 literal'
                elif x.startswith('data_word@') and 0x8006E5A0<=int(x[10:],16)<0x8006EDC8:
                    da=int(x[10:],16); 
                    tb=[t for t in HT if t<=da][-1]; w=HT[tb]; tb='table %X'%tb
                    if da&4: continue
                else: continue
                setr(o,o+0x20,'hud_palette','TLUT RGBA5551 16 colours','palette for %X'%(o+0x20+BASE),tb+' (func_80004010)')
                setr(o+0x20,o+0x20+w*16,'hud_texture','CI4 %dx32'%w,'bonus-game HUD letter/picture',tb+' (func_80004010)')
                break
    if f=='bonusabc':
        i=0x11130
        while i<0x3EED0:
            if c[i] or lab[i] is not None: i+=1; continue
            j=i
            while j<0x3EED0 and not c[j] and lab[j] is None: j+=1
            if j-i==0x220:
                setr(i,i+0x20,'hud_palette','TLUT RGBA5551 16 colours','unreferenced (no table entry) palette for %X'%(i+0x20+BASE),'none (unused)')
                setr(i+0x20,j,'hud_texture','CI4 32x32','unreferenced bonus-game picture','none (unused)')
            elif j-i==0x400:
                setr(i,j,'hud_texture','CI4 64x32','unreferenced second texture without own palette (PNG uses the preceding palette)','none (unused)')
            i=j
        setr(0,0x18,'display_list','F3DEX','gsDPPipeSync; gsDPSetCombineMode; gsSPEndDisplayList (missed by lvlB)','80154000 func_8001D9D0/func_8000B470')
        setr(0x18,0x30,'display_list','F3DEX','gsDPPipeSync; gsDPSetCombineMode; gsSPEndDisplayList (missed by lvlB)','none found (twin of streetab 0x18)')
        for q in (0x55B90,0x55BE0,0x55C30):
            setr(q,q+0x50,'coll_quad','0x50: u32 type=3,0,mask,0; f32 normal[3],dist; f32 corners[4][3]','collision quad (two triangles) tested by func_80040C00','set 801A9C8C (func_80023118 case 0x1501)')
        setr(0x55C80,0x55C8C,'coll_quad_ptrs','u32[3]','pointers to the 3 quads','set 801A9C8C +4')
        setr(0x55C8C,0x55C94,'coll_quad_set','u32 count, u32 ptr','quad set of actor 0x1501 (copied count*0x50 bytes)','func_80023118 case 0x1501 (spD4)')
    if f=='space':
        for o,t,fm in ((0xC40,0xE40,'CI8 32x32'),(0x17D70,0x17F70,'CI8 64x32'),(0x18D70,0x18F70,'CI8 32x32')):
            setr(o,t,'unused_palette','TLUT RGBA5551 256 colours','palette of an unreferenced texture in the texture bank','none (unused)')
            setr(t,t+{'CI8 32x32':0x400,'CI8 64x32':0x800}[fm],'unused_texture',fm,'texture never loaded by any display list or code; size from the gap, width from its neighbours','none (unused)')
    # emit
    def runs():
        i=0
        while i<n:
            if c[i] or lab[i] is not None: 
                j=i
                if c[i]:
                    while j<n and c[j]: j+=1
                    i=j; continue
                L=lab[i]
                while j<n and not c[j] and lab[j]==L: j+=1
                yield i,j,L; i=j; continue
            j=i
            while j<n and not c[j] and lab[j] is None: j+=1
            # split: zero runs >=16 or trailing zero runs -> pad
            k=i
            while k<j:
                if d[k]==0:
                    z=k
                    while z<j and d[z]==0: z+=1
                    if z-k>=16 or z==j:
                        yield k,z,('pad','zero','zero fill','none'); k=z; continue
                    k=z; continue
                k+=1
            # now emit non-pad pieces
            i=j
    out=list(runs())
    # fill unknown gaps between yielded pieces inside unlabelled areas
    cover=bytearray(n)
    for a_,b_,L in out: cover[a_:b_]=b'\1'*(b_-a_)
    i=0
    while i<n:
        if c[i] or cover[i]: i+=1; continue
        j=i
        while j<n and not c[j] and not cover[j]: j+=1
        out.append((i,j,('unknown','','','')) ); i=j
    for a_,b_,L in sorted(out):
        print(GAME,'%s.bif:0x%05X'%(f,a_),'%s.bif:0x%05X'%(f,b_),*L,sep='\t')
