/* SDK libc/string.c */
#include "PR/ultratypes.h"
#include "string.h"

// TODO: this comes from a header
#ident "$Revision: 1.23 $"

char* strchr(const char* s, int c);

size_t strlen(const char* s) {
    const char* sc = s;
    while (*sc != 0) {
        sc++;
    }
    return sc - s;
}
void* memcpy(void* s1, const void* s2, size_t n);
