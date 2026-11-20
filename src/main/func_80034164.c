/* builds a puzzle row: places the answer symbol at random gaps and fills the rest with distinct symbols */
typedef struct {
    u8 c[0x20];
    s32 v[32];
} Row;
s32 func_800217C4(s32);
Row *func_80042080(s32);
void func_80069C70(void *, s32, s32);
extern s32 D_80076A6C[][20];
extern u8 D_800A7F24;
extern s32 D_800A9154;
extern s32 D_800A93A4;
extern u8 D_800BD868[20];
extern u8 D_80106C70;
extern s32 D_80106C9C;
extern s32 D_80108D20;
extern u8 D_8012FE90[];
extern s32 D_8013ECF8;

Row *func_80034164(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    s32 j;
    s32 k;
    s32 unused;
    s32 r;
    s32 count;
    register Row *buf;

    D_8013ECF8 = 0;
    D_80108D20 = arg3;
    D_800A7F24 = D_8012FE90[D_80108D20];
    count = 0;
    buf = func_80042080(0xA0);
    if (buf == NULL) {
        return NULL;
    }
    func_80069C70(buf, 0, 0xA0);
    for (i = 0; i < arg0 + 2; i++) {
        buf->c[i] = 0;
        buf->v[i] = -1;
    }
    buf->c[arg0] = D_800A7F24;
    D_800A9154 = arg1;
    D_800A93A4 = 0;
    arg2 = 0;
    if (arg2 == 0) {
        for (i = 0; i < 20; i++) {
            D_800BD868[i] = 0;
        }
    }
    k = func_800217C4(20);
    i = 0;
    j = 1;
    for (; i < D_800A9154 - 1; i++) {
        if (k >= 20) {
            k = 0;
        }
        j += D_80076A6C[D_80106C70][k++];
        if (j >= arg0) {
            j = 1;
        }
        while (buf->c[j - 1] != 0 || buf->c[j] != 0 || buf->c[j + 1] != 0) {
            j++;
            if (j >= arg0) {
                j = 1;
            }
        }
        buf->c[j] = D_800A7F24;
    }
    j = 0;
    for (i = 1; i <= arg0; i++) {
        if (buf->c[i] == 0) {
            r = func_800217C4(D_80106C9C);
            if (r == D_80108D20) {
                r = (r + 1) % D_80106C9C;
            }
            if (buf->c[i - 1] == D_8012FE90[r]) {
                r = (r + 1) % D_80106C9C;
                if (r == D_80108D20) {
                    r = (r + 1) % D_80106C9C;
                }
            }
            if (buf->c[i + 1] == D_8012FE90[r]) {
                r = (r + 1) % D_80106C9C;
                if (r == D_80108D20) {
                    r = (r + 1) % D_80106C9C;
                }
            }
            buf->c[i] = D_8012FE90[r];
        } else {
            buf->v[i] = arg2 + j++;
        }
    }
    return buf;
}
