

struct n64dk_mE8_40 { u8 pad[0xE8]; struct { u8 b[0x40]; } m[1]; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
typedef struct { s32 a, b, c; } Trip;
struct gd_1A8_t { u8 pad[0x1A8]; Trip v; };
struct gd_1B0_s32 { u8 pad[0x1B0]; s32 v; };
struct gd_1F0_s32 { u8 pad[0x1F0]; s32 v; };
extern s32 D_800258C0;
extern s32 D_800717E0;
extern s32 D_800741E0;
extern f64 D_8007A7C8;
extern u8 D_800AD03A;
extern s32 D_80109D4C;
extern f32 D_8010E64C;
extern u8 D_80115518;
extern s32 D_80115520;
s32 func_80001424(s32, s32, s32, s32, f32);
s32 func_8001FAA8();
f64 func_800236B4();

void f(s32 arg0) {
    s32 sp18;
    s32 pad0;
    s32 pad1[2];
    s8 pad2;

    sp18 = arg0;
    sp18 = arg0;
    if (D_800AD03A == 0) {
        if (D_80115520 < 0) {
            ((struct gd_C_s32 *)sp18)->v = ((struct gd_C_s32 *)sp18)->v | 1;
        }
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            ((struct gd_C_s32 *)sp18)->v = ((struct gd_C_s32 *)sp18)->v | 1;
        }
        return;
    }
    D_80115518 = D_80115518 + 4;
    D_8010E64C = (f32)(((f64)(*(f32 *)(((((u32)D_80115518 >> 3) & 255) * 4) + ((s32)&D_800741E0)))) * D_8007A7C8);
    ((struct gd_28_f32 *)sp18)->v = ((struct gd_28_f32 *)sp18)->v + D_8010E64C;
    func_8001FAA8(((s32)&((struct n64dk_mE8_40 *)sp18)->m[D_800717E0]), (sp18 + 36), ((struct gd_1A8_t *)sp18)->v);
    ((struct gd_28_f32 *)sp18)->v = ((struct gd_28_f32 *)sp18)->v - D_8010E64C;
    func_80001424(((s32)&D_800258C0), sp18, ((struct gd_1F0_s32 *)sp18)->v, 1, ((f32)func_800236B4((sp18 + 36))));
}
