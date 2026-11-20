/* actor update: timed sound/scene events, then turns its facing toward a target direction */
typedef struct {
    u8 pad0[0xC]; s32 unk_0C;
    u8 pad10[0x20];
    f32 unk_30; f32 unk_34; f32 unk_38;
    f32 unk_3C; f32 unk_40; f32 unk_44;
    f32 unk_48; f32 unk_4C; f32 unk_50;
    u8 pad54[0xEC - 0x54]; void *unk_EC;
    u8 padF0[0x1AC - 0xF0]; f32 unk_1AC; u8 pad1B0[4]; s32 unk_1B4;
    f32 unk_1B8; f32 unk_1BC; f32 unk_1C0;
    u8 pad1C4[8]; s32 unk_1CC; u8 pad1D0[4]; s32 unk_1D4;
    u8 pad1D8[0x1EC - 0x1D8]; s8 unk_1EC; u8 pad1ED[0x204 - 0x1ED]; s32 unk_204;
} Actor;
void func_80018CB0(void);
void func_80037450(Actor *, s32);
void func_800375A8(Actor *, s32);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
void func_80058BA0(void *, s32, s32);
f64 func_80069920(f64, f64);
double fabs(double);
extern s32 D_800319A0;
extern s32 D_80074CFC;
extern s32 *D_8011551C;
extern s32 D_8013EEF8;

void func_8003B6D8(Actor *arg0) {
    register Actor *a;
    register f32 cur;
    register f32 diff;

    if (arg0->unk_1D4 == 25) {
        if (arg0->unk_0C & 8) {
            func_80037450(arg0, 0);
            arg0->unk_1B8 = 0.0f;
            arg0->unk_1C0 = -1.0f;
            arg0->unk_1CC = 0;
        }
        return;
    }
    if (arg0->unk_1CC == 60 && D_8011551C == NULL) {
        D_8011551C = &D_80074CFC;
        func_80058BA0(&D_8013EEF8, 0, 1);
    }
    if (arg0->unk_1CC >= 721 && arg0->unk_1CC < 780 && D_8011551C == NULL) {
        func_80018CB0();
    }
    if (arg0->unk_1CC >= 781 && D_8011551C == NULL) {
        func_800375A8(arg0, 0);
        arg0->unk_EC = &D_800319A0;
    }
    cur = func_80069920(arg0->unk_30, arg0->unk_38);
    diff = func_80069920(arg0->unk_1B8, arg0->unk_1C0);
    diff = diff - cur;
    if (diff > 3.14159) {
        diff = diff - 6.2831853;
    } else if (diff < -3.14159) {
        diff = diff + 6.2831853;
    }
    if (fabs(diff) < 0.03) {
        arg0->unk_30 = arg0->unk_1B8;
        arg0->unk_34 = arg0->unk_1BC;
        arg0->unk_38 = arg0->unk_1C0;
        if (arg0->unk_1EC > 0) {
            arg0->unk_1EC = -1;
        }
    } else {
        cur = cur + diff * 0.2;
        a = arg0;
        a->unk_30 = func_80057E70(cur);
        a = arg0;
        a->unk_38 = func_800566A0(cur);
        arg0->unk_204 = 1;
        arg0->unk_1AC = 5.0f;
        arg0->unk_1B4 = 0;
    }
    arg0->unk_48 = arg0->unk_34 * arg0->unk_44 - arg0->unk_38 * arg0->unk_40;
    arg0->unk_4C = arg0->unk_38 * arg0->unk_3C - arg0->unk_30 * arg0->unk_44;
    arg0->unk_50 = arg0->unk_30 * arg0->unk_40 - arg0->unk_34 * arg0->unk_3C;
}
