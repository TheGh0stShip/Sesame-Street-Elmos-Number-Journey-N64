/* _Ldunscale: split a double into exponent and normalised mantissa */
typedef long double ldouble;
short func_80068CD0(short *pex, ldouble *px) {
    unsigned short *ps = (unsigned short *)px;
    short xchar = (ps[0] & 0x7FF0) >> 4;
    if (xchar == 0x7FF) {
        *pex = 0;
        return (ps[0] & 0xF) || ps[1] || ps[2] || ps[3] ? 2 : 1;
    } else if (xchar > 0) {
        ps[0] = (ps[0] & ~0x7FF0) | 0x3FF0;
        *pex = xchar - 0x3FE;
        return -1;
    } else if (xchar < 0) {
        return 2;
    } else {
        *pex = 0;
        return 0;
    }
}
