/* _Ldtob: format a double for printf (%e %f %g) */
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
#define memcpy __builtin_memcpy

ldiv_t func_80069310(long, long); /* ldiv */
#define ldiv func_80069310
short func_80068CD0(short *pex, ldouble *px); /* _Ldunscale */
void func_80068D68(_Pft *px, u8 code, unsigned char *p, short nsig, short xexp); /* _Genld */
#define _Ldunscale func_80068CD0
#define _Genld func_80068D68
#define BUFF_LEN 0x20
#define ALIGN(s, align) (((unsigned int)(s) + ((align)-1)) & ~((align)-1))
static const ldouble pows[] = {10e0L, 10e1L, 10e3L, 10e7L, 10e15L, 10e31L, 10e63L, 10e127L, 10e255L};
void func_80068890(_Pft* px, u8 code) {
    u8 buff[BUFF_LEN];
    u8 *p;
    ldouble ldval;
    short err;
    short nsig;
    short xexp;

    p = buff;
    ldval = px->v.ld;

    if (px->prec < 0) {
        px->prec = 6;
    } else if (px->prec == 0 && (code == 'g' || code == 'G')) {
        px->prec = 1;
    }

    err = _Ldunscale(&xexp, &px->v.ld);
    if (err > 0) {
        memcpy(px->s, err == 2 ? "NaN" : "Inf", px->n1 = 3);
        return;
    } else if (err == 0) {
        nsig = 0;
        xexp = 0;
    } else {
        {
            int i;
            int n;

            if (ldval < 0) {
                ldval = -ldval;
            }

            // what
            if ((xexp = xexp * 30103 / 100000 - 4) < 0) {
                n = ALIGN(-xexp, 4), xexp = -n;

                for (i = 0; n > 0; n >>= 1, i++) {
                    if (n & 1) {
                        ldval *= pows[i];
                    }
                }
            } else if (xexp > 0) {
                ldouble factor = 1;
                
                xexp &= ~3;
                
                for (n = xexp, i = 0; n > 0; n >>= 1, i++) {
                    if (n & 1) {
                        factor *= pows[i];
                    }
                }

                ldval /= factor;
            }
        }
        {
            int gen = px->prec + ((code == 'f') ? 10 + xexp : 6);
            
            if (gen > 0x13) {
                gen = 0x13;
            }
            
            for (*p++ = '0'; gen > 0 && 0 < ldval; p += 8) {
                int j;
                long lo = ldval;
                
                if ((gen -= 8) > 0) {
                    ldval = (ldval - lo) * 1e8;
                }

                for (p += 8, j = 8; lo > 0 && --j >= 0;) {
                    ldiv_t qr;
                    qr = ldiv(lo, 10);
                    *--p = qr.rem + '0', lo = qr.quot;
                }
                
                while (--j >= 0) {
                    *--p = '0';
                }
            }

            gen = p - &buff[1];

            for (p = &buff[1], xexp += 7; *p == '0'; p++) {
                --gen, --xexp;
            }

            nsig = px->prec + ((code == 'f') ? xexp + 1 : ((code == 'e' || code == 'E') ? 1 : 0));
            
            if (gen < nsig) {
                nsig = gen;
            }

            if (nsig > 0) {
                const u8 drop = nsig < gen && '5' <= p[nsig] ? '9' : '0';
                int n;

                for (n = nsig; p[--n] == drop;) {
                    --nsig;
                }

                if (drop == '9') {
                    ++p[n];
                }
                
                if (n < 0) {
                    --p, ++nsig, ++xexp;
                }
            }
        }
    }

    _Genld(px, code, p, nsig, xexp);
}
