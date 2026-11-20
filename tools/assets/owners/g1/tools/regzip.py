"""regzip.py RAWFILE NAME -> writes the gzip member bytes to stdout exactly as the game's packer made them:
GNU gzip 1.2.4 built with -DSMALL_MEM (the 16-bit MS-DOS configuration), default level 6, original file name kept,
mtime 0, XFL 0, OS byte 0 (FAT). The binary is built by tools/build124.sh from the public GNU source (not shipped).
Function regzip(raw: bytes, name: str) -> bytes for import."""
import sys, os, subprocess, struct, zlib, tempfile
S = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); BIN = S + '/bin/gzip124_SMALL_MEM'
def regzip(raw, name):
    d = tempfile.mkdtemp(dir=S + '/out'); p = os.path.join(d, name); open(p, 'wb').write(raw)
    out = subprocess.run([BIN, '-6', '-c', p], capture_output=True, check=True).stdout
    q = out.find(b'\0', 10) + 1 if out[3] & 8 else 10
    return b'\x1f\x8b\x08\x08' + struct.pack('<I', 0) + b'\x00\x00' + name.encode() + b'\0' + out[q:-8] + struct.pack('<II', zlib.crc32(raw) & 0xFFFFFFFF, len(raw) & 0xFFFFFFFF)
if __name__ == '__main__': sys.stdout.buffer.write(regzip(open(sys.argv[1], 'rb').read(), sys.argv[2]))
