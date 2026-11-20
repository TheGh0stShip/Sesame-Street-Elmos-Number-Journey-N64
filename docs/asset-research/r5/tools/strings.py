#!/usr/bin/env python3
"""strings.py LETTER_ROM NUMBER_ROM FUNCTIONS_JSON FILESDIR OUTDIR

Catalogue of every text string in both games, written to OUTDIR/TEXT_<game>.tsv:
  * program image (.data/.rodata, the ROM header name, the boot segment): every NUL-terminated run of >= 4
    printable ASCII bytes (also Shift-JIS runs, if any), with the functions that build its address
    (lui + addiu/ori pairs) or that build the address of the data object holding a pointer to it;
  * asset files: the raw ROM files after the program (audio banks, songs, speech, morph animations, menu and
    common images) and the 8 level images after inflation, plus the gzip header file names. Asset data is
    binary, so a run there counts as text only when it is NUL- or edge-delimited, >= 6 bytes, made only of
    letters/digits/punctuation, has >= 4 letters and no run of 4 equal bytes; each one is listed with whether
    any pointer (code, program data, or a word inside the same image) holds its RAM address.
Columns: game, area, rom_offset, ram_address (or image offset), encoding, length, text (C-escaped),
referenced_by, note."""
import sys, os, re, bisect
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from seslib import *

PRINT = set(range(0x20, 0x7F)) | {9, 10, 13}
ASSET_OK = re.compile(rb'^[A-Za-z0-9 _.,:;!?\'"()/%\-+#=*&@\[\]<>~\\]+$')


def esc(b):
    return b.decode('latin-1').encode('unicode_escape').decode('ascii').replace('\t', '\\t')


def runs(buf, minlen, allow):
    i, n = 0, len(buf)
    while i < n:
        if buf[i] in allow:
            j = i
            while j < n and buf[j] in allow: j += 1
            if j - i >= minlen:
                yield i, j
            i = j
        else:
            i += 1


def sjis_runs(buf):
    for m in re.finditer(rb'(?:[\x81-\x9F\xE0-\xEF][\x40-\x7E\x80-\xFC]){3,}\x00', buf):
        yield m.start(), m.end() - 1


def plausible_asset(s):
    return (len(s) >= 6 and ASSET_OK.match(s) and sum(c in b'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz' for c in s) >= 4
            and not re.search(rb'(.)\1\1\1', s))


BINARY = {'audio_bank', 'texture', 'palette', 'vertex', 'vertex_list', 'morph_anim', 'speech', 'song', 'display_list',
          'collision', 'bbox', 'model_header', 'level_object', 'code', 'rsp', 'boot', 'unknown', 'pad'}


def ledger(ledgerdir, game):
    import csv
    rom, lv = [], {}
    for r in csv.DictReader(open(os.path.join(ledgerdir, 'LEDGER_%s.tsv' % game)), delimiter='\t'):
        rom.append((int(r['start'], 16), int(r['end'], 16), r['kind'], r['description'][:60]))
    for r in csv.DictReader(open(os.path.join(ledgerdir, 'LEVELS_%s.tsv' % game)), delimiter='\t'):
        lv.setdefault(r['file'], []).append((int(r['start'], 16), int(r['end'], 16), r['kind'], r['description'][:60]))
    rom.sort(); [l.sort() for l in lv.values()]
    def look(tab, a):
        best = None
        for s, e, k, d in tab:          # innermost (shortest) row containing a
            if s <= a < e and (best is None or e - s < best[1] - best[0]): best = (s, e, k, d)
        return best
    return (lambda a: look(rom, a)), (lambda f, a: look(lv.get(f, []), a))


MAGIC = (b'GCC', b'N64 ', b'MORT')


def program_objects(datadirs, game):
    """(rom_start, rom_end, class, description) of every .data/.rodata object typed by agents data1 and data2."""
    import csv
    out = []
    for d in datadirs.split(','):
        for r in csv.DictReader(open(os.path.join(d, 'MAP.tsv')), delimiter='\t'):
            if r['game'] != game: continue
            out.append((int(r['rom_start'], 16), int(r['rom_end'], 16), r['class'], r['format'] + ' ' + r['description']))
    out.sort()
    return out


def lookobj(objmap, a):
    i = bisect.bisect_right(objmap, (a, 1 << 40)) - 1
    while i >= 0:
        s, e, c, d = objmap[i]
        if s <= a < e: return objmap[i]
        if e <= a and s < a - 0x10000: break
        i -= 1
    return None


def main(lrom, nrom, fj, filesdir, ledgerdir, datadirs, outdir):
    os.makedirs(outdir, exist_ok=True)
    for game, rp in (('letter', lrom), ('number', nrom)):
        rom = open(rp, 'rb').read()
        fs = load_functions(fj, game)
        code = scan_code(rom, fs)
        built = {}
        for v, c in code.items():
            for t, pc in c['addrs']:
                built.setdefault(t, set()).add(v)
        dlo, dhi = SEG[game]['data'][0], SEG[game]['rodata'][1]
        objs = sorted(a for a in built if dlo <= a < dhi)
        dwords = {}
        for a, w in data_words(rom, game):
            dwords.setdefault(w, []).append(a)

        def obj_of(a):
            i = bisect.bisect_right(objs, a) - 1
            return objs[i] if i >= 0 else None

        def prog_refs(ram, ln):
            """exact references to ram; interior addresses (ram+1..ram+ln-1) are returned separately."""
            out, inner = [], []
            for t in range(ram, ram + ln):
                dst = out if t == ram else inner
                for f in sorted(built.get(t, ())):
                    dst.append('func_%08X' % f + ('' if t == ram else '(+%d)' % (t - ram)))
                for a in dwords.get(t, ()):
                    ob = obj_of(a)
                    us = sorted(built.get(ob, ())) if ob is not None else []
                    dst.append('ptr@%08X[%s]' % (a, ','.join('func_%08X' % u for u in us) or 'no code ref to its table'))
            return out, inner
        rejected = []
        rows = []
        rows.append(('header', 0x20, '-', 'ascii', rom[0x20:0x34].rstrip(b' \0'), 'boot code (not referenced by game code)', 'ROM header image name (20-byte field)'))
        # program .data/.rodata, typed by the data1/data2 object maps
        objmap = program_objects(datadirs, game)
        a0, a1 = dlo - RAM2ROM, dhi - RAM2ROM
        buf = rom[a0:a1]
        prog = []
        for st, en, cls, d in objmap:          # pass 1: every piece of every string/text object
            if cls not in ('string', 'text') or 'CreditLine' in d or not (a0 <= st < a1): continue
            k = st
            while k < en:
                e = rom.index(b'\0', k)
                if e > k and all(c in PRINT for c in rom[k:e]):
                    prog.append((k - a0, min(e, en) - a0, 'ascii', cls, d))
                k = e + 1
        instr = set()
        for i, j, *_ in prog: instr.update(range(i, j))
        for i, j in runs(buf, 4, PRINT):       # pass 2: text-like runs elsewhere (kept only when referenced)
            if (j < len(buf) and buf[j] != 0) or i in instr or (j - 1) in instr: continue
            ob = lookobj(objmap, a0 + i)
            if ob and 'CreditLine' in ob[3]: continue
            prog.append((i, j, 'ascii', ob[2] if ob else '?', ob[3] if ob else ''))
        for i, j in sjis_runs(buf):
            prog.append((i, j, 'shift-jis', '?', ''))
        prog.sort()
        last_ref = None
        for i, j, enc, cls, d in prog:
            ram = dlo + i
            s = buf[i:j]
            r, inner = prog_refs(ram, j - i)
            if cls not in ('string', 'text') and not r and not (cls == 'rsp_data' and len(s) >= 16):
                # a run may start with the last bytes of the previous object: retry at the next 4 offsets
                for t in range(1, 4):
                    if len(s) - t >= 4 and prog_refs(ram + t, 1)[0]:
                        i += t; ram += t; s = s[t:]; r, inner = prog_refs(ram, j - i); break
            keep = cls in ('string', 'text') or (cls == 'rsp_data' and len(s) >= 16) or r
            if not keep:
                rejected.append(('program %s' % cls, a0 + i, cls, s)); continue
            note = 'object class %s (%s)' % (cls, d[:60]) if d else 'no typed object'
            if r:
                last_ref = (ram, dlo + j)
            elif cls == 'rsp_data':
                note += '; read by the RSP (DMEM image), not by CPU code'
            else:
                if last_ref and ram - last_ref[0] < 0x400 and all(c in PRINT or c == 0 for c in rom[last_ref[1] - RAM2ROM:a0 + i]):
                    note += '; no direct reference: entry of a string table that starts at %08X (reached by index)' % last_ref[0]
                else:
                    note += '; UNREFERENCED: no code address build, no data pointer'
            if inner:
                note += '; interior addresses built by %s (unrelated %%hi/%%lo arithmetic or offsets)' % ','.join(inner[:6])
            sect = '.rodata' if ram >= SEG[game]['rodata'][0] else '.data'
            rows.append((sect, a0 + i, '%08X' % ram, enc, s, ';'.join(r) or '-', note))
        # staff credits: struct {s32 x; s32 y; char text[40];}[N]
        for st, en, cls, d in objmap:
            if 'CreditLine' not in d: continue
            tab = st + RAM2ROM
            users = ','.join('func_%08X' % u for u in sorted(built.get(tab, ()))) or '-'
            for k in range((en - st) // 0x30):
                o = st + k * 0x30 + 8
                t = rom[o:o + 40].split(b'\0')[0]
                if not t: continue
                rows.append(('.data', o, '%08X' % (o + RAM2ROM), 'ascii', t, 'table %08X[%d].text (%s)' % (tab, k, users),
                             'staff credit line %d; x=%d y=%d' % (k, int.from_bytes(rom[o - 8:o - 4], 'big', signed=True), int.from_bytes(rom[o - 4:o], 'big', signed=True))))
        # boot/entry code area strings (IPL3 + text before .data): report only plausible ones
        for i, j in runs(rom[0x40:0x1000], 6, PRINT):
            s = rom[0x40 + i:0x40 + j]
            if plausible_asset(s):
                rows.append(('ipl3', 0x40 + i, '-', 'ascii', s, '-', 'inside boot code'))
        # assets
        romlook, lvlook = ledger(ledgerdir, game)
        luis = {}
        for f in fs:
            for o, w in words(rom, f['rom'], f['rom'] + f['size']):
                if w >> 26 == 15: luis.setdefault(w & 0xFFFF, set()).add(f['vram'])

        def magic_users(m):
            u = luis.get(int.from_bytes(m[:2], 'big'), set())
            return ('code builds the constant high half 0x%04X in %s (possible magic compare)' % (int.from_bytes(m[:2], 'big'), ','.join('func_%08X' % x for x in sorted(u)))) if u else 'no code builds its first two bytes as a constant: no 32-bit constant compare against it in code'
        files = files_table(os.path.join(filesdir, 'FILES_%s.tsv' % game))
        for a, b, cls, note in files:
            if cls == 'gzip_level':
                nm = gzip_name(rom, a)
                if nm:
                    rows.append(('gzip header ' + cls, a + 10, '-', 'ascii', nm.encode('latin-1'), 'not read by the game (gzip FNAME field, skipped by the reader)', 'original file name'))
                img = gunzip_member(rom, a, b)
                base = 0x80154000
                iw = {}
                for o in range(0, len(img) - 3, 4):
                    iw.setdefault(int.from_bytes(img[o:o + 4], 'big'), []).append(o)
                for i, j in runs(img, 6, PRINT):
                    s = img[i:j]
                    if not plausible_asset(s) and not s.startswith(MAGIC): continue
                    k = lvlook(nm, i)
                    if k and k[2] in BINARY and not s.startswith(MAGIC):
                        rejected.append((nm, i, k[2], s)); continue
                    ram = base + i
                    r = prog_refs(ram, 1)[0] + ['img+%X' % o for o in iw.get(ram, [])]
                    rows.append(('level image %s (inflated)' % (nm or hex(a)), a, '%08X(+%X)' % (ram, i), 'ascii', s,
                                 ';'.join(r) or '-', ('in %s: %s; ' % (k[2], k[3]) if k else 'outside every ledger row; ') +
                                 ('' if r else 'UNREFERENCED: no pointer in code, program data or this image')))
                continue
            seg = rom[a:b]
            base = None
            if cls == 'gfx_pack':
                base = 0x8024B000 if note.startswith('common') else 0x80154000
            iw = {}
            if base:
                for o in range(0, len(seg) - 3, 4):
                    iw.setdefault(int.from_bytes(seg[o:o + 4], 'big'), []).append(o)
            if cls == 'speech_mort':
                n = seg.count(b'MORT')
                rows.append((cls, a + seg.find(b'MORT'), '-', 'ascii', b'MORT', 'stream magic, %d occurrences in this file; %s' % (n, magic_users(b'MORT')),
                             'GSM speech stream header magic (first occurrence listed)'))
            for i, j in runs(seg, 6, PRINT):
                s = seg[i:j]
                if not plausible_asset(s) and not s.startswith(MAGIC): continue
                k = romlook(a + i)
                if k and k[2] in BINARY and not s.startswith(MAGIC):
                    rejected.append(('rom', a + i, k[2], s)); continue
                ram = '%08X' % (base + i) if base else '-'
                r = (prog_refs(base + i, 1)[0] + ['img+%X' % o for o in iw.get(base + i, [])]) if base else []
                where = ('in %s: %s; ' % (k[2], k[3])) if k else 'outside every ledger row; '
                rows.append((cls, a + i, ram, 'ascii', s, ';'.join(r) or (magic_users(s) if s.startswith(MAGIC) else '-'),
                             where + ('' if r or s.startswith(MAGIC) else 'UNREFERENCED: no pointer to it')))
        # unused ROM tail
        last = max(b for a, b, c, n in files)
        for i, j in runs(rom[last:], 6, PRINT):
            s = rom[last + i:last + j]
            if plausible_asset(s):
                rows.append(('after last file', last + i, '-', 'ascii', s, '-', 'past every file'))
        with open(os.path.join(outdir, 'TEXT_%s.tsv' % game), 'w') as o:
            o.write('game\tarea\trom_offset\tram_address\tencoding\tlength\ttext\treferenced_by\tnote\n')
            for area, rom_off, ram, enc, s, ref, note in rows:
                o.write('%s\t%s\t0x%06X\t%s\t%s\t%d\t"%s"\t%s\t%s\n' % (game, area, rom_off, ram, enc, len(s), esc(s), ref, note))
        with open(os.path.join(outdir, 'rejected_%s.tsv' % game), 'w') as o:
            for f, off, k, t in rejected:
                o.write('%s\t0x%X\t%s\t"%s"\n' % (f, off, k, esc(t)))
        print(game, 'rejected', len(rejected), 'letter runs inside binary ledger classes')
        print(game, len(rows), 'strings;', sum(1 for r in rows if 'UNREFERENCED' in r[6]), 'unreferenced')


if __name__ == '__main__':
    main(*sys.argv[1:8])
