#!/usr/bin/env python3
"""asmfilter.py MODE IN OUT: the text step between KMC GCC -S and the assembler, for two lanes.
  builtin  normalize.builtin (KMC GCC output made deterministic for KMC GNU as)
  asn64    asn64.convert (ASN64 behaviour reproduced for modern GNU as)"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
mode, src, dst = sys.argv[1:4]
text = open(src, errors='replace').read()
if mode == 'builtin':
    import normalize; out = normalize.builtin(text)
elif mode == 'asn64':
    import asn64; out = asn64.convert(text)
else: sys.exit('unknown mode ' + mode)
open(dst, 'w').write(out)
