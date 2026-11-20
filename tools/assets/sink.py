"""sink.py: run an asset owner's `check` mode WITHOUT the ROM and keep the bytes it re-encodes.

An owner's check mode rebuilds bytes from its extracted files and compares them with slices of the ROM (or of an
inflated level image). Here the ROM it opens is a stand-in that holds NO ROM content: a buffer of one fill byte
(the gzip member headers, which are generated from the layout table, are the only non-fill bytes), and the level
images it inflates are fill buffers of the right size (sizes from the layout table). Every equality test of such
a slice against plain bytes records (space, offset, bytes) and answers "equal", so the check runs to its end
without seeing anything of the ROM. The captured bytes are the encoder's output.

Independence test (driver.py): an owner is run with fill 0x00 and with fill 0xFF; the captures must be identical,
so they cannot depend on the stand-in's content. Owners whose checks need real ROM content to find their structure
fail that test and have explicit encoders in owners.py instead.
Writes by the owner script go to /dev/null; the real ROM path is never given to it."""
import builtins, io, os, sys, types, traceback, zlib as _zlib

FAKE_ROM = '/nonexistent/baserom.z64'


class VBytes(bytes):
    def __new__(cls, data, base=0, space='ROM', log=None):
        o = super().__new__(cls, data); o.base = base; o.space = space; o.log = log; return o

    def __getitem__(self, k):
        r = bytes.__getitem__(self, k)
        if isinstance(k, slice):
            s, e, st = k.indices(len(self))
            if st != 1: return r
            return VBytes(r, self.base + s, self.space, self.log)
        return r

    def _rec(self, other):
        if self.log is not None and isinstance(other, (bytes, bytearray)) and not isinstance(other, VBytes):
            self.log.append((self.space, self.base, bytes(other)))
            return True
        return False

    def __eq__(self, other):
        if self._rec(other): return True
        return bytes.__eq__(self, other)

    def __ne__(self, other):
        if self._rec(other): return False
        return bytes.__ne__(self, other)
    __hash__ = bytes.__hash__


class _VFile(io.BytesIO):
    def __init__(self, data): super().__init__(b''); self._d = data; self._p = 0
    def read(self, n=-1):
        r = self._d[self._p:] if n is None or n < 0 else self._d[self._p:self._p + n]
        self._p += len(r); return r
    def seek(self, p, w=0):
        self._p = p if w == 0 else (self._p + p if w == 1 else len(self._d) + p); return self._p
    def tell(self): return self._p


def gz_header(name):
    return b'\x1f\x8b\x08\x08' + bytes(6) + name.encode() + b'\0'


def make_rom(size, members, fill, log):
    """members: [(rom_start, rom_end, name, image_size)]; only the generated gzip headers are not fill."""
    b = bytearray([fill]) * size
    for s, e, n, isz in members:
        h = gz_header(n); b[s:s + len(h)] = h
    return VBytes(bytes(b), 0, 'ROM', log)


def make_zlib(members, fill, log):
    z = types.ModuleType('zlib')
    for k in dir(_zlib):
        if not k.startswith('__'): setattr(z, k, getattr(_zlib, k))

    def member_of(data):
        if isinstance(data, VBytes) and data.space == 'ROM':
            for s, e, n, isz in members:
                if s <= data.base < e: return n, isz
        return None

    class DObj:
        def __init__(self, *a, **k): self._o = _zlib.decompressobj(*a, **k); self._done = False; self.unused_data = b''; self.eof = False
        def decompress(self, data, *a):
            m = member_of(data)
            if m is None: return self._o.decompress(data, *a)
            if self._done: return b''
            self._done = True; self.eof = True
            return VBytes(bytes([fill]) * m[1], 0, m[0], log)
        def flush(self, *a): return b''
    def decompress(data, *a, **k):
        m = member_of(data)
        if m is None: return _zlib.decompress(data, *a, **k)
        return VBytes(bytes([fill]) * m[1], 0, m[0], log)
    z.decompressobj = DObj; z.decompress = decompress
    return z


def run_check(path, argv, size, members, fill, allow_write=None):
    """exec owner script `path` as __main__ with argv (FAKE_ROM stands for the ROM). Returns (log, exit, out, err)."""
    log = []
    rom = make_rom(size, members, fill, log)
    real_open, real_makedirs, real_mkdir = builtins.open, os.makedirs, os.mkdir

    def popen(f, mode='r', *a, **k):
        if isinstance(f, (str, os.PathLike)):
            p = os.path.abspath(f)
            if p == FAKE_ROM:
                return _VFile(rom)
            if any(c in mode for c in 'wax+') and not (allow_write and p.startswith(allow_write)):
                return real_open(os.devnull, mode.replace('x', 'w').replace('+', ''), *a, **k)
        return real_open(f, mode, *a, **k)

    def pmakedirs(p, *a, **k):
        if allow_write and os.path.abspath(p).startswith(allow_write): return real_makedirs(p, *a, **k)
    g = {'__name__': '__main__', '__file__': path, '__builtins__': builtins}
    old = sys.argv, list(sys.path), os.getcwd(), sys.stdout, sys.modules.get('zlib')
    sys.argv = [path] + list(argv); sys.path.insert(0, os.path.dirname(path))
    for m in [m for m in sys.modules if m in ('png', 'n64tex', 'f3dex', 'gaps', 'seslib')]: del sys.modules[m]
    sys.modules['zlib'] = make_zlib(members, fill, log)
    builtins.open = popen; os.makedirs = pmakedirs; os.mkdir = lambda p, *a, **k: pmakedirs(p)
    buf = io.StringIO(); sys.stdout = buf; code = 0; err = None
    try:
        src = real_open(path).read()
        exec(compile(src, path, 'exec'), g)
    except SystemExit as e:
        code = e.code if isinstance(e.code, int) else (0 if e.code is None else 1)
    except Exception:
        err = traceback.format_exc().strip().splitlines()[-4:]; code = 'crash'
    finally:
        builtins.open = real_open; os.makedirs = real_makedirs; os.mkdir = real_mkdir
        sys.argv, sys.stdout = old[0], old[3]; sys.path[:] = old[1]; os.chdir(old[2])
        sys.modules['zlib'] = old[4]
        for m in [m for m in sys.modules if m in ('png', 'n64tex', 'f3dex', 'gaps', 'seslib')]: del sys.modules[m]
    return log, code, buf.getvalue(), err
