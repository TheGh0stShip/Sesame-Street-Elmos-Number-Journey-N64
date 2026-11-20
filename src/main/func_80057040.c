/* guMtxIdentF */
void func_80057040(f32 mf[4][4]) {
    s32 i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (i == j) mf[i][j] = 1.0f;
            else mf[i][j] = 0.0f;
}
