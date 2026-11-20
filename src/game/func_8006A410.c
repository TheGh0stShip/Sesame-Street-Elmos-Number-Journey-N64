#include "common.h"

/* strcpy: byte copy if src/dst alignments differ, else align dst then copy a word at a time (big-endian null test) */
char *strcpy(char *dst, const char *src) {
    char *d = dst;
    u32 w;

    if ((src - dst) & 3) {
        while ((*d++ = *src++) != 0) ;
        return dst;
    }
    while ((u32)d & 3) { if ((*d++ = *src++) == 0) return dst; }
    while (1) {
        w = *(const u32 *)src;
        src += 4;
        if (!(w & 0xFF000000)) { *d = 0; return dst; }
        if (!(w & 0xFF0000)) { *(u16 *)d = w >> 16; return dst; }
        if (!(w & 0xFF00)) { *(u16 *)d = w >> 16; d[2] = 0; return dst; }
        *(u32 *)d = w;
        d += 4;
        if (!(w & 0xFF)) return dst;
    }
}
