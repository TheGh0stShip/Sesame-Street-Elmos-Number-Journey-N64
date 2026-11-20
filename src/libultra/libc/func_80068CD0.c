/* SDK libc/xldtob.c */
#include "stdlib.h"
#include "string.h"
#include "xstdio.h"

// TODO: these come from headers
#ident "$Revision: 1.23 $"
#ident "$Revision: 1.34 $"
#ident "$Revision: 1.5 $"

#define BUFF_LEN 0x20

short _Ldunscale(short* pex, ldouble* px);
void _Genld(_Pft* px, char code, unsigned char* p, short nsig, short xexp);
extern const ldouble pows[];

// float properties
#define _D0 0
#define _DBIAS 0x3ff
#define _DLONG 1
#define _DOFF 4
#define _FBIAS 0x7e
#define _FOFF 7
#define _FRND 1
#define _LBIAS 0x3ffe
#define _LOFF 15
// integer properties
#define _C2 1
#define _CSIGN 1
#define _ILONG 0
#define _MBMAX 8
#define NAN 2
#define INF 1
#define FINITE -1
#define _DFRAC ((1 << _DOFF) - 1)
#define _DMASK (0x7fff & ~_DFRAC)
#define _DMAX ((1 << (15 - _DOFF)) - 1)
#define _DNAN (0x8000 | _DMAX << _DOFF | 1 << (_DOFF - 1))
#define _DSIGN 0x8000
#define _D1 1 // big-endian order
#define _D2 2
#define _D3 3

#define ALIGN(s, align) (((unsigned int)(s) + ((align)-1)) & ~((align)-1))

void _Ldtob(_Pft* px, char code);

short _Ldunscale(short* pex, ldouble* px) {
    unsigned short* ps = (unsigned short*)px;
    short xchar = (ps[_D0] & _DMASK) >> _DOFF;


    if (xchar == _DMAX) {
        *pex = 0;

        return (ps[_D0] & _DFRAC) || ps[_D1] || ps[_D2] || ps[_D3] ? 2 : 1;
    } else if (xchar > 0) {
        ps[_D0] = (ps[_D0] & ~_DMASK) | 0x3FF0;
        *pex = xchar - 0x3FE;
        return -1;
    } else if (xchar < 0) {
        return 2;
    } else {
        *pex = 0;
        return 0;
    }
}
void _Genld(_Pft* px, char code, unsigned char* p, short nsig, short xexp);

