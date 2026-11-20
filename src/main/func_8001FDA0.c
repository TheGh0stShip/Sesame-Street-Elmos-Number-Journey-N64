typedef struct { u32 time; f32 v[3]; } VecKey;
typedef struct { u32 time; f32 q[4]; s32 unk_14; s32 unk_18; } RotKey;
typedef struct {
    u32 numTrans;
    VecKey *trans;
    u32 numRot;
    RotKey *rot;
    u32 numScale;
    VecKey *scale;
} AnimTrack;

/* Samples a translation/rotation/scale keyframe track at time t into a 4x4 matrix. */
void func_8001FDA0(f32 m[4][4], AnimTrack *track, u32 t) {
    u32 n;
    u32 i;
    f32 trans[4];
    f32 scale[4];
    f32 frac;
    f32 u;
    f32 xx;
    f32 yy;
    f32 zz;
    f32 xy;
    f32 xz;
    f32 yz;
    f32 wx;
    f32 wy;
    f32 wz;
    VecKey *a;
    VecKey *b;
    VecKey *c;
    f32 q[4];
    f32 f;

    a = track->trans;
    n = track->numTrans;
    for (i = 1; i < n && (a + 1)->time <= t; i++, a++) {
    }
    b = a;
    if (i != n && a->time < t) {
        c = b + 1;
        frac = (f32)(t - b->time) / (f32)(c->time - b->time);
        u = 1.0f - frac;
        trans[0] = u * b->v[0] + frac * c->v[0];
        trans[1] = u * b->v[1] + frac * c->v[1];
        trans[2] = u * b->v[2] + frac * c->v[2];
    } else {
        trans[0] = b->v[0];
        trans[1] = b->v[1];
        trans[2] = b->v[2];
    }

    c = track->scale;
    n = track->numScale;
    for (i = 1; i < n && (c + 1)->time <= t; i++, c++) {
    }
    b = c;
    if (i != n && c->time < t) {
        a = b + 1;
        frac = (f32)(t - b->time) / (f32)(a->time - b->time);
        u = 1.0f - frac;
        scale[0] = u * b->v[0] + frac * a->v[0];
        scale[1] = u * b->v[1] + frac * a->v[1];
        scale[2] = u * b->v[2] + frac * a->v[2];
    } else {
        scale[0] = b->v[0];
        scale[1] = b->v[1];
        scale[2] = b->v[2];
    }

    (RotKey *)c = track->rot;
    n = track->numRot;
    for (i = 1; i < n && ((RotKey *)c + 1)->time <= t; i++, ((RotKey *)c)++) {
    }
    b = c;
    if (i != n && ((RotKey *)c)->time < t) {
        (RotKey *)a = (RotKey *)b + 1;
        f = (f32)(t - ((RotKey *)b)->time) / (f32)(((RotKey *)a)->time - ((RotKey *)b)->time);
        frac = f;
        u = 1.0f - f;
        q[0] = u * ((RotKey *)b)->q[0] + frac * ((RotKey *)a)->q[0];
        q[1] = u * ((RotKey *)b)->q[1] + frac * ((RotKey *)a)->q[1];
        q[2] = u * ((RotKey *)b)->q[2] + frac * ((RotKey *)a)->q[2];
        q[3] = u * ((RotKey *)b)->q[3] + frac * ((RotKey *)a)->q[3];
    } else {
        q[0] = ((RotKey *)b)->q[0];
        q[1] = ((RotKey *)b)->q[1];
        q[2] = ((RotKey *)b)->q[2];
        q[3] = ((RotKey *)b)->q[3];
    }

    u = 2.0f;
    xx = u * q[1] * q[1];
    yy = u * q[2] * q[2];
    zz = u * q[3] * q[3];
    wx = u * q[0] * q[1];
    wy = u * q[0] * q[2];
    wz = u * q[0] * q[3];
    xy = u * q[1] * q[2];
    xz = u * q[1] * q[3];
    yz = u * q[2] * q[3];

    m[0][0] = (1.0f - yy - zz) * scale[0];
    m[0][1] = (xy - wz) * scale[0];
    m[0][2] = (xz + wy) * scale[0];
    m[0][3] = 0.0f;
    m[1][0] = (xy + wz) * scale[1];
    m[1][1] = (1.0f - xx - zz) * scale[1];
    m[1][2] = (yz - wx) * scale[1];
    m[1][3] = 0.0f;
    m[2][0] = (xz - wy) * scale[2];
    m[2][1] = (yz + wx) * scale[2];
    m[2][2] = (1.0f - xx - yy) * scale[2];
    m[2][3] = 0.0f;
    m[3][0] = trans[0];
    m[3][1] = trans[1];
    m[3][2] = trans[2];
    m[3][3] = 1.0f;
}
