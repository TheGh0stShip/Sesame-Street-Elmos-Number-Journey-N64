#include "common.h"

/* memset */
void *memset(void *dst, s32 c, u32 n) {
    u8 *p;
    u32 w;
    if (n != 0) {
        c &= 0xFF;
        c |= c << 8;
        c |= c << 16;
        p = dst;
        if ((u32)dst & 1) {
            *p++ = c;
            n--;
        }
        w = n >> 2;
        if (n >= 2 && ((u32)p & 2)) {
            *(u16 *)p = c;
            p += 2;
            n -= 2;
            w = n >> 2;
        }
        while (w-- != 0) {
            *(u32 *)p = c;
            p += 4;
        }
        if (n & 2) {
            *(u16 *)p = c;
            p += 2;
        }
        if (n & 1) {
            *p = c;
        }
    }
    return dst;
}
