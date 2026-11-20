typedef struct {
    f32 pos[3];
    f32 yaw;
    f32 pitch;
    f32 unk_14;
    f32 roll;
    f32 unk_1C;
    f32 unk_20;
    f32 unk_24;
} CamState;
typedef struct {
    u8 unk_00[0x38];
    CamState st;
} CamRef;
typedef struct {
    CamRef *ref;
    s32 unk_04;
    f32 posLimit;
    f32 angLimit;
    f32 rollLimit;
} Clamp;
void func_80060E7C(void *dst, void *src, s32 size);
f32 func_8004B828(f32 a, f32 b);

/* copies a camera state, clamping each component to within a limit of a reference state */
void func_8004CFA8(Clamp *arg0, CamState *in, CamState *out) {
    register Clamp *c = arg0;
    register f32 lim;
    register f32 alim;
    register f32 rlim;
    f32 dr;
    f32 pad;
    f32 d[3];
    f32 dy;
    f32 dp;
    f32 pad2;

    lim = c->posLimit;
    alim = c->angLimit * in->unk_1C;
    rlim = c->rollLimit;
    func_80060E7C(out, in, sizeof(CamState));
    if (lim < 1000.0f) {
        d[0] = in->pos[0] - c->ref->st.pos[0];
        if (lim < d[0]) {
            out->pos[0] = c->ref->st.pos[0] + lim;
        }
        if (d[0] < -lim) {
            out->pos[0] = c->ref->st.pos[0] - lim;
        }
        d[1] = in->pos[1] - c->ref->st.pos[1];
        if (lim < d[1]) {
            out->pos[1] = c->ref->st.pos[1] + lim;
        }
        if (d[1] < -lim) {
            out->pos[1] = c->ref->st.pos[1] - lim;
        }
        d[2] = in->pos[2] - c->ref->st.pos[2];
        if (lim < d[2]) {
            out->pos[2] = c->ref->st.pos[2] + lim;
        }
        if (d[2] < -lim) {
            out->pos[2] = c->ref->st.pos[2] - lim;
        }
    }
    dy = func_8004B828(in->yaw, c->ref->st.yaw);
    if (alim < dy) {
        out->yaw = out->yaw = alim + c->ref->st.yaw;
    }
    if (dy < -alim) {
        out->yaw = out->yaw = c->ref->st.yaw - alim;
    }
    dp = in->pitch - c->ref->st.pitch;
    if (alim < dp) {
        out->pitch = c->ref->st.pitch + alim;
    }
    if (dp < -alim) {
        out->pitch = c->ref->st.pitch - alim;
    }
    if (rlim < 1000.0f) {
        dr = in->roll - c->ref->st.roll;
        if (rlim < dr) {
            out->roll = c->ref->st.roll + rlim;
        }
        if (dr < -rlim) {
            out->roll = c->ref->st.roll - rlim;
        }
    }
}
