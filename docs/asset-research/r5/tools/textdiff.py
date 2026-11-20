#!/usr/bin/env python3
"""textdiff.py OUTDIR: program strings (TEXT_<game>.tsv, areas .data/.rodata/header) present in one game only,
and staff-credit lines that differ by index. Writes OUTDIR/diff_text.tsv."""
import sys, os, csv
o = sys.argv[1]
t = {}
for g in ('letter', 'number'):
    t[g] = [r for r in csv.DictReader(open(os.path.join(o, 'TEXT_%s.tsv' % g)), delimiter='\t') if r['area'] in ('.data', '.rodata', 'header')]
with open(os.path.join(o, 'diff_text.tsv'), 'w') as out:
    out.write('kind\tgame\tram\ttext\treferenced_by\n')
    cred = {g: {r['referenced_by'].split('[')[1].split(']')[0]: r for r in t[g] if 'staff credit' in r['note']} for g in t}
    for k in sorted(set(cred['letter']) | set(cred['number']), key=int):
        a, b = cred['letter'].get(k), cred['number'].get(k)
        if not a or not b or a['text'] != b['text']:
            out.write('credit line %s\tletter|number\t%s|%s\t%s | %s\t-\n' % (k, a and a['ram_address'], b and b['ram_address'], a and a['text'], b and b['text']))
    other = {g: {r['text'] for r in t[g] if 'staff credit' not in r['note']} for g in t}
    for g, h in (('letter', 'number'), ('number', 'letter')):
        for r in t[g]:
            if 'staff credit' in r['note'] or r['text'] in other[h]: continue
            out.write('only in %s\t%s\t%s\t%s\t%s\n' % (g, g, r['ram_address'], r['text'], r['referenced_by'][:80]))
print('ok')
