typedef struct { f32 x, y, z; } Vec3f;

/* Adds signed byte deltas from an animation record to a position vector. */
void func_80000CA8(s32 arg0, u16 *arg1, s32 arg2, Vec3f *arg3, Vec3f *arg4) {
    s8 *sp0;
    u16 *sp4;

    sp4 = arg1;
    if ((*arg1 & 112) != 0) {
        sp0 = (s8 *)((u8 *)arg1 + *arg1 + ((((u8 *)sp4)[6] & 3) & 255) * arg2);
        if ((((u8 *)sp4)[6] & 16) != 0) {
            arg3->x += *sp0++;
        }
        if ((((u8 *)sp4)[6] & 32) != 0) {
            arg3->y += *sp0++;
        }
        if ((((u8 *)sp4)[6] & 64) != 0) {
            arg3->x = arg3->x + *sp0 * arg4->x;
            arg3->z += *sp0++ * arg4->z;
        }
    }
}
