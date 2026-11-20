struct gd_1AC_f32 { u8 pad[0x1AC]; f32 v; };
struct gd_1B4_f32 { u8 pad[0x1B4]; f32 v; };
struct gd_204_s32 { u8 pad[0x204]; s32 v; };
struct gd_226_s16 { u8 pad[0x226]; s16 v; };
struct gd_226_u16 { u8 pad[0x226]; u16 v; };
extern f64 D_8007AFC8;
extern f64 D_8007AFD0;
extern s16 D_800A7F6A;
extern u16 D_80107CBA;
extern u8 D_80107CF0;
extern s8 D_8012F150;
extern s32 D_8012F154;
extern s8 D_80140F48;
s32 func_800375A8();
s32 func_800377A8();
f32 func_80056690(f32);

void func_80037944(s32 arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    f64 sp28;

    sp10 = ((struct gd_1AC_f32 *)arg0)->v;
    sp14 = 0;
    sp18 = ((struct gd_1B4_f32 *)arg0)->v;
    sp20 = func_80056690((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
    if (sp20 >= 1e-07f) {
        sp28 = D_8007AFC8 / ((f64)sp20);
        sp10 = (f32)(((f64)sp10) * sp28);
        sp14 = (f32)(((f64)sp14) * sp28);
        sp18 = (f32)(((f64)sp18) * sp28);
    } else {
    }
    if (((f64)sp20) > D_8007AFD0) {
        sp20 = 127.0f;
    }
    D_80140F48 = (s32)sp20;
    ((struct gd_226_s16 *)arg0)->v = ((u8)sp20 << 1) + 8;
    if ((u32)((struct gd_226_u16 *)arg0)->v >= 17) {
        ((struct gd_226_s16 *)arg0)->v = 16;
    }
    func_800377A8();
    if (((D_80107CBA & 0xC00F) & 0xFFFF) != 0) {
        if (D_8012F154 != 11) {
            func_800375A8(arg0, 6);
        }
    } else {
        if (((struct gd_204_s32 *)arg0)->v == 0) {
            func_800375A8(arg0, 0);
        }
    }
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
