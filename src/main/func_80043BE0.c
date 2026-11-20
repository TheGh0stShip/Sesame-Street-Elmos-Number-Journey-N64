extern s32 D_8007B300;
extern s32 D_8007B304;
void func_80059540(void *, void *, s32);

/* 4x4 matrix multiply: dst = a * b, warns if dst aliases an input */
void func_80043BE0(f32 dst[4][4], f32 a[4][4], f32 b[4][4]) {
    s32 i;
    s32 j;

    if (dst != a) {
    } else {
        func_80059540(&D_8007B300, &D_8007B304, 16);
    }
    if (dst != b) {
    } else {
        func_80059540(&D_8007B300, &D_8007B304, 17);
    }
    for (i = 0; i < 4; i++) {
        j = 0;
        while (j < 4) {
            dst[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j] + a[i][3] * b[3][j];
            j++;
        }
    }
}
