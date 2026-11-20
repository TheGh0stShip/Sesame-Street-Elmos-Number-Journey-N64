typedef struct {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ u8 unk2C[0x60 - 0x2C];
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
    /* 0x8C */ f32 unk8C;
} Cam;

extern f64 D_8007B408;
f32 func_80056690(f32);
f32 func_800566A0(f32);
f32 func_80057E70(f32);

/* builds a direction vector from two angles, a point behind it and the normalised side vector */
void func_8004A050(Cam *arg0) {
    f32 len;
    f64 inv;

    arg0->unk84 = 0;
    arg0->unk88 = 1.0f;
    arg0->unk8C = 0;
    arg0->unk70 = ({ -func_80057E70(arg0->unk20); });
    arg0->unk6C = ({ func_800566A0(arg0->unk1C) * func_800566A0(arg0->unk20); });
    arg0->unk74 = ({ func_80057E70(arg0->unk1C) * func_800566A0(arg0->unk20); });
    arg0->unk60 = arg0->unk10 - arg0->unk6C * arg0->unk28;
    arg0->unk64 = arg0->unk14 - arg0->unk70 * arg0->unk28;
    arg0->unk68 = arg0->unk18 - arg0->unk74 * arg0->unk28;
    arg0->unk78 = arg0->unk70 * arg0->unk8C - arg0->unk74 * arg0->unk88;
    arg0->unk7C = arg0->unk74 * arg0->unk84 - arg0->unk6C * arg0->unk8C;
    arg0->unk80 = arg0->unk6C * arg0->unk88 - arg0->unk70 * arg0->unk84;
    len = func_80056690(arg0->unk78 * arg0->unk78 + arg0->unk7C * arg0->unk7C + arg0->unk80 * arg0->unk80);
    if (len >= 1e-07f) {
        inv = D_8007B408 / len;
        arg0->unk78 = arg0->unk78 * inv;
        arg0->unk7C = arg0->unk7C * inv;
        arg0->unk80 = arg0->unk80 * inv;
    } else {
    }
}
