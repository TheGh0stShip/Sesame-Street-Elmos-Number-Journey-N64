"""probe.py: run another agent's extractor in its check direction and capture the bytes it RE-ENCODED.

Each extractor's check mode builds bytes from its extracted files and compares them with a slice of the ROM.
The ROM handed to it here is a ProbeBytes: slices stay ProbeBytes and remember their ROM offset, and every
equality test of a ROM slice against plain bytes records (rom_start, rebuilt_bytes, equal). The recorded
bytes are the extractor's own output; nothing is copied from the ROM into the capture. A comparison of a ROM
slice with another ROM slice is ignored. Files opened by the extractor are logged (to tell which inputs were
standard re-encodable forms and which were verbatim .bin dumps)."""
import builtins, os, sys, io
MINE = os.path.dirname(os.path.dirname(os.path.abspath(__file__))) + '/'

class ProbeBytes(bytes):
    def __new__(cls, data, base=0, log=None):
        o = super().__new__(cls, data); o.base = base; o.log = log; return o
    def __getitem__(self, k):
        r = bytes.__getitem__(self, k)
        if isinstance(k, slice):
            s, e, st = k.indices(len(self))
            if st != 1: return r
            return ProbeBytes(r, self.base + s, self.log)
        return r
    def _rec(self, other):
        if self.log is not None and isinstance(other, (bytes, bytearray)) and not isinstance(other, ProbeBytes):
            self.log.append((self.base, bytes(other), bytes.__eq__(self, bytes(other))))
    def __eq__(self, other):
        self._rec(other); return bytes.__eq__(self, other)
    def __ne__(self, other):
        self._rec(other); return bytes.__ne__(self, other)
    __hash__ = bytes.__hash__

class _ProbeFile(io.BytesIO):
    def __init__(self, data): super().__init__(bytes(data)); self._d = data
    def read(self, n=-1):
        pos = self.tell()
        r = self._d[pos:] if n is None or n < 0 else self._d[pos:pos + n]
        self.seek(pos + len(r)); return r

def run_script(path, argv, rom_path, patch=None, chdir=None):
    """exec the script as __main__ with sys.argv = [path] + argv; opening rom_path for reading gives a
    ProbeBytes. Returns (captures, opened_files, exit_code, stdout_text)."""
    log = []; opened = []
    rom = ProbeBytes(builtins.open(rom_path, 'rb').read(), 0, log)
    real_open = builtins.open
    def popen(f, mode='r', *a, **k):
        p = os.path.abspath(f) if isinstance(f, str) else f
        if isinstance(f, str) and p == os.path.abspath(rom_path) and 'r' in mode and 'b' in mode:
            return _ProbeFile(rom)
        if isinstance(f, str) and any(c in mode for c in 'wax+'):
            if not p.startswith(MINE): return real_open(os.devnull, mode.replace('x', 'w'), *a, **k)   # never write outside r4
        elif isinstance(f, str): opened.append(p)
        return real_open(f, mode, *a, **k)
    g = {'__name__': '__main__', '__file__': path, 'open': popen, '__builtins__': builtins}
    old = sys.argv, list(sys.path), os.getcwd(), sys.stdout
    sys.argv = [path] + list(argv); sys.path.insert(0, os.path.dirname(path))
    for m in [m for m in sys.modules if m in ('png', 'n64tex', 'f3dex')]: del sys.modules[m]   # each agent has its own copy
    buf = io.StringIO(); sys.stdout = buf; code = 0
    if chdir: os.chdir(chdir)
    try:
        src = real_open(path).read()
        if patch: src = patch(src)
        exec(compile(src, path, 'exec'), g)
    except SystemExit as e:
        code = e.code if isinstance(e.code, int) else (0 if e.code is None else 1)
    finally:
        sys.argv, sys.path[:], cwd, sys.stdout = old[0], old[1], old[2], old[3]; os.chdir(cwd)
    return log, opened, code, buf.getvalue()
