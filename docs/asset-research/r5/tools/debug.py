#!/usr/bin/env python3
"""debug.py OUTDIR FUNCTIONS_JSON SRCROOT

Lists every debug / development function of both games from the call graph (OUTDIR/graph_<game>.json, made by
callgraph.py) and the string catalogue (OUTDIR/TEXT_<game>.tsv, made by strings.py):
  * the libultra debugger: rmon (rmon*.c) and rdb / kdebugserver (Letter address ranges, mapped to Number
    through the per-function hash: twins share it);
  * the print paths: osSyncPrintf, the KMC and rdb print sinks, the debug printf front end, __assert;
  * the KMC debugger hook in __osInitialize_common and the profiler sampling in __osTimerInterrupt;
  * game debug code: the on-screen debug overlay, the level-select text, the frame-timing statistics;
  * every function that calls a print path directly, and every function that uses a debug/error string.
Writes OUTDIR/debug_<game>.tsv: func, library, category, role, reach_from_entry, callers, debug strings used."""
import sys, os, json, re, csv

LETTER_SEEDS = {
    0x8005F808: ('print', 'debug printf front end: _Printf to the KMC port if a KMC debugger was detected, else to rdb'),
    0x8005F86C: ('print', 'osSyncPrintf (prints only when the KMC hook is installed)'),
    0x8005F5D0: ('print', 'KMC print sink: tab expansion, length-prefixed lines to the KMC port 0xBFF0xxxx'),
    0x8005F8B4: ('print', 'rdb print sink: __osRdbSend type 1 (print) packets'),
    0x80057BD0: ('print', '__assert: "ASSERTION FAULT: %s, %d" through the debug printf'),
    0x80047F60: ('print', 'game print helper: debug printf of a message, then a newline'),
    0x80065FA0: ('print', '_Printf (libultra xprintf.c), shared with sprintf'),
    0x80056DD0: ('hook', '__osInitialize_common: KMC debugger detection (0xBFF00000 == "KMC\\0"), installs the exception hook and sets the print flag'),
    0x800577A4: ('profiler', '__osTimerInterrupt with the profiler sampling branch'),
    0x80001F60: ('overlay', 'on-screen debug overlay: 4 pages of positions/timings/collision values drawn into a texture'),
    0x800301C8: ('overlay', 'level-select text: "Level select = %s" with a level-name table'),
    0x8000D0CC: ('overlay', 'frame-timing statistics (frame time, frame rate, lag counters)'),
    0x8001E314: ('error', 'divide-by-zero report ("DIVIDE BY ZERO!!!")'),
}
RANGES = [((0x8005EF28, 0x8005F410), 'rmon', 'rmon debugger: send/recv helpers, rmonMain, rdb thread'),
          ((0x80061730, 0x80061A40), 'rdb', 'rdb debug port: buffer send, kdebugserver'),
          ((0x80061AD0, 0x80061AD4), 'rdb', '__osRdbSend'),
          ((0x80063600, 0x80065C80), 'rmon', 'rmon debugger: breakpoints, registers, memory, threads (rmonbrk/rmonregs/rmontask/rmonmem)')]
DEBUG_TXT = re.compile(r'%|\\n|ASSERT|\.c"|DIVIDE|full|blown|Tried|delete|same frame|passed last|Level select|Render Update|ELMO|Ernie"|Grover"|B\.Bird|Cookie"|Count"|Screen Control|imem|rmon|"EX"')


def main(outdir, fj, srcroot):
    allf = json.load(open(fj))
    hashes = {}
    for f in allf: hashes.setdefault(f['game'], {})[f['vram']] = f['hash']
    lh = hashes['letter']
    for game in ('letter', 'number'):
        g = json.load(open(os.path.join(outdir, 'graph_%s.json' % game)))
        by_hash = {}
        for k, v in g.items(): by_hash.setdefault(v['hash'], []).append(int(k, 16))
        def to_game(a):
            if game == 'letter': return [a]
            return by_hash.get(lh.get(a), [])
        cat = {}
        for a, (c, r) in LETTER_SEEDS.items():
            for b in to_game(a): cat[b] = (c, r)
        if game == 'number':
            cat[0x80001F60] = ('overlay', 'on-screen debug overlay (Number variant: adds Cookie/Count pages and a Screen Control page with FOV and screen height)')
        for (lo, hi), c, r in RANGES:
            for a in lh:
                if lo <= a < hi:
                    for b in to_game(a): cat.setdefault(b, (c, r))
        # strings used per function
        used = {}
        for row in csv.DictReader(open(os.path.join(outdir, 'TEXT_%s.tsv' % game)), delimiter='\t'):
            if row['area'] not in ('.data', '.rodata') or not DEBUG_TXT.search(row['text']): continue
            for m in re.findall(r'func_([0-9A-F]{8})', row['referenced_by']):
                used.setdefault(int(m, 16), []).append(row['text'])
        prints = {a for a, (c, r) in cat.items() if c == 'print' and a not in to_game(0x80065FA0)}
        for k, v in g.items():
            a = int(k, 16)
            if a in cat: continue
            if any(int(x, 16) in prints for x in v['callees']):
                cat[a] = ('calls-print', 'calls a debug print path directly')
            elif a in used:
                cat[a] = ('uses-debug-string', 'uses debug/error text (see strings)')
        with open(os.path.join(outdir, 'debug_%s.tsv' % game), 'w') as o:
            o.write('func\tlibrary\tcategory\trole\treach_from_entry\tcallers\tdebug_strings_used\n')
            for a in sorted(cat):
                v = g['%08X' % a]
                o.write('func_%08X\t%s\t%s\t%s\t%s\t%s\t%s\n' % (a, v['lib'], cat[a][0], cat[a][1],
                        'yes' if v['reach_entry'] else 'NO', ','.join(sorted(set(('tbl@' if x.startswith('data') else '') + x.split(':')[1][:8] for x in v['refs_in']))) or '-',
                        ' | '.join(used.get(a, []))[:300] or '-'))
        print(game, len(cat), 'debug functions;', sum(1 for a in cat if not g['%08X' % a]['reach_entry']), 'unreachable')


if __name__ == '__main__':
    main(*sys.argv[1:4])
