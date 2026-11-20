typedef struct Actor {
    u8 unk_000[0xAC];
    f32 unk_AC;
    f32 unk_B0;
    f32 unk_B4;
    u8 unk_B8[0x1FC - 0xB8];
    s32 unk_1FC;
    s32 unk_200;
} Actor;

extern f64 D_8007B118;
extern f64 D_8007B120;
extern s32 D_80109D4C;
void func_800375A8(Actor *actor, s32 arg1);

/* clears flag 0x10000 of unk_200 when the actor's unk_B4 exceeds a threshold */
void func_8003CEA8(Actor *actor) {
    f32 sp10;
    f32 sp14;
    f32 sp18;

    if ((actor->unk_1FC & 0x80000000) != 0) {
        if (D_80109D4C == 0) {
            return;
        }
        sp10 = actor->unk_AC;
        sp14 = actor->unk_B4;
        sp18 = -actor->unk_B0;
        if (sp14 > D_8007B118) {
            actor->unk_200 = actor->unk_200 & 0xFFFEFFFF;
            func_800375A8(actor, 3);
        } else {
        }
        if (sp14 > D_8007B120) {
            actor->unk_200 = actor->unk_200 & 0xFFFEFFFF;
        }
    }
}
