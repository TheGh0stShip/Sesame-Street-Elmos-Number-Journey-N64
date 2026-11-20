#include "common.h"

/* _Printf (libultra xprintf.c): core printf engine writing through the output callback pfn */

#define __va_rounded_size(__TYPE) (((sizeof(__TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_arg(__AP, __type)                                                                      \
    ((__AP = (char *)((__alignof__(__type) > 4 ? ((int)__AP + 8 - 1) & -8 : ((int)__AP + 4 - 1) & -4) \
                      + __va_rounded_size(__type))),                                              \
     *(__type *)(void *)(__AP - __va_rounded_size(__type)))
typedef double ldouble; /* long double is 8 bytes in this build */
typedef struct {
    /* 0x0 */ union { long long ll; ldouble ld; } v;
    /* 0x8 */ unsigned char *s;
    /* 0xC */ int n0;
    /* 0x10 */ int nz0;
    /* 0x14 */ int n1;
    /* 0x18 */ int nz1;
    /* 0x1C */ int n2;
    /* 0x20 */ int nz2;
    /* 0x24 */ int prec;
    /* 0x28 */ int width;
    /* 0x2C */ size_t nchar;
    /* 0x30 */ unsigned int flags;
    /* 0x34 */ char qual;
} _Pft;
#define FLAGS_SPACE 1
#define FLAGS_PLUS 2
#define FLAGS_MINUS 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16
#define isdigit(x) ((x >= '0' && x <= '9'))
#define ATOI(dst, src)                   \
    for (dst = 0; isdigit(*src); ++src) { \
        if (dst < 999)                   \
            dst = dst * 10 + *src - '0'; \
    }
#define MAX_PAD ((sizeof(D_80078F90) - 1))
#define PAD(s, n)                                             \
    if (0 < (n)) {                                            \
        int i, j = (n);                                       \
        for (; 0 < j; j -= i) {                               \
            i = MAX_PAD < (unsigned int)j ? (int)MAX_PAD : j; \
            PUT(s, i);                                        \
        }                                                     \
    }
#define PUT(s, n)                                \
    if (0 < (n)) {                               \
        if ((arg = (*pfn)(arg, s, n)) != NULL)   \
            x.nchar += (n);                      \
        else                                     \
            return x.nchar;                      \
    }
/* spaces */
/* zeroes */
/* fchar: " +-#0" */
/* fbit */
         /* "hlL" */
 /* strchr */
void _Putfld(_Pft *px, va_list *pap, char code, char *ac); /* _Putfld */

int _Printf(void *pfn(void *, const char *, size_t), void *arg, const char *fmt, va_list ap) {
    _Pft x;

    x.nchar = 0;

    while (1) {
        const char *s;
        char c;
        const char *t;
        char ac[32];
        s = fmt;

        for (c = *s; c != 0 && c != '%';) {
            c = *++s;
        }

        PUT(fmt, s - fmt);

        if (c == 0) {
            return x.nchar;
        }

        fmt = ++s;

        for (x.flags = 0; (t = strchr(D_8007BBD0, *s)) != NULL; s++) {
            x.flags |= D_8007BBD8[t - D_8007BBD0];
        }

        if (*s == '*') {
            x.width = va_arg(ap, int);
            if (x.width < 0) {
                x.width = -x.width;
                x.flags |= FLAGS_MINUS;
            }
            s++;
        } else
            ATOI(x.width, s);

        if (*s != '.') {
            x.prec = -1;
        } else if (*++s == '*') {
            x.prec = va_arg(ap, int);
            ++s;
        } else
            for (x.prec = 0; isdigit(*s); s++) {
                if (x.prec < 999)
                    x.prec = x.prec * 10 + *s - '0';
            }

        x.qual = strchr(D_8007BBF0, *s) ? *s++ : '\0';

        if (x.qual == 'l' && *s == 'l') {
            x.qual = 'L';
            ++s;
        }

        _Putfld(&x, &ap, *s, ac);
        x.width -= x.n0 + x.nz0 + x.n1 + x.nz1 + x.n2 + x.nz2;

        {
            if (!(x.flags & FLAGS_MINUS)) {
                int i, j;
                if (0 < (x.width)) {
                    i, j = x.width;
                    for (; 0 < j; j -= i) {
                        i = MAX_PAD < (unsigned int)j ? (int)MAX_PAD : j;
                        PUT(D_80078F90, i);
                    }
                }
            }

            PUT(ac, x.n0);
            PAD(D_80078FB4, x.nz0)

            PUT(x.s, x.n1);
            PAD(D_80078FB4, x.nz1);

            PUT(x.s + x.n1, x.n2);
            PAD(D_80078FB4, x.nz2);

            if (x.flags & FLAGS_MINUS) {
                PAD(D_80078F90, x.width);
            }
        }
        fmt = s + 1;
    }
    return 0;
}
