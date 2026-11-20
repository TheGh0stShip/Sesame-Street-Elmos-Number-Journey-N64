/* Countdown; when not expired, spawns 12 particles from func_80012E78 and advances position. */


struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_18_s8 { u8 pad[0x18]; s8 v; };
struct gd_18_u8 { u8 pad[0x18]; u8 v; };
struct gd_19_s8 { u8 pad[0x19]; s8 v; };
struct gd_1A_s8 { u8 pad[0x1A]; s8 v; };
struct gd_1A_u8 { u8 pad[0x1A]; u8 v; };
struct gd_1B_s8 { u8 pad[0x1B]; s8 v; };
struct gd_22_s8 { u8 pad[0x22]; s8 v; };
struct gd_23_s8 { u8 pad[0x23]; s8 v; };
struct gd_24_s16 { u8 pad[0x24]; s16 v; };
extern s32 D_80011554;
s32 func_80012E78();
s32 func_80012F14();

void f(register s32 arg0) {
    register s32 reg_s1;
    register s32 reg_s2;
    s32 pad0[4];
    s8 pad1;

    if (((s8)(--((struct gd_18_s8 *)arg0)->v)) == 0) {
        func_80012F14(arg0);
        return;
    }
    for (reg_s2 = 0; !(reg_s2 == 12); reg_s2 = reg_s2 + 1) {
        if ((reg_s1 = func_80012E78()) != 0) {
            ((struct gd_8_s32 *)reg_s1)->v = (s32)&D_80011554;
            ((struct gd_C_s32 *)reg_s1)->v = ((struct gd_C_s32 *)arg0)->v;
            ((struct gd_10_s32 *)reg_s1)->v = ((struct gd_10_s32 *)arg0)->v;
            ((struct gd_14_s32 *)reg_s1)->v = ((struct gd_14_s32 *)arg0)->v;
            if (reg_s2 == 7) {
                ((struct gd_24_s16 *)reg_s1)->v = 70;
            } else {
                ((struct gd_24_s16 *)reg_s1)->v = 50;
            }
            ((struct gd_18_s8 *)reg_s1)->v = 2;
            ((struct gd_22_s8 *)reg_s1)->v = (reg_s2 * 16) + 63;
            ((struct gd_23_s8 *)reg_s1)->v = 1;
        }
        ((struct gd_C_s32 *)arg0)->v = (((struct gd_19_s8 *)arg0)->v << 12) + ((struct gd_C_s32 *)arg0)->v;
        ((struct gd_10_s32 *)arg0)->v = (((struct gd_1A_s8 *)arg0)->v << 14) + ((struct gd_10_s32 *)arg0)->v;
        ((struct gd_14_s32 *)arg0)->v = (((struct gd_1B_s8 *)arg0)->v << 12) + ((struct gd_14_s32 *)arg0)->v;
        if (((struct gd_1A_s8 *)arg0)->v >= -118) {
            ((struct gd_1A_s8 *)arg0)->v = ((struct gd_1A_u8 *)arg0)->v - 1;
        }
    }
}
