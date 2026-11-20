#include "common.h"

/* __assert: prints an assertion fault message */
 /* "\nASSERTION FAULT: %s, %d: \"%s\"\n" */
void osSyncPrintf(const char *fmt, ...); /* osSyncPrintf */
void func_80059540(const char *exp, const char *filename, int line) {
    osSyncPrintf(D_8007B890, filename, line, exp);
}
