/* _Putfld (libultra xprintf.c): formats one printf conversion into the _Pft fields */
typedef unsigned int size_t;
typedef char *va_list;
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
#define LDSIGN(x) (((unsigned short *)&(x))[0] & 0x8000)
void func_80068640(_Pft *px, char code); /* _Litob */
void func_80068890(_Pft *px, char code); /* _Ldtob */
size_t func_80060E58(const char *s);      /* strlen */

void func_80067E80(_Pft *px, va_list *pap, char code, char *ac) {
    px->n0 = px->nz0 = px->n1 = px->nz1 = px->n2 = px->nz2 = 0;

    switch (code) {
        case 'c':
            ac[px->n0++] = va_arg(*pap, int);
            break;
        case 'd':
        case 'i':
            if (px->qual == 'l') {
                px->v.ll = va_arg(*pap, long);
            } else if (px->qual == 'L') {
                px->v.ll = va_arg(*pap, long long);
            } else {
                px->v.ll = va_arg(*pap, int);
            }
            if (px->qual == 'h') {
                px->v.ll = (short)px->v.ll;
            }
            if (px->v.ll < 0) {
                ac[px->n0++] = '-';
            } else if (px->flags & FLAGS_PLUS) {
                ac[px->n0++] = '+';
            } else if (px->flags & FLAGS_SPACE) {
                ac[px->n0++] = ' ';
            }
            px->s = (unsigned char *)&ac[px->n0];
            func_80068640(px, code);
            break;
        case 'x':
        case 'X':
        case 'u':
        case 'o':
            if (px->qual == 'l') {
                px->v.ll = va_arg(*pap, long);
            } else if (px->qual == 'L') {
                px->v.ll = va_arg(*pap, long long);
            } else {
                px->v.ll = va_arg(*pap, int);
            }
            if (px->qual == 'h') {
                px->v.ll = (unsigned short)px->v.ll;
            } else if (px->qual == 0) {
                px->v.ll = (unsigned int)px->v.ll;
            }
            if (px->flags & FLAGS_HASH) {
                ac[px->n0++] = '0';
                if (code == 'x' || code == 'X') {
                    ac[px->n0++] = code;
                }
            }
            px->s = (unsigned char *)&ac[px->n0];
            func_80068640(px, code);
            break;
        case 'e':
        case 'f':
        case 'g':
        case 'E':
        case 'G':
            px->v.ld = px->qual == 'L' ? va_arg(*pap, ldouble) : va_arg(*pap, double);
            if (LDSIGN(px->v.ld))
                ac[px->n0++] = '-';
            else if (px->flags & FLAGS_PLUS)
                ac[px->n0++] = '+';
            else if (px->flags & FLAGS_SPACE)
                ac[px->n0++] = ' ';
            px->s = (unsigned char *)&ac[px->n0];
            func_80068890(px, code);
            break;
        case 'n':
            if (px->qual == 'h') {
                *va_arg(*pap, unsigned short *) = px->nchar;
            } else if (px->qual == 'l') {
                *va_arg(*pap, unsigned long *) = px->nchar;
            } else if (px->qual == 'L') {
                *va_arg(*pap, unsigned long long *) = px->nchar;
            } else {
                *va_arg(*pap, unsigned int *) = px->nchar;
            }
            break;
        case 'p':
            px->v.ll = (long)va_arg(*pap, void *);
            px->s = (unsigned char *)&ac[px->n0];
            func_80068640(px, 'x');
            break;
        case 's':
            px->s = va_arg(*pap, unsigned char *);
            px->n1 = func_80060E58((char *)px->s);
            if (px->prec >= 0 && px->prec < px->n1) {
                px->n1 = px->prec;
            }
            break;
        case '%':
            ac[px->n0++] = '%';
            break;
        default:
            ac[px->n0++] = code;
            break;
    }
}
