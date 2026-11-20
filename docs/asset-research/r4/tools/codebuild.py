"""codebuild.py GAME OUTDIR [ADDR...]: compile every verified function source of GAME (sesverify v16) with its
recorded lane and flags and keep the relocatable object: OUTDIR/<game>/code/obj/func_ADDR.o + func_ADDR.json.
Resumable: functions whose object already exists are skipped. Linking (placing .text at its real address and
the function's own .rodata at the address the link map gives) is done by rebuild_rom.py, without the compiler.
Run through job.sh with cwd=sydkit, env.sh sourced, N64DK_TOOLCHAIN=toolroot, PYTHONPATH=sydkit."""
import sys, os, re, json, shutil, tempfile, time
from pathlib import Path
from n64dk.match.pipeline import build, split_flags
from n64dk.match.toolchain import load_config
from n64dk.match import autodraft
L = os.path.expanduser('${N64DK_LAB}'); V = L + '/sesverify/v17'
GAME, OUT = sys.argv[1], sys.argv[2]; ONLY = set(int(x, 16) for x in sys.argv[3:])
od = os.path.join(OUT, GAME, 'code', 'obj'); os.makedirs(od, exist_ok=True)
proj = Path(tempfile.mkdtemp(prefix='r4cb-')); cfg = load_config(proj, None, None)
PRE = autodraft.PRELUDE + "\n"
n = ok = 0; t0 = time.time()
for ln in open(V + '/' + GAME + '/VERIFY.tsv'):
    c = ln.rstrip('\n').split('\t')
    if len(c) < 5 or not c[0]: continue
    v = int(c[0], 16)
    if ONLY and v not in ONLY: continue
    if not c[3].startswith('MATCH'): continue
    o = os.path.join(od, 'func_%08X.o' % v)
    if os.path.exists(o): continue
    how = re.sub(r'\(.*?\)', '', c[4]).split()
    lane, flags = how[0], ' '.join(how[1:])
    src = V + '/' + GAME + '/src/func_%08X.c' % v
    n += 1
    folder = proj / ('c%d' % n); folder.mkdir(); (folder / 'f.c').write_text(PRE + open(src).read())
    try:
        obj = build(folder / 'f.c', split_flags(flags.split(), lane), cfg, folder)
        shutil.copy(str(obj), o + '.tmp'); os.replace(o + '.tmp', o)
        json.dump(dict(vram='%08X' % v, size=int(c[1]), lane=lane, flags=flags, src=src), open(o[:-2] + '.json', 'w'))
        ok += 1
    except Exception as e:
        print('FAIL %08X %s' % (v, str(e)[:200]))
    shutil.rmtree(str(folder), True)
    if n % 50 == 0: print('%d compiled, %.0fs' % (n, time.time() - t0), flush=True)
shutil.rmtree(str(proj), True)
print(GAME, 'compiled', ok, 'of', n, '%.0fs' % (time.time() - t0))
