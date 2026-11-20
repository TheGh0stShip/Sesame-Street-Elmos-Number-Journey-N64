#!/usr/bin/env python3
"""rebuild_rom.py GAME OUTDIR [--redo STAGE,...]
End-to-end rebuild proof: construct the complete 8 MB ROM image of GAME from extracted assets, compiled
function sources and generated padding, then compare it byte for byte (and by SHA-1) with the original.

Stages (each cached under OUTDIR/<game>/cache/, so the run resumes; --redo assets,code forces a stage):
  assets  every owner agent's extractor is run in its check direction through probe.py (or, where the owner's
          check re-encodes from the ROM rather than from its files, its encoder is called on the extracted
          files directly: sprA, speech); level gzip members are re-built from the inflated images.
  code    every function object compiled by codebuild.py is linked here: .text at its real address, its own
          .rodata/.data sections at the addresses the link map gives (derived from the typed data1/data2
          objects that name the function, matched against the REBUILT data image), func_/D_ names by the
          address in them, real names (libgcc/libm) from sesverify NAMES.tsv.
  assemble  every ledger row is filled from those layers (its owner's first, then any other owner's), padding
          rows are generated as zeros, the header CRCs are recomputed over the rebuilt image, and whatever is
          left is an explicit exception (original bytes placed, reason recorded).
Outputs: OUTDIR/<game>/rebuilt.z64 (lab only), OUTDIR/REBUILD_<game>.tsv, OUTDIR/SUMMARY_<game>.json, LINKMAP_<game>.tsv.
Run with PYTHONPATH=<lab>/sydkit (n64dk ELF relocation) through job.sh. Reads the ROM; writes only under OUTDIR."""
import sys, os, re, json, struct, zlib, hashlib, pickle, glob, importlib.util, collections
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import probe
L = os.path.expanduser('${N64DK_LAB}'); S = L + '/sesassets'; PY = None
ROMS = {'letter': L + "/roms/Sesame Street - Elmo's Letter Adventure (USA).z64",
        'number': L + "/roms/Sesame Street - Elmo's Number Journey (USA).z64"}
UNMATCHED = {'letter': 0x80069504, 'number': 0x8006AE74}
RAM2ROM = 0x7FFFF400
RODATA = {'letter': 0x791B0, 'number': 0x799C0}
DATA = {'letter': (0x6D5AC, 0x7C6B0), 'number': (0x6EF1C, 0x7CB20)}
N = 0x800000
# source codes per byte
SRC = {0: 'none', 1: 'rebuilt-from-extracted', 2: 'rebuilt-from-extracted(other-owner)', 3: 'compiled',
       4: 'generated-pad', 5: 'extracted-verbatim', 6: 'generated', 9: 'exception'}

def load_mod(path, name, strip=None):
    src = open(path).read()
    if strip: src = strip(src)
    g = {'__name__': name, '__file__': path}
    sys.path.insert(0, os.path.dirname(path))
    for m in [m for m in sys.modules if m in ('png', 'n64tex', 'f3dex')]: del sys.modules[m]
    try: exec(compile(src, path, 'exec'), g)
    finally: sys.path.pop(0)
    return g

# ------------------------------------------------------------------ asset adapters
# each returns a list of captures (rom_start, bytes, equal_flag_or_None, tag) and a note
def run_probe(owner, script, argv, rom_path, game, tagfn=None, patch=None):
    log, opened, code, out = probe.run_script(script, argv, rom_path, patch=patch)
    caps = [(a, b, e, tagfn(a, len(b)) if tagfn else 1) for a, b, e in log]
    bins = sorted(set(p for p in opened if p.endswith('.bin')))
    return caps, dict(owner=owner, exit=code, captures=len(caps), opened=len(set(opened)), bin_inputs=len(bins),
                      tail=out.strip().splitlines()[-3:])

def ad_audio(game, rom_path, hintfree):
    # extract_audio.adpcm_encode_check(pcm, book, orig) tries the ORIGINAL frame header first. With hintfree the
    # original is replaced by zeros, so the header choice comes from the WAV + JSON alone.
    def patch(src):
        if not hintfree: return src
        return src.replace('def adpcm_encode_check(pcm, book, orig):',
                           'def adpcm_encode_check(pcm, book, orig):\n    orig = bytes(len(orig))', 1)
    caps, note = run_probe('audio', S + '/audio/tools/extract_audio.py', ['check', rom_path, S + '/audio/out'], rom_path, game, patch=patch)
    rom = open(rom_path, 'rb').read(); out = []; dropped = 0
    for a, b, e, t in caps:
        if e or len(b) % 9 > 8: out.append((a, b, e, t)); continue
        # an unequal sample: keep the 9-byte frames that re-encode equal; the others become exceptions
        for k in range(0, len(b), 9):
            fr = b[k:k + 9]
            if fr == rom[a + k:a + k + len(fr)]: out.append((a + k, fr, True, t))
            else: dropped += 1
    note['adpcm_frames_not_determined'] = dropped
    return out, note

def ad_sprA(game, rom_path):
    g = load_mod(S + '/sprA/tools/extract_sprites.py', 'sprA', strip=lambda s: s.replace('\nmain()', '\n'))
    caps = []
    for p in sorted(glob.glob(S + '/sprA/out/%s/*.json' % game)):
        a = int(os.path.basename(p)[:6], 16)
        caps.append((a, g['encode'](json.load(open(p))), None, 1))
    return caps, dict(owner='sprA', note='encode() of every out/<game>/*.json (its own check mode re-encodes the ROM parse)', captures=len(caps))

def ad_speech(game, rom_path):
    g = load_mod(S + '/speech/tools/extract_speech.py', 'speech')
    caps = []
    for p in sorted(glob.glob(S + '/speech/out/%s_*.params.json' % game)):
        j = json.load(open(p))
        hdr = dict(frames=j['frames'], rate=j['rate'], words=j['words'])
        w0 = struct.unpack('>I', j['magic'].encode())[0]
        b, _ = g['repack'](hdr, j['runs'], w0)
        caps.append((j['rom'], b, None, 1))
    return caps, dict(owner='speech', note='repack() of every <game>_*.params.json (its own check re-packs the ROM parse)', captures=len(caps))

def ad_packC(game, rom_path):
    idx = json.load(open(S + '/packC/out/%s/index.json' % game)); base = idx['rom_start']
    verb = [(base + o['start'], base + o['end']) for o in idx['objects'] if o['file'] and o['file'].endswith('.bin')]
    vs = sorted(verb)
    import bisect
    def tag(a, n):
        return 5 if any(s <= a and a + n <= e for s, e in vs) else 1
    caps, note = run_probe('packC', S + '/packC/tools/extract_packC.py', ['check', rom_path, game, S + '/packC/out/' + game], rom_path, game)
    # packC's check compares the whole file at once: split the capture by object so .bin pieces are tagged
    out = []
    for a, b, e, t in caps:
        for o in idx['objects']:
            s, z = base + o['start'], base + o['end']
            if a <= s and z <= a + len(b):
                out.append((s, b[s - a:z - a], None, 5 if (o['file'] or '').endswith('.bin') else 1))
    note['verbatim_bin_objects'] = len(verb)
    return out, note

def ad_ucode(game, rom_path):
    od = S + '/ucode/out/ucode_' + game
    caps, note = run_probe('ucode', S + '/ucode/tools/extract_ucode.py', [rom_path, od, '--check'], rom_path, game,
                           tagfn=lambda a, n: 5, patch=None)
    note['note'] = 'RSP microcode blobs are binary dumps (.bin): extracted-verbatim'
    return caps, note

def gzip_members(game):
    rows = []
    for l in open(S + '/ledger/LEDGER_%s.tsv' % game):
        c = l.rstrip('\n').split('\t')
        if c[5] == 'lvlA' and c[2] == 'gzip_level': rows.append((int(c[0], 16), int(c[1], 16), c))
    return rows

def ad_lvlA(game, rom_path, rom):
    """gzip members: header generated from the documented fields (FNAME = image name, mtime 0, xfl 0, os 0);
    deflate stream: zlib re-compression of the inflated image tried at every level / memLevel / strategy;
    CRC32 + ISIZE generated from the inflated image."""
    caps = []; info = []
    for s, e, c in gzip_members(game):
        name = re.search(r'(\w+\.bif)', c[4]).group(1)
        data = open(S + '/lvlA/out/%s/%s.bin' % (game, name), 'rb').read()
        hdr = b'\x1f\x8b\x08\x08' + struct.pack('<I', 0) + b'\x00\x00' + name.encode() + b'\x00'
        trailer = struct.pack('<II', zlib.crc32(data) & 0xFFFFFFFF, len(data) & 0xFFFFFFFF)
        dlen = (e - s) - len(hdr) - 8
        # inflated content proof: inflate the ROM member and compare with the extracted image
        d = zlib.decompressobj(-15); inf = d.decompress(rom[s + len(hdr):e]) + d.flush()
        same = inf == data
        orig = rom[s + len(hdr):s + len(hdr) + dlen]
        hit = None
        for lv in (9, 6, 8, 7, 5, 4, 3, 2, 1):
            for ml in (8, 9, 7, 6, 5, 4, 3, 2, 1):
                for st in (0, 1, 2, 3, 4):
                    for wb in (-15, -14, -13, -12, -11, -10, -9):
                        co = zlib.compressobj(lv, 8, wb, ml, st)
                        head = co.compress(data[:65536])
                        if head and head != orig[:len(head)]: continue
                        full = head + co.compress(data[65536:]) + co.flush()
                        if full == orig: hit = (lv, ml, st, wb); break
                    if hit: break
                if hit: break
            if hit: break
        caps.append((s, hdr, None, 6)); caps.append((e - 8, trailer, None, 6))
        if hit:
            co = zlib.compressobj(hit[0], 8, hit[3], hit[1], hit[2]); caps.append((s + len(hdr), co.compress(data) + co.flush(), None, 1))
        info.append(dict(name=name, start=s, end=e, deflate_bytes=dlen, inflated=len(data), inflated_equal=same,
                         zlib=hit, status='rebuilt' if hit else 'container-not-reproduced'))
    return caps, dict(owner='lvlA', members=info)

def ad_header(img, rom):
    """header CRC1/CRC2 recomputed over the REBUILT image with ucode's check_header.crc"""
    g = load_mod(S + '/ucode/tools/check_header.py', 'hdr', strip=lambda s: s.split("\nif __name__")[0])
    c = zlib.crc32(rom[0x40:0x1000]) & 0xFFFFFFFF          # identify CIC from the IPL3 (an exception range anyway)
    cic, seed = g['CIC'][c]
    c1, c2 = g['crc'](bytes(img), cic, seed)
    return [(0x10, struct.pack('>II', c1, c2), None, 6)]

ASSET_JOBS = {
    'audio': lambda game, rp, rom: ad_audio(game, rp, True),
    'songs': lambda game, rp, rom: run_probe('songs', S + '/songs/tools/extract_songs.py', ['check', rp, game, S + '/songs/out/' + game], rp, game),
    'packA': lambda game, rp, rom: run_probe('packA', S + '/packA/tools/extract_packA.py', ['check', rp, game, S + '/packA/out/' + game], rp, game),
    'packB': lambda game, rp, rom: run_probe('packB', S + '/packB/tools/extract_packB.py', [rp, game, S + '/packB/out', 'check'], rp, game),
    'packC': lambda game, rp, rom: ad_packC(game, rp),
    'hud': lambda game, rp, rom: run_probe('hud', S + '/hud/tools/extract_hud.py', ['check', rp, game, S + '/hud/out'], rp, game),
    'lvlD': lambda game, rp, rom: run_probe('lvlD', S + '/lvlD/tools/extract_level_misc.py', ['check', rp, game, S + '/lvlD/out/' + game], rp, game),
    'gfx': lambda game, rp, rom: run_probe('gfx', S + '/gfx/tools/extract_gfx.py', ['check', rp, S + '/gfx/out'], rp, game),
    'data1': lambda game, rp, rom: run_probe('data1', S + '/data1/tools/gen_data.py', ['check', rp, game, S + '/data1/out'], rp, game),
    'data2': lambda game, rp, rom: run_probe('data2', S + '/data2/tools/gen_data.py', ['check', rp, game, S + '/data2/out/c'], rp, game),
    'ucode': lambda game, rp, rom: ad_ucode(game, rp),
    'sprA': lambda game, rp, rom: ad_sprA(game, rp),
    'speech': lambda game, rp, rom: ad_speech(game, rp),
    'lvlA': lambda game, rp, rom: ad_lvlA(game, rp, rom),
}

def cached(path, fn, redo):
    if os.path.exists(path) and not redo: return pickle.load(open(path, 'rb'))
    r = fn(); pickle.dump(r, open(path + '.tmp', 'wb')); os.replace(path + '.tmp', path); return r

# ------------------------------------------------------------------ code linking
def link_code(game, img, have, rom, cdir):
    from n64dk.elf import Elf
    from n64dk.match import compare as cmp
    from n64dk.match.symbols import SymbolMap
    F = {f['vram']: f for f in json.load(open(L + '/sestools/functions.json')) if f['game'] == game and f['rom'] is not None}
    names = {}
    for l in open(L + '/sesverify/v17/%s/NAMES.tsv' % game):
        _, n, a = l.rstrip('\n').split('\t')
        if a != '?': names[n] = int(a, 16)
    for kd in ('kitd', 'kit'):                              # SDK names the kit identified (symbol table, no bytes)
        p = L + '/sesagents/%s/%s/symbols/sdk_symbols.txt' % (kd, game)
        if os.path.exists(p):
            for l in open(p):
                m = re.match(r'\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)', l)
                if m and m.group(1) not in names: names[m.group(1)] = int(m.group(2), 16)
    # typed objects of data1/data2 (RAM address, size, evidence) -> per function candidate addresses
    ev = collections.defaultdict(list)
    for p, ramcol in ((S + '/data2/tools/types_%s.tsv' % game, False), (S + '/data1/tools/types_%s.tsv' % game, True)):
        for l in open(p):
            if l.startswith('#') or not l.strip(): continue
            c = l.rstrip('\n').split('\t')
            try: a = int(c[0], 16)
            except ValueError: continue
            ram = a if ramcol else a + RAM2ROM
            for fn in re.findall(r'func_([0-9A-Fa-f]{8})', '\t'.join(c[1:])): ev[int(fn, 16)].append(ram)
    dlo, dhi = DATA[game]                                    # program .data/.rodata region in ROM (ASSET_FACTS)
    res = {}; linkmap = []
    for v, f in sorted(F.items()):
        o = cdir + '/func_%08X.o' % v
        if not os.path.exists(o): res[v] = None; continue
        try: res[v] = link_one(game, v, f, o, Elf, cmp, SymbolMap, names, ev, dlo, dhi, img, have, rom, linkmap)
        except KeyError as ex:
            res[v] = ('unresolved-name', None, str(ex)); linkmap.append((v, 'unresolved-name', str(ex)))
    return res, linkmap

def link_one(game, v, f, o, Elf, cmp, SymbolMap, names, ev, dlo, dhi, img, have, rom, linkmap):
    """Link one function. Each own data section (.rodata etc.) is cut into ITEMS at the offsets .text refers
    to (a jump table, a literal, a string); each item is placed independently: first inside the typed
    data1/data2 objects whose evidence names this function (in address order, after the previous item),
    else by searching the rebuilt data image. If that deterministic choice does not reproduce the text,
    the other candidate addresses are tried and the function is flagged resolved-by-compare."""
    elf = Elf.load(o)
    syms = SymbolMap(); syms.update(names)
    secs = [s for s in ('.rodata', '.rdata', '.lit4', '.lit8', '.data', '.sdata') if elf.section(s) is not None and len(elf.section(s).data)]
    tmp = {s: 0x81000000 + k * 0x100000 for k, s in enumerate(secs)}
    rels = list(cmp.relocations(elf, '.text'))
    # item of each relocation (HI16 paired with the following LO16 of the same symbol)
    owner = {}; pend = {}
    for r in rels:
        if r.section not in secs: continue
        if r.type == 5: pend.setdefault(r.symbol, []).append(r.offset); continue
        owner[r.offset] = (r.section, r.addend)
        for h in pend.pop(r.symbol, []): owner[h] = (r.section, r.addend)
    evo = sorted(set(ev.get(v, [])))
    items = []                                              # (section, item_offset, core, [candidate ROM addrs])
    for s in secs:
        blob = cmp.relocate(elf, s, dict({'.text': v}, **tmp), syms)
        offs = sorted(set([0] + [ad for (sec, ad) in owner.values() if sec == s and 0 <= ad < len(blob)]))
        for i, oo in enumerate(offs):
            end = offs[i + 1] if i + 1 < len(offs) else len(blob)
            raw = blob[oo:end]; k = len(raw.rstrip(b'\0'))
            k = min(len(raw), max(-(-k // 4) * 4, 8 if len(raw) >= 8 else 4))   # keep a literal's own zero bytes
            core = raw[:k] if k else raw
            items.append([s, oo, core, [], ''])
    cursor = 0; prev = None
    for it in items:
        s, oo, core, cands, _ = it
        if not any(core) and prev is not None and prev[0] == s and prev[3]:
            # an all-zero item (zero-initialised static) has no content to match: keep it contiguous with the
            # previous item of its section, as the compiler laid it out
            cands.append(prev[3][0] + (oo - prev[1])); it[4] = 'contiguous'; prev = it; continue
        prev = it
        for r in evo:
            for d in range(-len(core) + 4, 64, 4):          # the typed object may start before or after the item
                a = r + d - RAM2ROM
                if dlo <= a and a + len(core) <= dhi and have[a:a + len(core)].all() and bytes(img[a:a + len(core)]) == core and a not in cands: cands.append(a)
        it[4] = 'typed-evidence'
        if not cands:
            used = set(x[3][0] + RAM2ROM for x in items if x is not it and x[3])
            lo_ = RODATA[game] if s in ('.rodata', '.rdata', '.lit4', '.lit8') else dlo
            free_ev = [r - RAM2ROM for r in evo if r not in used and lo_ <= r - RAM2ROM < dhi]
            if free_ev and len(free_ev) <= len(items) + 2:
                # the function's own typed object exists but the compiled literal differs from its bytes
                cands.extend(free_ev); it[4] = 'typed-evidence(content-differs)'
        if not cands and it[4] != 'contiguous':
            it[4] = 'data-image-search'; reg = bytes(img[dlo:dhi]); p = reg.find(core)
            while p != -1:
                if have[dlo + p:dlo + p + len(core)].all(): cands.append(dlo + p)
                p = reg.find(core, p + 1)
        cands.sort(key=lambda a: (a % 4 != 0, a < cursor, a))               # deterministic: first at/after the previous item
        if cands: cursor = cands[0] + len(core)
    if any(not it[3] for it in items):
        info = [(it[0], it[1], it[4], 'NOT FOUND' if not it[3] else '%X' % it[3][0]) for it in items]
        linkmap.append((v, 'unlinked', info)); return ('unlinked', None, info)
    def build(choice):
        base = cmp.relocate(elf, '.text', dict({'.text': v}, **tmp), syms); out = bytearray(base)
        cache = {}
        for off, (sec, ad) in owner.items():
            k = max((j for j, it in enumerate(items) if it[0] == sec and it[1] <= ad), key=lambda j: items[j][1])
            if k not in cache:
                b2 = dict(tmp); b2[sec] = choice[k] + RAM2ROM - items[k][1]
                cache[k] = cmp.relocate(elf, '.text', dict({'.text': v}, **b2), syms)
            out[off:off + 4] = cache[k][off:off + 4]
        return bytes(out)
    choice = [it[3][0] for it in items]
    built = build(choice); meth = ','.join(sorted(set(it[4] for it in items))) or 'no-own-sections'
    n = f['size']
    T = rom[f['rom']:f['rom'] + n]
    words = {k: [off for off, (sec, ad) in owner.items() if sec == items[k][0] and max((j for j, it in enumerate(items) if it[0] == sec and it[1] <= ad), key=lambda j: items[j][1]) == k] for k in range(len(items))}
    fixed = 0
    for k, it in enumerate(items):                          # an item whose deterministic address gives other words
        if all(built[w:w + 4] == T[w:w + 4] for w in words[k]): continue
        for a in it[3][1:]:
            c2 = list(choice); c2[k] = a; b2 = build(c2)
            if all(b2[w:w + 4] == T[w:w + 4] for w in words[k]): choice, built = c2, b2; fixed += 1; break
    if fixed: meth += ' resolved-by-compare(%d items)' % fixed
    ends = [k for k in range(0, len(built), 4) if built[k:k + 4] == b'\x03\xe0\x00\x08']
    if ends and not any(built[max(ends) + 8:]): built = built[:max(ends) + 8]
    built = built[:n]
    linkmap.append((v, meth, [(it[0] + '+%X' % it[1], '%08X' % (c + RAM2ROM), len(it[2])) for it, c in zip(items, choice)]))
    return ('ok', built + bytes(n - len(built)), (len(built), meth, choice))

# ------------------------------------------------------------------ main
def main():
    game, OUT = sys.argv[1], sys.argv[2]
    redo = set(sys.argv[sys.argv.index('--redo') + 1].split(',')) if '--redo' in sys.argv else set()
    rp = ROMS[game]; rom = open(rp, 'rb').read(); assert len(rom) == N
    gd = os.path.join(OUT, game); cd = os.path.join(gd, 'cache'); os.makedirs(cd, exist_ok=True)
    # ledger
    rows = []
    for l in open(S + '/ledger/LEDGER_%s.tsv' % game):
        c = l.rstrip('\n').split('\t')
        if c[0] == 'start': continue
        rows.append(dict(s=int(c[0], 16), e=int(c[1], 16), cls=c[2], desc=c[4], owner=c[5], kind=c[7]))
    # stage assets
    layers = {}; notes = {}
    for owner, fn in ASSET_JOBS.items():
        caps, note = cached(os.path.join(cd, 'asset_%s.pkl' % owner), lambda: fn(game, rp, rom), 'assets' in redo or owner in redo)
        layers[owner] = caps; notes[owner] = note
        print(owner, json.dumps(note, default=str)[:300], flush=True)
    # per-owner layers as arrays
    def paint(caps):
        img = np.zeros(N, np.uint8); tag = np.zeros(N, np.uint8)
        for a, b, e, t in caps:
            if a < 0 or a >= N: continue
            b = b[:N - a]; img[a:a + len(b)] = np.frombuffer(b, np.uint8); tag[a:a + len(b)] = t
        return img, tag
    P = {o: paint(c) for o, c in layers.items()}
    # combined data image for the code link map: data1 + data2 (+ lvlD tables, ucode data) only
    dimg = np.zeros(N, np.uint8); dhave = np.zeros(N, bool)
    for o in ('ucode', 'data2', 'data1', 'lvlD'):
        i, t = P[o]; m = t > 0; dimg[m] = i[m]; dhave[m] = True
    # stage code
    cres, linkmap = cached(os.path.join(cd, 'code.pkl'), lambda: link_code(game, dimg, dhave, rom, os.path.join(gd, 'code', 'obj')), 'code' in redo)
    F = {f['vram']: f for f in json.load(open(L + '/sestools/functions.json')) if f['game'] == game and f['rom'] is not None}
    cimg = np.zeros(N, np.uint8); ctag = np.zeros(N, np.uint8)
    for v, r in cres.items():
        if r and r[0] == 'ok':
            a = F[v]['rom']; cimg[a:a + len(r[1])] = np.frombuffer(r[1], np.uint8); ctag[a:a + len(r[1])] = 3
    # assemble
    R = np.frombuffer(rom, np.uint8)
    img = np.zeros(N, np.uint8); src = np.zeros(N, np.uint8); why = {}
    order = list(P)
    for r in rows:
        s, e = r['s'], r['e']; sl = slice(s, e)
        free = np.ones(e - s, bool)
        def take(im, tg, code):
            m = free & (tg[sl] > 0)
            if m.any():
                img[sl][m] = im[sl][m]
                src[sl][m] = np.where(tg[sl][m] == 1, code, tg[sl][m]) if code != 3 else 3
                free[m] = False
        if r['owner'] == 'code': take(cimg, ctag, 3)
        if r['owner'] in P: take(*P[r['owner']], 1)
        for o in order:
            if o != r['owner'] and free.any() and r['owner'] != 'code': take(*P[o], 2)
        if free.any() and r['kind'] == 'pad':
            img[sl][free] = 0; src[sl][free] = 4
            stale = free & (R[sl] != 0)                     # zero padding generated; stale non-zero bytes are exceptions
            free[:] = stale
        if free.any():
            img[sl][free] = R[sl][free]; src[sl][free] = 9
            why[s] = exception_reason(r, game, F, cres)
    # header CRC over the rebuilt image (after everything else is placed)
    for a, b, _, t in ad_header(img, rom):
        img[a:a + len(b)] = np.frombuffer(b, np.uint8); src[a:a + len(b)] = 6
    eq = img == R
    # TSV: segments of equal (source, equal) inside each ledger row
    out = ['start\tend\tbytes\towner\tkind\tclass\tsource\tequal\tnote']
    tot = collections.Counter(); bad = collections.Counter()
    for r in rows:
        s, e = r['s'], r['e']
        ss = src[s:e]; qq = eq[s:e]
        brk = np.flatnonzero((ss[1:] != ss[:-1]) | (qq[1:] != qq[:-1])) + 1
        cuts = [0] + brk.tolist() + [e - s]
        for a, z in zip(cuts, cuts[1:]):
            code = int(ss[a]); q = bool(qq[a]); n = z - a
            note = why.get(s, '') if code == 9 else ''
            if code == 3: note = 'func_%08X' % rom_func(F, s + a)
            out.append('0x%06X\t0x%06X\t%d\t%s\t%s\t%s\t%s\t%s\t%s' % (s + a, s + z, n, r['owner'], r['kind'], r['cls'], SRC[code], 'yes' if q else 'no', note))
            tot[SRC[code]] += n
            if not q: bad[SRC[code]] += n
    open(os.path.join(OUT, 'REBUILD_%s.tsv' % game), 'w').write('\n'.join(out) + '\n')
    open(os.path.join(OUT, 'LINKMAP_%s.tsv' % game), 'w').write('func\tmethod\tsections(name=address/size)\n' + ''.join(
        '%08X\t%s\t%s\n' % (v, m, ' '.join('%s=%s/%d' % x for x in i) if m not in ('unlinked', 'unresolved-name') else str(i)) for v, m, i in linkmap))
    img.tofile(os.path.join(gd, 'rebuilt.z64'))
    sha_o = hashlib.sha1(rom).hexdigest(); sha_r = hashlib.sha1(img.tobytes()).hexdigest()
    # exceptions replaced by zeros: what the extracted/decompiled form alone gives
    nz = img.copy(); nz[src == 9] = 0
    summ = dict(game=game, sha1_original=sha_o, sha1_rebuilt=sha_r, sha1_equal=sha_o == sha_r,
                bytes_by_source=dict(tot), bytes_not_equal_by_source=dict(bad),
                exception_bytes=int((src == 9).sum()), exceptions=[dict(start='0x%06X' % k, reason=v) for k, v in sorted(why.items())],
                reproduced_equal_bytes=int(((src != 9) & (src != 5) & eq).sum()), verbatim_extracted_bytes=int((src == 5).sum()),
                owners=notes, code=dict(linked=sum(1 for x in cres.values() if x and x[0] == 'ok'),
                                        unlinked=[('%08X' % v) for v, x in cres.items() if x and x[0] != 'ok'],
                                        missing=[('%08X' % v) for v, x in cres.items() if x is None]))
    json.dump(summ, open(os.path.join(OUT, 'SUMMARY_%s.json' % game), 'w'), indent=1, default=str)
    print(json.dumps({k: summ[k] for k in ('sha1_original', 'sha1_rebuilt', 'sha1_equal', 'bytes_by_source', 'bytes_not_equal_by_source', 'exception_bytes', 'reproduced_equal_bytes', 'verbatim_extracted_bytes', 'code')}, indent=1))

def rom_func(F, a):
    for v, f in F.items():
        if f['rom'] <= a < f['rom'] + f['size']: return v
    return 0

def exception_reason(r, game, F, cres):
    if r['owner'] == 'code':
        v = rom_func(F, r['s'])
        if v == UNMATCHED[game]: return 'unmatched function func_%08X: original bytes placed' % v
        x = cres.get(v)
        return 'func_%08X: %s' % (v, 'no object' if x is None else x[0])
    if r['kind'] == 'boot': return 'IPL3 boot code: no extracted form (identified only)'
    if r['kind'] == 'header': return 'ROM header fields other than CRC: no extracted form'
    if r['kind'] == 'container' and r['owner'] == 'lvlA': return 'gzip deflate stream not reproduced by zlib (container-not-reproduced; inflated content proven equal)'
    if r['owner'] == 'audio' and r['kind'] != 'pad': return 'ADPCM frames whose (scale, predictor) header the WAV + JSON do not determine (hint-free re-encode picks another equivalent header)'
    if r['kind'] == 'pad': return 'non-zero padding (stale bytes) not regenerated by its owner'
    return 'no extractor re-encoded these bytes (owner %s)' % r['owner']

if __name__ == '__main__':
    main()
