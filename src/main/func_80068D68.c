/* _Genld: emit digits, point and exponent for _Ldtob */
typedef long double ldouble;
typedef unsigned int size_t;
typedef struct { long quot; long rem; } ldiv_t;
typedef struct {
    union { long long ll; ldouble ld; } v;
    unsigned char *s;
    int n0, nz0, n1, nz1, n2, nz2, prec, width;
    size_t nchar;
    unsigned int flags;
    u8 qual;
} _Pft;
void *func_80060E7C(void *, const void *, size_t); /* memcpy */
#define memcpy func_80060E7C

void func_80068D68(_Pft* px, u8 code, unsigned char* p, short nsig, short xexp) {
    const unsigned char point = '.';

    if (nsig <= 0) {
        nsig = 1, p = "0";
    }

    if (code == 'f' || (code == 'g' || code == 'G') && xexp >= -4 && xexp < px->prec) {
        xexp++;
        if (code != 'f') {
            if (((px->flags & 8) == 0) && nsig < px->prec) {
                px->prec = nsig;
            }

            if ((px->prec -= xexp) < 0) {
                px->prec = 0;
            }
        }

        if (xexp <= 0) {
            px->s[px->n1++] = '0';

            if (px->prec > 0 || (px->flags & 8)) {
                px->s[px->n1++] = point;
            }

            if (px->prec < -xexp) {
                xexp = -px->prec;
            }

            px->nz1 = -xexp;
            px->prec += xexp;

            if (px->prec < nsig) {
                nsig = px->prec;
            }

            memcpy(&px->s[px->n1], p, px->n2 = nsig);
            px->nz2 = px->prec - nsig;
        } else if (nsig < xexp) {
            memcpy(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = xexp - nsig;
            if (px->prec > 0 || (px->flags & 8)) {
                px->s[px->n1] = point;
                px->n2++;
            }

            px->nz2 = px->prec;
        } else {
            memcpy(&px->s[px->n1], p, xexp);
            px->n1 += xexp;
            nsig -= xexp;

            if (px->prec > 0 || (px->flags & 8)) {
                px->s[px->n1++] = point;
            }

            if (px->prec < nsig) {
                nsig = px->prec;
            }

            memcpy(&px->s[px->n1], &p[xexp], nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }
    } else {
        if (code == 'g' || code == 'G') {
            if (nsig < px->prec) {
                px->prec = nsig;
            }

            if (--px->prec < 0) {
                px->prec = 0;
            }

            code = (code == 'g') ? 'e' : 'E';
        }

        px->s[px->n1++] = *p++;

        if (px->prec > 0 || (px->flags & 8)) {
            px->s[px->n1++] = point;
        }

        if (px->prec > 0) {
            if (px->prec < --nsig) {
                nsig = px->prec;
            }

            memcpy(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }

        p = &px->s[px->n1];
        *p++ = code;

        if (xexp >= 0) {
            *p++ = '+';
        } else {
            *p++ = '-';
            xexp = -xexp;
        }

        if (xexp >= 100) {
            if (xexp >= 1000) {
                *p++ = (xexp / 1000) + '0', xexp %= 1000;
            }
            *p++ = (xexp / 100) + '0', xexp %= 100;
        }
        *p++ = (xexp / 10) + '0', xexp %= 10;

        *p++ = xexp + '0';
        px->n2 = (size_t)p - ((size_t)px->s + px->n1);
    }

    if ((px->flags & 0x14) == 0x10) {
        int n = px->n0 + px->n1 + px->nz1 + px->n2 + px->nz2;

        if (n < px->width) {
            px->nz0 = px->width - n;
        }
    }
}
