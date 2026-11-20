typedef s32 Mtx[16];
void func_80056F98(f32 mf[4][4], Mtx *m);  /* guMtxL2F */
void func_80057180(f32 a[4][4], f32 b[4][4], f32 r[4][4]); /* guMtxCatF */
void func_80056F00(f32 mf[4][4], Mtx *m);  /* guMtxF2L */
/* guMtxCatL */
void func_80057310(Mtx *m, Mtx *n, Mtx *res) {
    f32 mf[4][4], nf[4][4], resf[4][4];
    func_80056F98(mf, m);
    func_80056F98(nf, n);
    func_80057180(mf, nf, resf);
    func_80056F00(resf, res);
}
