"""Pull the display-list words that the matched C code writes with D_8013EB0C++ (gfx command builders).
Only for analysis: the extractor itself does not need the sources (results are frozen into gfx_tables.py)."""
import re, os

W0 = re.compile(r'^\s*(?:\(\*\(s32 \*\)\w+\)|\w+->w0)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;')
W1 = re.compile(r'^\s*(?:\(\(struct gd_4_s32 \*\)\w+\)->v|\w+->w1)\s*=\s*(.+);')

def pairs(path):
    out = []; w0 = None
    for ln, line in enumerate(open(path), 1):
        m = W0.match(line)
        if m:
            w0 = int(m.group(1), 0); continue
        m = W1.match(line)
        if m and w0 is not None:
            e = m.group(1).strip()
            sym = re.search(r'D_(80[0-9A-F]{6})', e)
            if re.fullmatch(r'-?(0x[0-9A-Fa-f]+|\d+)', e):
                w1 = int(e, 0) & 0xFFFFFFFF; kind = 'const'
            elif sym and re.fullmatch(r'\(s32\)&?D_80[0-9A-F]{6}|\(u32\)D_80[0-9A-F]{6}|&?D_80[0-9A-F]{6}|\(s32\)D_80[0-9A-F]{6}', e):
                w1 = int(sym.group(1), 16); kind = 'sym'
            else:
                w1 = None; kind = e
            out.append((ln, w0, w1, kind))
            w0 = None
    return out

if __name__ == '__main__':
    import sys
    sys.path.insert(0, os.path.dirname(__file__))
    import f3dex
    d = sys.argv[1]
    for f in sorted(os.listdir(d)):
        p = pairs(os.path.join(d, f))
        if not any((w0 >> 24) == 0xFD for _, w0, _, _ in p):
            continue
        print('##', f)
        for ln, w0, w1, k in p:
            if (w0 >> 24) in (0xFD, 0xF5, 0xF3, 0xF4, 0xF0, 0xF2, 0x06):
                print('  %5d %s %s' % (ln, f3dex.disasm(w0, w1 if w1 is not None else 0) if w1 is not None else '%08X <%s>' % (w0, k), ''))
