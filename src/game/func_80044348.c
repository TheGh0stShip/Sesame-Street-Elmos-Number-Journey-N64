#include "common.h"


void func_80044348(s32 arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;

    func_80042780(arg0, ((s32)&sp10));
    (*(f32 *)arg0) = sp10;
    ((struct gd_4_f32 *)arg0)->v = sp14;
    ((struct gd_8_f32 *)arg0)->v = sp18;
}
