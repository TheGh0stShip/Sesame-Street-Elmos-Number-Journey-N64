/* guScaleF */
void func_80057040(float mf[4][4]); /* guMtxIdentF */
void func_80057D90(float mf[4][4], float x, float y, float z) {
    func_80057040(mf);
    mf[0][0] = x;
    mf[1][1] = y;
    mf[2][2] = z;
    mf[3][3] = 1;
}
