/* osSyncPrintf: formats through _Printf when a print hook is installed */
typedef char *va_list; /* gcc 2.7 va-mips.h */
#define va_start(ap, last) (ap = (va_list)__builtin_next_arg(last))
extern void *D_800A8F80;
void *func_80060F40(void *, const char *, u32);
s32 func_80067910(void *(*prout)(void *, const char *, u32), void *arg, const char *fmt, va_list ap); /* _Printf */
void func_800611DC(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if (D_800A8F80 != NULL) {
        func_80067910(func_80060F40, NULL, fmt, ap);
    }
}
