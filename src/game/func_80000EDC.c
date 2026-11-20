#include "common.h"

/* copies n words from src to dst */
void func_80000EDC(u32 *dst, u32 *src, u32 n) {
    u32 i;
    for (i = 0; i < n; i++) {
        *dst++ = *src++;
    }
}
