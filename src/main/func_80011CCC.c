struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_19_s8 { u8 pad[0x19]; s8 v; };
struct gd_1A_s8 { u8 pad[0x1A]; s8 v; };
struct gd_1A_u8 { u8 pad[0x1A]; u8 v; };
struct gd_1B_s8 { u8 pad[0x1B]; s8 v; };
s32 func_80011554();

void f(register s32 arg0) {
    ((struct gd_C_s32 *)arg0)->v = (((struct gd_19_s8 *)arg0)->v << 15) + ((struct gd_C_s32 *)arg0)->v;
    ((struct gd_10_s32 *)arg0)->v = (((struct gd_1A_s8 *)arg0)->v << 15) + ((struct gd_10_s32 *)arg0)->v;
    ((struct gd_14_s32 *)arg0)->v = (((struct gd_1B_s8 *)arg0)->v << 15) + ((struct gd_14_s32 *)arg0)->v;
    if (((struct gd_1A_s8 *)arg0)->v >= -118) {
        ((struct gd_1A_s8 *)arg0)->v = ((struct gd_1A_u8 *)arg0)->v - 4;
    }
    func_80011554(arg0);
}
