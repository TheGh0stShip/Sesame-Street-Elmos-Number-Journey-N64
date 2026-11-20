#include "common.h"
#include "game_structs.h"


extern Mtx D_8013C730[];
extern Vtx D_8006F6D8[4];
extern Vtx D_8006F718[4];

/* Draws the scrolling sky/backdrop quad, rotated to follow the camera's heading. */
void func_80002EA8(void) {
    Gfx *g;
    s32 unused[4];
    register f32 c;
    register f32 s;
    register f32 x;
    register f32 y;
    register f32 z;
    register f32 dx;
    register f32 dz;
    register f32 ang;

    D_80106C90 += D_8006EE80;
    D_800A7F28 += D_8006EE84;
    D_80106C90 &= 0x7FFF;
    D_800A7F28 &= 0x7FFF;
    x = (*(Actor *(*))&D_80076800)->pos.x;
    y = (*(Actor *(*))&D_80076800)->pos.y;
    z = (*(Actor *(*))&D_80076800)->pos.z;
    dx = (*(Actor *(*))&D_80076800)->fwd.x - x;
    dz = (*(Actor *(*))&D_80076800)->fwd.z - z;
    ang = func_80069920(dx, dz);
    D_8006F758 = ang * 57.2957;
    if (D_8006F758 < 0.0) {
        D_8006F758 += 360.0;
    }
    D_8006F758 += 180.0f;
    guPosition(&D_8013C730[D_800717E0], D_8006F75C, D_8006F758, 0.0f, 1.0f, x, y, z);
    g = D_8013EC7C++;
    g->words.w0 = 0x01020040;
    g->words.w1 = (u32)(D_800717E0 * sizeof(Mtx) + (u8 *)D_8013C730 - 0x80000000);
    c = __sinf(-ang);
    s = __cosf(-ang);
    if (D_800717E0 == 0) {
        D_8006F6D8[0].v.tc[0] = -c * -4096.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[0].v.tc[1] = s * -4096.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F6D8[1].v.tc[0] = -c * -4096.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[1].v.tc[1] = s * -4096.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F6D8[2].v.tc[0] = -c * 0.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[2].v.tc[1] = s * 0.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F6D8[3].v.tc[0] = -c * 0.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[3].v.tc[1] = s * 0.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
    } else {
        D_8006F718[0].v.tc[0] = -c * -4096.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[0].v.tc[1] = s * -4096.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F718[1].v.tc[0] = -c * -4096.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[1].v.tc[1] = s * -4096.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F718[2].v.tc[0] = -c * 0.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[2].v.tc[1] = s * 0.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F718[3].v.tc[0] = -c * 0.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[3].v.tc[1] = s * 0.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
    }
    g = D_8013EC7C++;
    g->words.w0 = 0xE7000000;
    g->words.w1 = 0;
    g = D_8013EC7C++;
    g->words.w0 = 0xBA001402;
    g->words.w1 = 0;
    g = D_8013EC7C++;
    g->words.w0 = 0xB6000000;
    g->words.w1 = 0x009F3207;
    g = D_8013EC7C++;
    g->words.w0 = 0xB900031D;
    g->words.w1 = 0x0F0A4000;
    g = D_8013EC7C++;
    g->words.w0 = 0xFCFFFFFF;
    g->words.w1 = 0xFFFCF87C;
    g = D_8013EC7C++;
    g->words.w0 = 0xBA000C02;
    g->words.w1 = 0x2000;
    g = D_8013EC7C++;
    g->words.w0 = 0xFD10001F;
    g->words.w1 = (u32)D_8006EE88;
    g = D_8013EC7C++;
    g->words.w0 = 0xBB000001;
    g->words.w1 = 0xFFFFFFFF;
    g = D_8013EC7C++;
    g->words.w0 = 0xF5101000;
    g->words.w1 = 0x07000000;
    g = D_8013EC7C++;
    g->words.w0 = 0xE6000000;
    g->words.w1 = 0;
    g = D_8013EC7C++;
    g->words.w0 = 0xF3000000;
    g->words.w1 = 0x07400000;
    g = D_8013EC7C++;
    g->words.w0 = 0xF5101000;
    g->words.w1 = 0x00014050;
    g = D_8013EC7C++;
    g->words.w0 = 0xF2000000;
    g->words.w1 = 0x0007C07C;
    if (D_800717E0 == 0) {
        g = D_8013EC7C++;
        g->words.w0 = 0x0400103F;
        g->words.w1 = (u32)D_8006F6D8;
    } else {
        g = D_8013EC7C++;
        g->words.w0 = 0x0400103F;
        g->words.w1 = (u32)D_8006F718;
    }
    g = D_8013EC7C++;
    g->words.w0 = 0xB1000402;
    g->words.w1 = 0x604;
    g = D_8013EC7C++;
    g->words.w0 = 0xE7000000;
    g->words.w1 = 0;
}
