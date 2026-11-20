typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { s32 x; s32 y; char text[0x28]; } CreditLine;
extern u8 D_80071758;
extern s32 D_8007174C;
extern s32 D_80071750;
extern u8 D_80071753;
extern u32 D_80071754;
extern s8 D_80071759;
extern u8 D_8007175A;
extern u8 D_8007175B;
extern u8 D_80128443;
extern s8 D_80076B60;
extern s8 D_800A7A5D;
extern Gfx *D_8013EC7C;
extern CreditLine D_8006FEEC[];
extern u8 D_8006FE68[];
extern char D_80079EA8[];
extern Gfx D_80075580[];
s32 func_8000E030(void);
void func_8000E16C(s32);
void func_8000E2D0(s32);
void func_8000E1B0(s32, s32);
void func_8000E1EC(s32, s32);
void func_8000E264(s32, s32, s32, s32);
void func_8000E32C(char *);
void func_8006A410(char *, char *);
s32 func_80060E58(char *);
void func_80069C70(char *, s32, s32);
/* credits screen: advances the visible line range with a fade, then draws the title and lines */
void func_8000ADBC(s32 arg0) {
    char buf[0x50];
    s32 pad;
    s32 i;
    s32 len;
    s32 saved;
    s32 x;
    s32 y;
    s32 unused;
    Gfx *g;
    Gfx *g2;

    y = 10;
    unused = 0;
    if (D_80071758 != 0 && D_8007174C >= 100) {
        D_80128443 = 0xFF;
        D_80071759 = 0;
        D_80071750 = 0xFF;
        D_8007175A = 1;
        D_8007175B = 8;
        D_80071754 = 0;
        D_80076B60 = 0;
        D_80071758 = 0;
        D_8007174C = 0;
        D_800A7A5D = 0;
        return;
    }
    if (D_8007174C >= 101) {
        D_80071750 -= 5;
        if (D_80071750 <= 0) {
            switch (D_80071754) {
            case 0:
                D_8007175A = 0x8;
                D_8007175B = 0xF;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 1;
                break;
            case 1:
                D_8007175A = 0xF;
                D_8007175B = 0x15;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 2;
                break;
            case 2:
                D_8007175A = 0x15;
                D_8007175B = 0x1B;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 3;
                break;
            case 3:
                D_8007175A = 0x1B;
                D_8007175B = 0x1E;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 4;
                break;
            case 4:
                D_8007175A = 0x1E;
                D_8007175B = 0x23;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 5;
                break;
            case 5:
                D_8007175A = 0x23;
                D_8007175B = 0x29;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 6;
                break;
            case 6:
                D_8007175A = 0x29;
                D_8007175B = 0x2C;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 7;
                break;
            case 7:
                D_8007175A = 0x2C;
                D_8007175B = 0x30;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 8;
                break;
            case 8:
                D_8007175A = 0x30;
                D_8007175B = 0x35;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 9;
                break;
            case 9:
                D_8007175A = 0x35;
                D_8007175B = 0x3B;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 10;
                break;
            case 10:
                D_8007175A = 0x3B;
                D_8007175B = 0x41;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 11;
                break;
            case 11:
                D_8007175A = 0x41;
                D_8007175B = 0x46;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 12;
                break;
            case 12:
                D_8007175A = 0x46;
                D_8007175B = 0x4C;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 13;
                break;
            case 13:
                D_8007175A = 0x4C;
                D_8007175B = 0x52;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 14;
                break;
            case 14:
                D_8007175A = 0x52;
                D_8007175B = 0x55;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 15;
                break;
            case 15:
                D_8007175A = 0x55;
                D_8007175B = 0x59;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 16;
                break;
            case 16:
                D_8007175A = 0x59;
                D_8007175B = 0x5C;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 17;
                break;
            case 17:
                D_8007175A = 0x5C;
                D_8007175B = 0x60;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 18;
                break;
            case 18:
                D_8007175A = 0x60;
                D_8007175B = 0x65;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 19;
                break;
            case 19:
                D_8007175A = 0x65;
                D_8007175B = 0x6A;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 20;
                break;
            case 20:
                D_8007175A = 0x6A;
                D_8007175B = 0x6D;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 21;
                break;
            case 21:
                D_8007175A = 0x6D;
                D_8007175B = 0x70;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 22;
                break;
            case 22:
                D_8007175A = 0x70;
                D_8007175B = 0x76;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 23;
                break;
            case 23:
                D_8007175A = 0x76;
                D_8007175B = 0x7C;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 24;
                break;
            case 24:
                D_8007175A = 0x7C;
                D_8007175B = 0x7F;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 25;
                break;
            case 25:
                D_8007175A = 0x7F;
                D_8007175B = 0x82;
                D_80071750 = 0xFF;
                D_8007174C = 0;
                D_80071754 = 0;
                D_80071758 = 1;
                break;
            default:
                break;
            }
        }
    } else {
        D_8007174C++;
    }
    g = D_8013EC7C++; g->w0 = 0xB900031D; g->w1 = 0x00504240;
    g = D_8013EC7C++; g->w0 = 0xFCFFFFFF; g->w1 = 0xFFFEFB7D;
    g = D_8013EC7C++; g->w0 = 0xBA000E02; g->w1 = 0;
    g = D_8013EC7C++; g->w0 = 0xBB000000; g->w1 = 0x80008000;
    g = D_8013EC7C++; g->w0 = 0xFB000000; g->w1 = 0xFF;
    g = D_8013EC7C++; g->w0 = 0xE45003C0; g->w1 = 0;
    g2 = D_8013EC7C++; g2->w0 = 0xB4000000; g2->w1 = 0;
    g2 = D_8013EC7C++; g2->w0 = 0xB3000000; g2->w1 = 0x04000400;
    saved = func_8000E030();
    func_8000E2D0(1);
    func_8000E1B0(0x50, 1);
    y += 15;
    func_8006A410(buf, D_80079EA8);
    len = func_80060E58(buf);
    x = len * 8;
    x = (0x140 - x) >> 1;
    func_8000E1EC(x, y);
    func_8000E264(0xFF, 0xFF, 0xFF, 0xFF);
    func_8000E32C(buf);
    for (i = D_8007175A; i < D_8007175B; i++) {
        y += 15;
        func_80069C70(buf, 0, 0x28);
        func_8006A410(buf, D_8006FEEC[i].text);
        len = func_80060E58(D_8006FEEC[i].text);
        x = len << 3;
        D_8006FEEC[i].x = (0x140 - x) >> 1;
        y = D_8006FEEC[i].y + i * 24;
        func_8000E1EC(D_8006FEEC[i].x, D_8006FEEC[i].y);
        switch (D_8006FE68[i]) {
        case 0:
            func_8000E264(0xFF, 0xFF, 0xFF, D_80071753);
            break;
        case 1:
            func_8000E264(0, 0xFF, 0, D_80071753);
            break;
        case 2:
            func_8000E264(0xFF, 0xFF, 0, D_80071753);
            break;
        case 3:
            func_8000E264(0, 0x64, 0xC8, D_80071753);
            break;
        default:
            func_8000E264(0xFF, 0xFF, 0xFF, D_80071753);
            break;
        }
        func_8000E32C(D_8006FEEC[i].text);
    }
    func_8000E16C(saved);
    g2 = D_8013EC7C++; g2->w0 = 0x06000000; g2->w1 = (u32)D_80075580;
}
