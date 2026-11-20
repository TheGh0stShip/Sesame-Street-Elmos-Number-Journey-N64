typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct Actor {
    u8 unk_000[0x24]; f32 pos[3]; f32 fwd[3]; u8 unk_03C[0x224 - 0x3C]; u16 unk_224;
} Actor;

void func_8000EDB0(char *);
s32 func_80060EB0(char *, const char *, ...);
f64 func_80069920(f64, f64);
extern s32 D_800717E0;
extern char D_80079D2C[];
extern char D_80079D40[];
extern char D_80079D58[];
extern char D_80079D60[];
extern char D_80079D68[];
extern char D_80079D70[];
extern char D_80079D78[];
extern char D_80079D88[];
extern char D_80079D94[];
extern char D_80079DB0[];
extern char D_80079DB8[];
extern char D_80079DC0[];
extern char D_80079DC8[];
extern char D_80079DD0[];
extern char D_80079DD8[];
extern char D_80079DE8[];
extern char D_80079DF4[];
extern u64 D_800A7A68;
extern s8 D_800A9140;
extern f32 D_800A9150;
extern f32 D_800AD000;
extern u32 *D_800AD028;
extern u16 D_800B1092;
extern s8 D_80106C6C;
extern f32 D_80106C78;
extern Actor *D_80106C84;
extern u32 D_80106CB0[];
extern f32 D_80107CB0;
extern f32 D_80107CB4;
extern u64 D_80107CE8;
extern s8 D_80107D1C;
extern u32 D_80107D20[];
extern f32 D_80108FB4;
extern f32 D_801153FC;
extern Gfx *D_8013EC7C;
extern u64 D_80140F50;

/* Debug overlay: sets up a text tile, clears the text buffer and prints one page of debug values. */
void func_80001F60(void) {
    char buf[32];
    Gfx *g;
    register u16 i;
    register u32 *p;
    register f32 a;
    register f32 b;
    register f32 c;

    if (D_80106C6C != 0) {
        g = D_8013EC7C++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
        g = D_8013EC7C++;
        g->w0 = 0xBA001402;
        g->w1 = 0;
        g = D_8013EC7C++;
        g->w0 = 0xB6000000;
        g->w1 = 0x009F3207;
        g = D_8013EC7C++;
        g->w0 = 0xB900031D;
        g->w1 = 0x0F0A4000;
        g = D_8013EC7C++;
        g->w0 = 0xBB000001;
        g->w1 = 0x80008000;
        g = D_8013EC7C++;
        g->w0 = 0xFCFFFFFF;
        g->w1 = 0xFFFCF279;
        g = D_8013EC7C++;
        g->w0 = 0xBA000C02;
        g->w1 = 0;
        g = D_8013EC7C++;
        g->w0 = 0xBA001301;
        g->w1 = 0;
        g = D_8013EC7C++;
        g->w0 = 0xBA000E02;
        g->w1 = 0;
        if (D_800717E0 == 0) {
            g = D_8013EC7C++;
            g->w0 = 0xFD100000;
            g->w1 = (u32) D_80107D20;
            g = D_8013EC7C++;
            g->w0 = 0xF5100000;
            g->w1 = 0x07018060;
            g = D_8013EC7C++;
            g->w0 = 0xE6000000;
            g->w1 = 0;
            g = D_8013EC7C++;
            g->w0 = 0xF3000000;
            g->w1 = 0x077FF080;
            g = D_8013EC7C++;
            g->w0 = 0xE7000000;
            g->w1 = 0;
            g = D_8013EC7C++;
            g->w0 = 0xF5102000;
            g->w1 = 0x00018060;
            g = D_8013EC7C++;
            g->w0 = 0xF2000000;
            g->w1 = 0x000FC07C;
            D_800AD028 = D_80107D20;
            p = D_80107D20;
        } else {
            g = D_8013EC7C++;
            g->w0 = 0xFD100000;
            g->w1 = (u32) D_80106CB0;
            g = D_8013EC7C++;
            g->w0 = 0xF5100000;
            g->w1 = 0x07018060;
            g = D_8013EC7C++;
            g->w0 = 0xE6000000;
            g->w1 = 0;
            g = D_8013EC7C++;
            g->w0 = 0xF3000000;
            g->w1 = 0x077FF080;
            g = D_8013EC7C++;
            g->w0 = 0xE7000000;
            g->w1 = 0;
            g = D_8013EC7C++;
            g->w0 = 0xF5102000;
            g->w1 = 0x00018060;
            g = D_8013EC7C++;
            g->w0 = 0xF2000000;
            g->w1 = 0x000FC07C;
            D_800AD028 = D_80106CB0;
            p = D_80106CB0;
        }
        for (i = 0x400; i != 0; i--, p++) {
            *p = 0;
        }
        switch (D_80106C6C) {
        case 1:
            D_800A9140 = 0;
            D_80107D1C = 0;
            func_80060EB0(buf, D_80079D2C);
            func_8000EDB0(buf);
            D_800A9140 = 1;
            D_80107D1C = 0;
            a = D_80107CE8 / 760625.0f;
            b = D_800A7A68 / 760625.0f;
            c = D_80140F50 / 760625.0f;
            func_80060EB0(buf, D_80079D40, a, b, c);
            func_8000EDB0(buf);
            D_800A9140 = 2;
            D_80107D1C = 0;
            break;
        case 2:
            D_800A9140 = 0;
            D_80107D1C = 0;
            func_80060EB0(buf, D_80079D58);
            func_8000EDB0(buf);
            if (D_80106C84 != NULL) {
                D_800A9140 = 0;
                D_80107D1C = 5;
                func_80060EB0(buf, D_80079D60, (s32) D_80106C84->pos[0]);
                func_8000EDB0(buf);
                D_80107D1C = 14;
                func_80060EB0(buf, D_80079D68, (s32) D_80106C84->pos[1]);
                func_8000EDB0(buf);
                D_80107D1C = 23;
                func_80060EB0(buf, D_80079D70, (s32) D_80106C84->pos[2]);
                func_8000EDB0(buf);
                D_800A9140 = 1;
                D_80107D1C = 0;
                a = func_80069920(D_80106C84->fwd[0], D_80106C84->fwd[2]) * 57.2957;
                if (a < 0.0) {
                    a = a + 360.0;
                }
                func_80060EB0(buf, D_80079D78, a);
                func_8000EDB0(buf);
                D_800A9140 = 2;
                D_80107D1C = 0;
                func_80060EB0(buf, D_80079D88, (f32) D_80106C84->unk_224 * 0.125);
                func_8000EDB0(buf);
            }
            break;
        case 3:
            D_800A9140 = 0;
            D_80107D1C = 0;
            func_80060EB0(buf, D_80079D94);
            func_8000EDB0(buf);
            D_800A9140 = 1;
            D_80107D1C = 0;
            func_80060EB0(buf, D_80079DB0);
            func_8000EDB0(buf);
            D_80107D1C = 7;
            func_80060EB0(buf, D_80079DB8, D_800AD000);
            func_8000EDB0(buf);
            D_80107D1C = 13;
            func_80060EB0(buf, D_80079DB8, D_80107CB0);
            func_8000EDB0(buf);
            D_80107D1C = 21;
            func_80060EB0(buf, D_80079DC0, D_800AD000 * D_800AD000 + D_80107CB0 * D_80107CB0);
            func_8000EDB0(buf);
            D_800A9140 = 2;
            D_80107D1C = 0;
            func_80060EB0(buf, D_80079DC8);
            func_8000EDB0(buf);
            D_80107D1C = 7;
            func_80060EB0(buf, D_80079DB8, D_80107CB4);
            func_8000EDB0(buf);
            D_80107D1C = 13;
            func_80060EB0(buf, D_80079DB8, D_80106C78);
            func_8000EDB0(buf);
            D_80107D1C = 21;
            func_80060EB0(buf, D_80079DC0, D_80107CB4 * D_80107CB4 + D_80106C78 * D_80106C78);
            func_8000EDB0(buf);
            D_800A9140 = 3;
            D_80107D1C = 0;
            func_80060EB0(buf, D_80079DD0);
            func_8000EDB0(buf);
            D_80107D1C = 7;
            func_80060EB0(buf, D_80079DB8, D_801153FC);
            func_8000EDB0(buf);
            D_80107D1C = 13;
            func_80060EB0(buf, D_80079DB8, D_800A9150);
            func_8000EDB0(buf);
            D_80107D1C = 21;
            func_80060EB0(buf, D_80079DC0, D_801153FC * D_801153FC + D_800A9150 * D_800A9150);
            func_8000EDB0(buf);
            break;
        case 4:
            D_800A9140 = 0;
            D_80107D1C = 9;
            func_80060EB0(buf, D_80079DD8);
            func_8000EDB0(buf);
            D_800A9140 = 1;
            D_80107D1C = 0;
            func_80060EB0(buf, D_80079DE8, D_800B1092);
            func_8000EDB0(buf);
            D_800A9140 = 1;
            D_80107D1C = 16;
            func_80060EB0(buf, D_80079DF4, D_80108FB4);
            func_8000EDB0(buf);
            break;
        }
        g = D_8013EC7C++;
        g->w0 = 0xF5602000;
        g->w1 = 0;
        g = D_8013EC7C++;
        g->w0 = 0xF2000000;
        g->w1 = 0x003FC07C;
        g = D_8013EC7C++;
        g->w0 = 0xE44800F0;
        g->w1 = 0x00080070;
        {
            Gfx *g2;

            g2 = D_8013EC7C++;
            g2->w0 = 0xB4000000;
            g2->w1 = 0;
            g2 = D_8013EC7C++;
            g2->w0 = 0xB3000000;
            g2->w1 = 0x04000400;
        }
    }
}
