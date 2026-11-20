/* SDK libc/string.c */
#include "PR/ultratypes.h"
#include "string.h"

// TODO: this comes from a header
#ident "$Revision: 1.23 $"

char* strchr(const char* s, int c);
size_t strlen(const char* s);

void* memcpy(void* s1, const void* s2, size_t n) {
    char* su1 = (char*)s1;
    const char* su2 = (const char*)s2;
    while (n > 0) {
        *su1 = *su2;
        su1++;
        su2++;
        n--;
    }
    return (void*)s1;
}
