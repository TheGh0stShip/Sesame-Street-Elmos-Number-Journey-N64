/* _Litob (xlitob.c): format an integer conversion (d i o u x X) for printf */
typedef unsigned int size_t;
typedef struct {
    union { long long ll; long double ld; } v;
    unsigned char *s;
    int n0; int nz0; int n1; int nz1; int n2; int nz2;
    int prec; int width; size_t nchar; unsigned int flags; char qual;
} _Pft;
typedef struct lldiv_t { long long quot; long long rem; } lldiv_t;

lldiv_t func_80069394(long long num, long long denom); /* lldiv */
void *func_80060E7C(void *dst, const void *src, size_t n); /* memcpy */
extern char D_80078FE0[]; /* ldigs "0123456789abcdef" */
extern char D_80078FF4[]; /* udigs "0123456789ABCDEF" */

#define BUFF_LEN 0x18
#define FLAGS_MINUS 4
#define FLAGS_ZERO 16

void func_80068640(_Pft *px, unsigned char code)
{
    char buff[BUFF_LEN];
    const char *digs;
    int base;
    int i;
    unsigned long long ullval;

    digs = (code == 'X') ? D_80078FF4 : D_80078FE0;

    base = (code == 'o') ? 8 : ((code != 'x' && code != 'X') ? 10 : 16);
    i = BUFF_LEN;
    ullval = px->v.ll;

    if ((code == 'd' || code == 'i') && px->v.ll < 0) {
        ullval = -ullval;
    }

    if (ullval != 0 || px->prec != 0) {
        buff[--i] = digs[ullval % base];
    }

    px->v.ll = ullval / base;

    while (px->v.ll > 0 && i > 0) {
        lldiv_t qr = func_80069394(px->v.ll, base);

        px->v.ll = qr.quot;
        buff[--i] = digs[qr.rem];
    }

    px->n1 = BUFF_LEN - i;

    func_80060E7C(px->s, buff + i, px->n1);

    if (px->n1 < px->prec) {
        px->nz0 = px->prec - px->n1;
    }

    if (px->prec < 0 && (px->flags & (FLAGS_ZERO | FLAGS_MINUS)) == FLAGS_ZERO) {
        if ((i = px->width - px->n0 - px->nz0 - px->n1) > 0) {
            px->nz0 += i;
        }
    }
}
