struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_6_s16 { u8 pad[0x6]; s16 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_8_s16 { u8 pad[0x8]; s16 v; };
struct gd_A_s16 { u8 pad[0xA]; s16 v; };
struct gd_C_s16 { u8 pad[0xC]; s16 v; };
struct gd_E_s16 { u8 pad[0xE]; s16 v; };

void f(s32 arg0, s32 arg1, s32 arg2) {
    if ((*(f32 *)arg1) > 0.0f) {
        if (((struct gd_4_f32 *)arg1)->v > 0.0f) {
            if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
            } else {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
            }
        } else {
            if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
            } else {
                (*(f32 *)arg0) = (f32)((struct gd_4_s16 *)arg2)->v;
                ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
                ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
            }
        }
        return;
    }
    if (((struct gd_4_f32 *)arg1)->v > 0.0f) {
        if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
            (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
            ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
            ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
        } else {
            (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
            ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_8_s16 *)arg2)->v;
            ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
        }
        return;
    }
    if (((struct gd_8_f32 *)arg1)->v > 0.0f) {
        (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
        ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
        ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_C_s16 *)arg2)->v;
        return;
    }
    (*(f32 *)arg0) = (f32)((struct gd_6_s16 *)arg2)->v;
    ((struct gd_4_f32 *)arg0)->v = (f32)((struct gd_A_s16 *)arg2)->v;
    ((struct gd_8_f32 *)arg0)->v = (f32)((struct gd_E_s16 *)arg2)->v;
}
