struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_2C_s32 { u8 pad[0x2C]; s32 v; };
extern s32 D_80109DE4;
s32 func_80046ABC();

void f(s32 arg0, s32 arg1) {
    s32 sp10;
    s32 pad0;
    s32 pad1[126];
    s32 sp210;
    f32 sp214;
    s32 sp218;
    s32 pad2;
    f32 sp220;
    f32 sp224;
    f32 sp228;

    sp218 = 1;
    sp10 = ((struct gd_10_s32 *)arg1)->v;
    D_80109DE4 = 0;
    for (;;) {
        if (sp218 <= 0) {
            return;
        }
        sp210 = *(s32 *)(((sp218 * 4) + ((s32)&sp10)) - 4);
        sp218--;
        if (((*(s32 *)sp210) & 1) != 0) {
            func_80046ABC(arg0, arg1, sp210);
            D_80109DE4++;
        } else {
            sp220 = ((struct gd_10_f32 *)arg0)->v;
            sp224 = ((struct gd_18_f32 *)arg0)->v;
            sp228 = -((struct gd_14_f32 *)arg0)->v;
            sp214 = (((((struct gd_10_f32 *)sp210)->v * sp220) + (((struct gd_14_f32 *)sp210)->v * sp224)) + (((struct gd_18_f32 *)sp210)->v * sp228)) - ((struct gd_1C_f32 *)sp210)->v;
            if (sp214 >= 0.0f) {
                if ((((struct gd_2C_s32 *)arg0)->v & 1) == 0) {
                    if (sp214 <= (*(f32 *)arg0)) {
                        (*(s32 *)((sp218 * 4) + ((s32)&sp10))) = ((struct gd_24_s32 *)sp210)->v;
                        sp218++;
                    }
                }
                (*(s32 *)((sp218 * 4) + ((s32)&sp10))) = ((struct gd_20_s32 *)sp210)->v;
                sp218++;
            } else {
                if ((((struct gd_2C_s32 *)arg0)->v & 1) == 0) {
                    if ((-(*(f32 *)arg0)) < sp214) {
                        (*(s32 *)((sp218 * 4) + ((s32)&sp10))) = ((struct gd_20_s32 *)sp210)->v;
                        sp218++;
                    }
                }
                (*(s32 *)((sp218 * 4) + ((s32)&sp10))) = ((struct gd_24_s32 *)sp210)->v;
                sp218++;
            }
        }
        for (;;) {
            if (sp218 < 129) {
                break;
            }
        }
    }
}
