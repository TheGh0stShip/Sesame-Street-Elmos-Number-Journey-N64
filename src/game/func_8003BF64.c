#include "common.h"


void func_8003BF64(s32 arg0) {
    func_800374D0(arg0);
    func_8003BEA4(arg0);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_1DC_f32 *)arg0)->v = 0.01f;
    ((struct gd_1E0_f32 *)arg0)->v = (f32)func_80069920(((f64)((struct gd_30_f32 *)arg0)->v), ((f64)((struct gd_38_f32 *)arg0)->v));
    ((struct gd_1E8_f32 *)arg0)->v = (f32)func_80069920(((f64)(((struct gd_24_f32 *)D_80076800)->v - ((struct gd_24_f32 *)arg0)->v)), ((f64)(((struct gd_2C_f32 *)D_80076800)->v - ((struct gd_2C_f32 *)arg0)->v)));
    ((struct gd_EC_s32 *)arg0)->v = (s32)&D_800324A0;
    func_8004BCEC(D_80133FB8, 0);
    if (D_800A7F2C != 0) {
        func_8004E884(D_800A7F2C, 0);
    }
    switch (D_8012F154) {
    case 9:
        func_8004E884(D_800A7F6C, 0);
        func_8001933C(((s32)&D_004C12F0), ((s32)&D_004C1970), 85, 20);
        if (D_800A7A74 == D_800A7A58) {
            D_8012FE68 = 9;
        } else {
            if (D_800ACFB0 == D_8010E648) {
                D_8012FE68 = 11;
                D_80109DF4 = 0;
            } else {
                D_8012FE68 = 10;
            }
        }
        return;
    case 10:
        func_8004E884(D_800A7F6C, 0);
        func_8001933C(((s32)&D_004BC400), ((s32)&D_004BCB50), 85, 20);
        if (D_800A7A74 == D_800A7A58) {
            D_8012FE68 = 9;
        } else {
            if (D_800ACFB0 == D_8010E648) {
                D_8012FE68 = 11;
                D_80109DF4 = 0;
            } else {
                D_8012FE68 = 9;
            }
        }
        return;
    case 7:
        func_8004E884(D_800A7F6C, 0);
        func_8001933C(((s32)&D_004C6350), ((s32)&D_004C69A0), 85, 20);
        if (D_800A7A74 == D_800A7A58) {
            D_8012FE68 = 7;
        } else {
            if (D_800ACFB0 == D_8010E648) {
                D_8012FE68 = 11;
                D_80109DF4 = 1;
            } else {
                D_8012FE68 = 8;
            }
        }
        return;
    case 8:
        func_8004E884(D_800A7F6C, 0);
        func_8001933C(((s32)&D_004CB8D0), ((s32)&D_004CBCD0), 85, 20);
        if (D_800A7A74 == D_800A7A58) {
            D_8012FE68 = 7;
        } else {
            if (D_800ACFB0 == D_8010E648) {
                D_8012FE68 = 11;
                D_80109DF4 = 1;
            } else {
                D_8012FE68 = 7;
            }
        }
        return;
    case 5:
        func_8004E884(D_800A7F6C, 0);
        func_8001933C(((s32)&D_004D7C90), ((s32)&D_004D83F0), 85, 20);
        if (D_800A7A74 == D_800A7A58) {
            D_8012FE68 = 5;
        } else {
            if (D_800ACFB0 == D_8010E648) {
                D_8012FE68 = 11;
                D_80109DF4 = 2;
            } else {
                D_8012FE68 = 6;
            }
        }
        return;
    case 6:
        func_8004E884(D_800A7F6C, 0);
        func_8001933C(((s32)&D_004CFEC0), ((s32)&D_004D0690), 85, 20);
        if (D_800A7A74 == D_800A7A58) {
            D_8012FE68 = 5;
        } else {
            if (D_800ACFB0 == D_8010E648) {
                D_8012FE68 = 11;
                D_80109DF4 = 2;
            } else {
                D_8012FE68 = 5;
            }
        }
        return;
    case 11:
        if (D_800A7A5C == 7) {
            D_8012FE68 = 1;
            return;
        }
        D_8012FE68 = 1;
        return;
    }
}
