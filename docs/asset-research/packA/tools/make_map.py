import sys; sys.path.insert(0, sys.argv[1])
from extract_packA import layout, size_of, FILES, RAM
rows=['game\trom_start\trom_end\tclass\tformat\tdescription\treferenced_by\textractor']
for g in ('letter','number'):
    s,e=FILES[g]; end=s
    for off,name,k,pe,w,h,ref in layout():
        n=size_of(k,pe,w,h); a=s+off; assert a==end; end=a+n
        fmt='TLUT %d x RGBA5551 (0x%X B) + %s %dx%d (0x%X B)'%(pe,pe*2,k.upper(),w,h,n-pe*2)
        rows.append('\t'.join([g,'0x%06X'%a,'0x%06X'%end,'texture',fmt,
          'mode-22 menu %s (RAM 0x%08X)'%(name,RAM+off),
          ref+'; file loaded by %s case 22'%('func_8001D9D0' if g=='letter' else 'func_8001E2CC'),
          'tools/extract_packA.py']))
    assert end==e
open(sys.argv[2],'w').write('\n'.join(rows)+'\n')
