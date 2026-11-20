typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;
typedef struct HitInfo {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
} HitInfo;
typedef struct Ray {
    f32 unk_00;
    Vec3f unk_04;
    Vec3f unk_10;
    Vec3f unk_1C;
    f32 unk_28;
    s32 unk_2C;
    s32 unk_30;
    s32 unk_34;
    f32 unk_38;
    HitInfo *unk_3C;
    u8 pad40[0x18];
    s32 unk_58;
} Ray;
s32 func_8001E9E0(Ray *);
void func_80042EB0(Ray *, s32);
void func_80043170(Ray *);
void func_800432EC(Ray *);
void func_80043490(Ray *);
void func_800437CC(Ray *);
f32 func_80056690(f32);

/* steps a ray from start toward end against collision, sliding on hits */
s32 func_800208D0(Ray *arg0, s32 arg1, Vec3f *arg2, Vec3f *arg3, Vec3f *arg4, s32 *arg5) {
    Vec3f dir;
    s32 pad1C[4];
    Vec3f pos;
    s32 pad3C[4];
    u32 n;
    u32 iter;
    s32 state;
    f32 len;
    f32 step;
    f32 total;
    Ray *r;
    HitInfo *hit;
    s32 hits;
    s32 mask;
    s32 pad78[2];
    f64 inv;

    r = arg0;
    mask = 0;
    pos.x = arg2->x;
    pos.y = arg2->y;
    pos.z = arg2->z;
    dir.x = arg3->x - pos.x;
    dir.y = arg3->y - pos.y;
    dir.z = arg3->z - pos.z;
    len = func_80056690(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        dir.x = dir.x * inv;
        dir.y = dir.y * inv;
        dir.z = dir.z * inv;
    } else {
    }
    total = len;
    for (n = 1; n * r->unk_00 <= len; n++) {
    }
    step = len / n;
    r->unk_28 = step;
    if (step >= 0.1f) {
        state = 1;
    } else {
        state = 3;
    }
    iter = 0;
    hits = 0;
    r->unk_1C.x = dir.x;
    r->unk_1C.y = -dir.z;
    r->unk_1C.z = dir.y;
    r->unk_04.x = pos.x;
    r->unk_04.y = -pos.z;
    r->unk_04.z = pos.y;
    if (n >= 2) {
        r->unk_10.x = r->unk_04.x + r->unk_1C.x * step;
        r->unk_10.y = r->unk_04.y + r->unk_1C.y * step;
        r->unk_10.z = r->unk_04.z + r->unk_1C.z * step;
    } else {
        r->unk_10.x = arg3->x;
        r->unk_10.y = -arg3->z;
        r->unk_10.z = arg3->y;
    }
    while (state == 1) {
        r->unk_34 = 0;
        r->unk_38 = 1e20f;
        func_80042EB0(r, arg0->unk_58);
        if (r->unk_34 == 0) {
            r->unk_04.x = r->unk_10.x;
            r->unk_04.y = r->unk_10.y;
            r->unk_04.z = r->unk_10.z;
            n--;
            if (n != 0) {
                r->unk_10.x = r->unk_04.x + r->unk_1C.x * step;
                r->unk_10.y = r->unk_04.y + r->unk_1C.y * step;
                r->unk_10.z = r->unk_04.z + r->unk_1C.z * step;
                r->unk_28 = step;
            } else {
                state = 3;
            }
        } else {
            if (r->unk_2C & 1) {
                return 1;
            }
            hit = r->unk_3C;
            switch (r->unk_34) {
                case 2:
                    if (r->unk_2C & 2) {
                        func_80043490(r);
                    } else {
                        func_800432EC(r);
                        if (func_8001E9E0(r) != 0) {
                            state = 4;
                        }
                    }
                    hits++;
                    break;
                case 5:
                case 6:
                    func_80043170(r);
                    if (func_8001E9E0(r) != 0) {
                        state = 4;
                    }
                    hits++;
                    break;
                case 3:
                case 4:
                case 10:
                case 12:
                    if (r->unk_2C & 2) {
                        func_80043490(r);
                    } else {
                        func_80043170(r);
                        if (func_8001E9E0(r) != 0) {
                            state = 4;
                        }
                    }
                    hits++;
                    break;
                case 1:
                    func_800437CC(r);
                    hits++;
                    break;
            }
            mask |= hit->unk_08;
            if (hit->unk_08 & *arg5) {
                arg4->x = r->unk_1C.x;
                arg4->y = r->unk_1C.z;
                arg4->z = -r->unk_1C.y;
                arg3->x = r->unk_04.x;
                arg3->y = r->unk_04.z;
                arg3->z = -r->unk_04.y;
                *arg5 = mask;
                return 2;
            } else if (r->unk_28 < 0.1f) {
                n--;
                if (n != 0) {
                    r->unk_10.x = r->unk_04.x + r->unk_1C.x * step;
                    r->unk_10.y = r->unk_04.y + r->unk_1C.y * step;
                    r->unk_10.z = r->unk_04.z + r->unk_1C.z * step;
                    r->unk_28 = step;
                    state = 1;
                } else {
                    state = 3;
                }
            } else if (state == 4) {
                n--;
                if (n != 0) {
                    pos.x = r->unk_04.x;
                    pos.y = r->unk_04.z;
                    pos.z = -r->unk_04.y;
                    dir.x = arg3->x - pos.x;
                    dir.y = arg3->y - pos.y;
                    dir.z = arg3->z - pos.z;
                    len = func_80056690(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
                    if (len >= 1e-7f) {
                        inv = 1.0 / len;
                        dir.x = dir.x * inv;
                        dir.y = dir.y * inv;
                        dir.z = dir.z * inv;
                    } else {
                    }
                    if (len > 1e-7f) {
                        r->unk_1C.x = dir.x;
                        r->unk_1C.y = -dir.z;
                        r->unk_1C.z = dir.y;
                        r->unk_10.x = r->unk_04.x + r->unk_1C.x * step;
                        r->unk_10.y = r->unk_04.y + r->unk_1C.y * step;
                        r->unk_10.z = r->unk_04.z + r->unk_1C.z * step;
                        r->unk_28 = step;
                        state = 1;
                    } else {
                        state = 3;
                    }
                } else {
                    state = 3;
                }
            }
        }
        iter++;
        if (iter >= 7) {
            state = 2;
        }
    }
    arg3->x = r->unk_04.x;
    arg3->y = r->unk_04.z;
    arg3->z = -r->unk_04.y;
    arg4->x = r->unk_1C.x;
    arg4->y = r->unk_1C.z;
    arg4->z = -r->unk_1C.y;
    *arg5 = mask;
    if (hits != 0) {
        return 1;
    }
    return 0;
}
