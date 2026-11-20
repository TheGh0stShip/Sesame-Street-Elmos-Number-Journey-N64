typedef struct {
    u8 pad0[0x30];
    f32 unk_30; f32 unk_34; f32 unk_38; f32 unk_3C; f32 unk_40; f32 unk_44;
    f32 unk_48; f32 unk_4C; f32 unk_50;
    u8 pad54[0xEC - 0x54];
    s32 unk_EC;
    u8 padF0[0x1DC - 0xF0];
    f32 unk_1DC; f32 unk_1E0; f32 unk_1E4; f32 unk_1E8;
} S;
extern S *D_80106C84;
f32 func_80057E70(f32);
f32 func_800566A0(f32);

/* eases angle 0x1E0 toward target 0x1E8 (wrapping at pi), then rebuilds a direction and cross product */
void func_800324A0(void) {
    register S *p;
    register f32 angle;
    register f32 diff;

    p = D_80106C84;
    if (p == NULL) {
        return;
    }
    angle = p->unk_1E0;
    diff = p->unk_1E8;
    diff -= angle;
    if (((diff >= 0.0f) ? diff : -diff) < 0.01) {
        angle = p->unk_1E8;
        p->unk_EC = 0;
    } else {
        if (p->unk_1DC < 0.07) {
            p->unk_1DC = p->unk_1DC + 0.01;
        }
        if (diff > 3.14159) {
            diff = diff - 6.2831853;
        } else if (diff < -3.14159) {
            diff = diff + 6.2831853;
        }
        angle = angle + diff * p->unk_1DC;
    }
    p->unk_1E0 = angle;
    p->unk_30 = func_80057E70(angle);
    p->unk_38 = func_800566A0(angle);
    p->unk_48 = p->unk_34 * p->unk_44 - p->unk_38 * p->unk_40;
    p->unk_4C = p->unk_38 * p->unk_3C - p->unk_30 * p->unk_44;
    p->unk_50 = p->unk_30 * p->unk_40 - p->unk_34 * p->unk_3C;
}
