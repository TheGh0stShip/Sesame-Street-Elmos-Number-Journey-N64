#include "common.h"

/* overlap-safe byte copy (memmove-style): copy n bytes from src to dst */
void func_800529BC(u8 *dst, u8 *src, s32 n) {
    if (src < dst) {
        dst += n;
        src += n;
        for (n--; n != -1; n--) {
            *--dst = *--src;
        }
    } else {
        for (n--; n != -1; n--) {
            *dst++ = *src++;
        }
    }
}
