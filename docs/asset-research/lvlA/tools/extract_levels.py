#!/usr/bin/env python3
"""extract_levels.py ROM GAME OUTDIR [check]
Inflate the 8 gzip level members (class gzip_level in FILES_<game>.tsv) to OUTDIR/<stored name>.bin.
check: verify CRC32/ISIZE, and try to reproduce the compressed bytes with zlib levels 1-9."""
import sys, zlib, struct, os, json
HERE = os.path.dirname(os.path.abspath(__file__))
FILES = os.path.join(HERE, '..', '..', 'FILES_%s.tsv')

ROWS = []
def members(game):
    for line in open(FILES % game):
        f = line.rstrip('\n').split('\t')
        if len(f) > 4 and f[0] != 'id':
            ROWS.append((int(f[1], 16), int(f[2], 16), f[4]))
    for s, e, c in ROWS:
        if c == 'gzip_level':
            yield s, e

def true_end(member_end):
    # the loader's start/end pair: first file boundary at or after the member end
    return min(r[1] for r in ROWS if r[1] >= member_end)

def parse(rom, s, e):
    b = rom[s:]
    assert b[:3] == b'\x1f\x8b\x08', hex(s)
    flg, mtime, xfl, os_ = b[3], *struct.unpack('<I', b[4:8]), b[8], b[9]
    p = 10
    if flg & 4: n = struct.unpack('<H', b[p:p+2])[0]; p += 2 + n
    name = None
    if flg & 8: q = b.index(0, p); name = b[p:q].decode('latin1'); p = q + 1
    if flg & 16: p = b.index(0, p) + 1
    if flg & 2: p += 2
    d = zlib.decompressobj(-15); data = d.decompress(b[p:]) + d.flush()
    dend = len(b) - len(d.unused_data)
    crc, isize = struct.unpack('<II', b[dend:dend+8])
    tail = b[dend+8:e-s] if s+dend+8 <= e else b''
    return dict(start=s, end=e, flg=flg, mtime=mtime, xfl=xfl, os=os_, name=name, hdr=p,
                deflate=b[p:dend], data=data, crc=crc, isize=isize,
                member_end=s+dend+8, tail_len=len(tail), tail_zero=not any(tail))

def main():
    rom = open(sys.argv[1], 'rb').read(); game = sys.argv[2]; out = sys.argv[3]
    check = len(sys.argv) > 4 and sys.argv[4] == 'check'
    os.makedirs(out, exist_ok=True); ok = True
    for s, e in members(game):
        m = parse(rom, s, e)
        if m['member_end'] > e:
            e = true_end(m['member_end']); m = parse(rom, s, e)
        open(os.path.join(out, m['name'] + '.bin'), 'wb').write(m['data'])
        line = '%s 0x%06X-0x%06X member_end=0x%06X tail=%d(zero=%s) mtime=%d xfl=%d os=%d inflated=%d' % (
            m['name'], s, e, m['member_end'], m['tail_len'], m['tail_zero'], m['mtime'], m['xfl'], m['os'], len(m['data']))
        if check:
            c = zlib.crc32(m['data']) == m['crc'] and len(m['data']) & 0xffffffff == m['isize']
            ok &= c
            rep = [lv for lv in range(1, 10) for strat in [0]
                   if (lambda co: co.compress(m['data']) + co.flush())(zlib.compressobj(lv, 8, -15)) == m['deflate']]
            line += ' crc_isize=%s zlib_reproduces=%s' % ('OK' if c else 'BAD', rep or 'none')
        print(line)
    if check: print('ALL CRC OK' if ok else 'CRC FAILURE'); sys.exit(0 if ok else 1)
main()
