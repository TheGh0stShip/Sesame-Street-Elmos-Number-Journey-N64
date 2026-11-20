import sys, zlib, os
rom, tsv, outd = sys.argv[1:4]
d = open(rom, 'rb').read(); os.makedirs(outd, exist_ok=True)
for l in open(tsv):
    f = l.rstrip('\n').split('\t')
    if len(f) > 4 and f[4] == 'gzip_level':
        s, e = int(f[1], 16), int(f[2], 16)
        name = f[5].split()[-1]
        o = zlib.decompressobj(31); data = o.decompress(d[s:e])
        open(os.path.join(outd, name), 'wb').write(data)
        print(name, hex(s), len(data), len(o.unused_data))
