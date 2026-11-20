import sys, os, glob, zlib, subprocess, struct, tempfile
L = os.path.expanduser('${N64DK_LAB}'); S = L + '/sesassets'
for g, name in (('letter', 'Letter'), ('number', 'Number')):
    rom = open(glob.glob(L + '/roms/*%s*' % name)[0], 'rb').read(); o = 0; n = 0
    while True:
        o = rom.find(b'\x1f\x8b\x08', o)
        if o < 0: break
        flg, mtime, xfl, osb = rom[o+3], struct.unpack('<I', rom[o+4:o+8])[0], rom[o+8], rom[o+9]
        p = o + 10; fname = b''
        if flg & 8:
            e = rom.index(b'\0', p); fname = rom[p:e]; p = e + 1
        d = zlib.decompressobj(-15)
        try: raw = d.decompress(rom[p:p + 400000]); used = len(rom[p:p+400000]) - len(d.unused_data)
        except Exception: o += 1; continue
        if len(raw) < 100000: o += 1; continue
        stream = rom[p:p + used]; res = []
        with tempfile.NamedTemporaryFile(dir=S + '/g1/out', delete=False) as f: f.write(raw); tmp = f.name
        for lv in range(1, 10):
            out = subprocess.run(['gzip', '-n', '-%d' % lv, '-c', tmp], capture_output=True).stdout
            if out[10:10 + len(stream)] == stream: res.append('gzip-%d' % lv)
            elif len(out) - 18 == len(stream): res.append('gzip-%d:samesize' % lv)
        print(g, '0x%X' % o, fname.decode('latin1'), 'flg', flg, 'mtime', mtime, 'xfl', xfl, 'os', osb, 'raw', len(raw), 'deflate', len(stream), 'first-byte %02X' % stream[0], res or 'no gzip level matches')
        o = p + used; n += 1
