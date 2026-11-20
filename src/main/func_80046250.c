struct gd_4_s32 { u8 pad[0x4]; s32 v; };
extern s32 D_8007B328;
extern s32 D_8007B32C;
s32 func_80059540();

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (arg0) {
    case 0:
        func_80059540(((s32)&D_8007B328), ((s32)&D_8007B32C), 41);
    }
    switch (arg2) {
    case 1:
        if ((*(s32 *)arg0) != 0) {
            ((struct gd_4_s32 *)(*(s32 *)arg0))->v = arg1;
        } else {
            ((struct gd_4_s32 *)arg0)->v = arg1;
        }
        (*(s32 *)arg1) = *(s32 *)arg0;
        ((struct gd_4_s32 *)arg1)->v = 0;
        (*(s32 *)arg0) = arg1;
        return;
    case 2:
        if (((struct gd_4_s32 *)arg0)->v != 0) {
            (*(s32 *)((struct gd_4_s32 *)arg0)->v) = arg1;
        } else {
            (*(s32 *)arg0) = arg1;
        }
        (*(s32 *)arg1) = 0;
        ((struct gd_4_s32 *)arg1)->v = ((struct gd_4_s32 *)arg0)->v;
        ((struct gd_4_s32 *)arg0)->v = arg1;
        return;
    case 3:
        switch (arg3) {
        case 0:
            func_80059540(((s32)&D_8007B328), ((s32)&D_8007B32C), 63);
        }
        (*(s32 *)arg1) = *(s32 *)arg3;
        ((struct gd_4_s32 *)arg1)->v = arg3;
        if ((*(s32 *)arg3) != 0) {
            ((struct gd_4_s32 *)(*(s32 *)arg3))->v = arg1;
        }
        (*(s32 *)arg3) = arg1;
        return;
    case 4:
        switch (arg3) {
        case 0:
            func_80059540(((s32)&D_8007B328), ((s32)&D_8007B32C), 71);
        }
        (*(s32 *)arg1) = arg3;
        ((struct gd_4_s32 *)arg1)->v = ((struct gd_4_s32 *)arg3)->v;
        if (((struct gd_4_s32 *)arg3)->v != 0) {
            (*(s32 *)((struct gd_4_s32 *)arg3)->v) = arg1;
        }
        ((struct gd_4_s32 *)arg3)->v = arg1;
        return;
    }
    func_80059540(((s32)&D_8007B328), ((s32)&D_8007B32C), 79);
    return;
}
