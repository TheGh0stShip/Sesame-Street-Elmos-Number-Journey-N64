"""lprobe.py: run a level-image owner's extractor in its check direction and capture the bytes it REBUILT.

Same technique as r4/tools/probe.py, extended one layer down: the ROM handed to the extractor is a ProbeBytes
(space 'ROM'), and zlib is replaced (for the duration of the run) by a shim whose decompress() of a ROM slice that
lies inside a gzip level member returns the inflated image as a ProbeBytes of space '<name>.bif' (offsets are image
offsets). Every equality test of such a slice against plain bytes records
(space, offset, rebuilt_bytes, equal, last_extracted_file_opened). Comparisons of image slices with other image
slices are ignored, so nothing from the ROM or the image enters a capture.

Writes: any file opened for writing outside g3/ goes to /dev/null and os.makedirs outside g3/ is a no-op, so the
probe never changes another agent's folder (also for helper modules such as png.py: builtins.open is patched)."""
import builtins, os, sys, io, types, zlib as _zlib, traceback
G3 = os.path.dirname(os.path.dirname(os.path.abspath(__file__))) + '/'


class ProbeBytes(bytes):
    def __new__(cls, data, base=0, space='ROM', log=None, last=None):
        o = super().__new__(cls, data); o.base = base; o.space = space; o.log = log; o.last = last; return o

    def __getitem__(self, k):
        r = bytes.__getitem__(self, k)
        if isinstance(k, slice):
            s, e, st = k.indices(len(self))
            if st != 1: return r
            return ProbeBytes(r, self.base + s, self.space, self.log, self.last)
        return r

    def _rec(self, other):
        if self.log is not None and isinstance(other, (bytes, bytearray)) and not isinstance(other, ProbeBytes):
            self.log.append((self.space, self.base, bytes(other), bytes.__eq__(self, bytes(other)),
                             self.last[-1] if self.last else None))

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


def make_zlib(members, log, last):
    """members: [(rom_start, rom_end, 'name.bif')]"""
    z = types.ModuleType('zlib')
    for k in dir(_zlib):
        if not k.startswith('__'): setattr(z, k, getattr(_zlib, k))

    def space_of(data):
        if isinstance(data, ProbeBytes) and data.space == 'ROM':
            for s, e, n in members:
                if s <= data.base < e: return n
        return None

    class DObj:
        def __init__(self, *a, **k): self._o = _zlib.decompressobj(*a, **k); self._sp = None; self._pos = 0
        def decompress(self, data, *a):
            r = self._o.decompress(data, *a); sp = space_of(data) or self._sp
            if sp:
                self._sp = sp; out = ProbeBytes(r, self._pos, sp, log, last); self._pos += len(r); return out
            return r
        def __getattr__(self, k): return getattr(self._o, k)

    def decompress(data, *a, **k):
        r = _zlib.decompress(data, *a, **k); sp = space_of(data)
        return ProbeBytes(r, 0, sp, log, last) if sp else r
    z.decompressobj = DObj; z.decompress = decompress
    return z


def run_script(path, argv, rom_path, members, patch=None):
    """exec the script as __main__ with sys.argv = [path] + argv. Returns (captures, opened, exit, stdout, error)."""
    log, opened, last = [], [], []
    rom = ProbeBytes(builtins.open(rom_path, 'rb').read(), 0, 'ROM', log, last)
    real_open, real_makedirs = builtins.open, os.makedirs

    def popen(f, mode='r', *a, **k):
        p = os.path.abspath(f) if isinstance(f, (str, bytes, os.PathLike)) and not isinstance(f, int) else None
        if p and p == os.path.abspath(rom_path) and 'r' in mode and 'b' in mode:
            return _ProbeFile(rom)
        if p and any(c in mode for c in 'wax+'):
            if not p.startswith(G3): return real_open(os.devnull, mode.replace('x', 'w'), *a, **k)
        elif p:
            opened.append(p); last.append(p)
        return real_open(f, mode, *a, **k)

    def pmakedirs(p, *a, **k):
        if os.path.abspath(p).startswith(G3): return real_makedirs(p, *a, **k)
    g = {'__name__': '__main__', '__file__': path, '__builtins__': builtins}
    old = sys.argv, list(sys.path), os.getcwd(), sys.stdout, sys.modules.get('zlib')
    sys.argv = [path] + list(argv); sys.path.insert(0, os.path.dirname(path))
    for m in [m for m in sys.modules if m in ('png', 'n64tex', 'f3dex')]: del sys.modules[m]   # each agent has its own copy
    sys.modules['zlib'] = make_zlib(members, log, last)
    builtins.open = popen; os.makedirs = pmakedirs
    buf = io.StringIO(); sys.stdout = buf; code = 0; err = None
    try:
        src = real_open(path).read()
        if patch: src = patch(src)
        exec(compile(src, path, 'exec'), g)
    except SystemExit as e:
        code = e.code if isinstance(e.code, int) else (0 if e.code is None else 1)
    except Exception:
        err = traceback.format_exc().strip().splitlines()[-3:]; code = 'crash'
    finally:
        builtins.open = real_open; os.makedirs = real_makedirs
        sys.argv, sys.stdout = old[0], old[3]; sys.path[:] = old[1]; os.chdir(old[2])
        sys.modules['zlib'] = old[4]
        for m in [m for m in sys.modules if m in ('png', 'n64tex', 'f3dex')]: del sys.modules[m]
    return log, opened, code, buf.getvalue(), err
