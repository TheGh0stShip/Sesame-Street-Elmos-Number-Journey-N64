/* guMtxIdent: load identity into fixed-point Mtx */
typedef union { long m[4][4]; long long force_structure_alignment; } Mtx;
#define FTOFIX32(x) (long)((x) * (float)0x00010000)
static void guMtxF2L(float mf[4][4], Mtx *m) {
    int i, j;
    int e1, e2;
    int *ai, *af;
    ai = (int *)&m->m[0][0];
    af = (int *)&m->m[2][0];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 2; j++) {
            e1 = FTOFIX32(mf[i][j * 2]);
            e2 = FTOFIX32(mf[i][j * 2 + 1]);
            *(ai++) = (e1 & 0xffff0000) | ((e2 >> 16) & 0xffff);
            *(af++) = ((e1 << 16) & 0xffff0000) | (e2 & 0xffff);
        }
}
static void guMtxIdentF(float mf[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (i == j) mf[i][j] = 1.0;
            else mf[i][j] = 0.0;
}
void func_80057090(Mtx *m) {
    float mf[4][4];
    guMtxIdentF(mf);
    guMtxF2L(mf, m);
}
