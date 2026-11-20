#!/usr/bin/env python3
"""make_map.py OUT_MAP.tsv : MAP.tsv (one line per object) from types_letter.tsv / types_number.tsv. No ROM needed."""
import sys, os
HERE = os.path.dirname(os.path.abspath(__file__))
CLS = {'char[]': ('string', 'NUL-terminated ASCII C string(s)'), 'f32': ('float', 'IEEE-754 single, big-endian'),
       'f64': ('double', 'IEEE-754 double, big-endian'), 'jtbl': ('jumptable', 'u32 code addresses (switch cases)'),
       'pad': ('pad', 'zero fill (section/object alignment)'), 'rsp_data': ('rsp_data', 'RSP DMEM image (see agent ucode)'),
       's64': ('data', 's64 array, big-endian'), 'u32': ('data', 'u32 words, big-endian'), 's32': ('data', 's32 array, big-endian'),
       'u16': ('data', 'u16 array'), 's16': ('data', 's16 array'), 'u8': ('data', 'byte array')}
L = ['game\trom_start\trom_end\tclass\tformat\tdescription\treferenced_by\textractor']
for g in ('letter', 'number'):
    for ln in open(os.path.join(HERE, 'types_%s.tsv' % g)):
        if ln.startswith('#'): continue
        a, sz, ct, nm, ev = ln.rstrip('\n').split('\t')
        a, sz = int(a, 16), int(sz, 16)
        cl, fm = CLS[ct]
        if nm.endswith('_tail'): cl = 'texture'
        sec = '.rodata' if a >= {'letter': 0x791B0, 'number': 0x799C0}[g] else '.data'
        desc = '%s %s %s' % (sec, nm, ct)
        ref = ev.split('@', 1)[1] if '@' in ev and not ev.startswith('tail') else ev
        L.append('\t'.join([g, '0x%X' % a, '0x%X' % (a + sz), cl, fm, desc, ref or '-',
                            'tools/gen_data.py (check mode verifies)']))
open(sys.argv[1], 'w').write('\n'.join(L) + '\n')
print(len(L) - 1, 'map lines')
