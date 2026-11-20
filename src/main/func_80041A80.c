struct gd_4_s32 { u8 pad[0x4]; s32 v; };

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (arg2) {
    case 1:
        (*(s32 *)arg0) = *(s32 *)arg3;
        ((struct gd_4_s32 *)arg0)->v = arg3;
        (*(s32 *)arg3) = arg0;
        if ((*(s32 *)arg0) != 0) {
            ((struct gd_4_s32 *)(*(s32 *)arg0))->v = arg0;
        }
        if (arg3 == ((struct gd_4_s32 *)arg1)->v) {
            ((struct gd_4_s32 *)arg1)->v = arg0;
        }
        return;
    case 2:
        (*(s32 *)arg0) = arg3;
        ((struct gd_4_s32 *)arg0)->v = ((struct gd_4_s32 *)arg3)->v;
        ((struct gd_4_s32 *)arg3)->v = arg0;
        if (((struct gd_4_s32 *)arg0)->v != 0) {
            (*(s32 *)((struct gd_4_s32 *)arg0)->v) = arg0;
        }
        if (arg3 == (*(s32 *)arg1)) {
            (*(s32 *)arg1) = arg0;
        }
        return;
    case 3:
        (*(s32 *)arg0) = *(s32 *)arg1;
        ((struct gd_4_s32 *)arg0)->v = 0;
        if (((struct gd_4_s32 *)arg1)->v == 0) {
            ((struct gd_4_s32 *)arg1)->v = arg0;
        }
        (*(s32 *)arg1) = arg0;
        if ((*(s32 *)arg0) != 0) {
            ((struct gd_4_s32 *)(*(s32 *)arg0))->v = arg0;
        }
        return;
    case 4:
        (*(s32 *)arg0) = 0;
        ((struct gd_4_s32 *)arg0)->v = ((struct gd_4_s32 *)arg1)->v;
        if (((struct gd_4_s32 *)arg1)->v != 0) {
            (*(s32 *)((struct gd_4_s32 *)arg1)->v) = arg0;
        }
        ((struct gd_4_s32 *)arg1)->v = arg0;
        if ((*(s32 *)arg1) == 0) {
            (*(s32 *)arg1) = arg0;
        }
        return;
    }
}
