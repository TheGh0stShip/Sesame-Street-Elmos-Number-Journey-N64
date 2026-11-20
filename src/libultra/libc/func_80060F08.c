/* SDK libc/sprintf.c */
#include "xstdio.h"
#include "string.h"
#include "os.h"

// TODO: this comes from a header
#ident "$Revision: 1.23 $"

void* proutSprintf(void* s, const char* buf, size_t n);
int sprintf(char* s, const char* fmt, ...);
void* proutSprintf(void* s, const char* buf, size_t n) {
    return (char*)memcpy(s, buf, n) + n;
}
