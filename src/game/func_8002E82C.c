#include "common.h"

typedef struct {
    u8 pad0[0x24];
    f32 eye[3];
    f32 at[3];
    f32 up[3];
} Camera;
typedef struct Obj {
    u8 pad0[0x24];
    f32 pos[3];
    f32 fwd[3];
    f32 up[3];
    f32 side[3];
    u8 pad54[0x60 - 0x54];
    u8 unk_60[0x7C - 0x60];
    f32 unk_7C;
    f32 unk_80;
    f32 unk_84;
    u8 pad88[0x9C - 0x88];
    s32 unk_9C;
    u8 padA0[0xAC - 0xA0];
    f32 unk_AC;
    f32 unk_B0;
    f32 unk_B4;
    u8 padB8[0xBC - 0xB8];
    u8 unk_BC[0xD0 - 0xBC];
    s32 unk_D0;
    u8 padD4[0xD8 - 0xD4];
    f32 unk_D8;
    f32 unk_DC;
    f32 unk_E0;
    u8 padE4[0x1AC - 0xE4];
    f32 unk_1AC;
    u8 pad1B0[4];
    f32 unk_1B4;
    f32 dir[3];
    u8 pad1C4[0x1E4 - 0x1C4];
    f32 unk_1E4;
    u8 pad1E8[0x1F4 - 0x1E8];
    f32 unk_1F4;
    u8 pad1F8[0x1FC - 0x1F8];
    s32 unk_1FC;
    s32 unk_200;
    s32 unk_204;
} Obj;

extern Obj *D_80106C84;
void func_8002E35C(f32 *);
s32 func_800208D0(void *, void *, f32 *, f32 *, f32 *, s32 *);

#define ABS(x) ((x) >= 0.0f ? (x) : -(x))

/* Moves the player object from stick input relative to the camera, turns it and collides it. */
void func_8002E82C(void) {
    f32 sx;
    f32 sy;
    f32 cam[3];
    f32 cross[3];
    f32 pos[3];
    f32 dir[3];
    f32 len;
    f32 amt;
    s32 r;
    s32 flags;
    f32 d70;
    f32 d74;
    s32 d78;
    f64 inv;
    register Obj *p;
    register f32 k;

    p = D_80106C84;
    if (p == NULL) {
        return;
    }
    k = 0.15f;
    p->unk_204 = 0;
    sx = D_80107CBC;
    sy = D_80107CBD;
    func_8004DCD0(D_800A7F1C, sx, sy);
    cam[0] = (*(Camera *(*))&D_80076800)->at[0] - (*(Camera *(*))&D_80076800)->eye[0];
    cam[1] = (*(Camera *(*))&D_80076800)->at[1] - (*(Camera *(*))&D_80076800)->eye[1];
    cam[2] = (*(Camera *(*))&D_80076800)->at[2] - (*(Camera *(*))&D_80076800)->eye[2];
    pos[0] = p->pos[0];
    pos[1] = p->pos[1];
    pos[2] = p->pos[2];
    dir[0] = p->dir[0];
    dir[1] = p->dir[1];
    dir[2] = p->dir[2];
    if (p->unk_200 & 3) {
        if (ABS(sx) > 8.0f) {
            cross[0] = cam[1] * (*(Camera *(*))&D_80076800)->up[2] - cam[2] * (*(Camera *(*))&D_80076800)->up[1];
            cross[1] = cam[2] * (*(Camera *(*))&D_80076800)->up[0] - cam[0] * (*(Camera *(*))&D_80076800)->up[2];
            cross[2] = cam[0] * (*(Camera *(*))&D_80076800)->up[1] - cam[1] * (*(Camera *(*))&D_80076800)->up[0];
            len = sqrtf(cross[0] * cross[0] + cross[2] * cross[2]);
            cross[0] = cross[0] / len;
            cross[2] = cross[2] / len;
            sx = sx * (sx >= 0.0f ? sx : -sx);
            sx = sx * 0.01f;
            amt = sx * cos((*(Unk112E90 *(*))&D_80112E90)->unk_FC + 1.570795) * k;
            pos[0] = pos[0] + amt;
            dir[0] = dir[0] + amt;
            amt = sx * sin((*(Unk112E90 *(*))&D_80112E90)->unk_FC + 1.570795) * k;
            pos[2] = pos[2] + amt;
            dir[2] = dir[2] + amt;
            p->unk_204 = 1;
        }
        if (ABS(sy) > 8.0f) {
            len = sqrtf(cam[0] * cam[0] + cam[2] * cam[2]);
            cam[0] = cam[0] / len;
            cam[2] = cam[2] / len;
            sy = sy * (sy >= 0.0f ? sy : -sy);
            sy = sy * 0.01f;
            amt = sy * cos((*(Unk112E90 *(*))&D_80112E90)->unk_FC) * k;
            pos[0] = pos[0] + amt;
            dir[0] = dir[0] + amt;
            amt = sy * sin((*(Unk112E90 *(*))&D_80112E90)->unk_FC) * k;
            pos[2] = pos[2] + amt;
            dir[2] = dir[2] + amt;
            p->unk_204 = 1;
        }
        if (p->unk_204 == 0) {
            func_8002E568(&sx, &sy);
            if (p->unk_204 == 1) {
                amt = sx * cos((*(Unk112E90 *(*))&D_80112E90)->unk_FC + 1.570795) * k;
                pos[0] = pos[0] + amt;
                dir[0] = dir[0] + amt;
                amt = sx * sin((*(Unk112E90 *(*))&D_80112E90)->unk_FC + 1.570795) * k;
                pos[2] = pos[2] + amt;
                dir[2] = dir[2] + amt;
                amt = sy * cos((*(Unk112E90 *(*))&D_80112E90)->unk_FC) * k;
                pos[0] = pos[0] + amt;
                dir[0] = dir[0] + amt;
                amt = sy * sin((*(Unk112E90 *(*))&D_80112E90)->unk_FC) * k;
                pos[2] = pos[2] + amt;
                dir[2] = dir[2] + amt;
            }
        }
    }
    if (!(p->unk_200 & 1)) {
        pos[0] = p->pos[0];
        pos[2] = p->pos[2];
    }
    if (!(p->unk_200 & 2)) {
        dir[0] = p->fwd[0];
        dir[1] = p->fwd[1];
        dir[2] = p->fwd[2];
    }
    if (p->unk_204 != 0) {
        if (D_800AD039 >= 3) {
            D_800AD039 = 0;
            D_800A7F18 = 0;
        }
        len = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
        if (len >= 1e-7f) {
            inv = 1.0 / len;
            dir[0] = dir[0] * inv;
            dir[1] = dir[1] * inv;
            dir[2] = dir[2] * inv;
        } else {
        }
        if (len > 1e-7f) {
            p->dir[0] = dir[0];
            p->dir[1] = dir[1];
            p->dir[2] = dir[2];
        }
    }
    sx = func_80069920(p->fwd[0], p->fwd[2]);
    sy = func_80069920(p->dir[0], p->dir[2]);
    sy = sy - sx;
    if (ABS(sy) < 0.03) {
        p->fwd[0] = p->dir[0];
        p->fwd[1] = p->dir[1];
        p->fwd[2] = p->dir[2];
    } else {
        if (sy > 3.14159) {
            sy = sy - 6.2831853;
        } else if (sy < -3.14159) {
            sy = sy + 6.2831853;
        }
        sx = sx + sy * 0.2;
        p->fwd[0] = __sinf(sx);
        p->fwd[2] = __cosf(sx);
        p->unk_204 = 1;
    }
    p->side[0] = p->fwd[1] * p->up[2] - p->fwd[2] * p->up[1];
    p->side[1] = p->fwd[2] * p->up[0] - p->fwd[0] * p->up[2];
    p->side[2] = p->fwd[0] * p->up[1] - p->fwd[1] * p->up[0];
    pos[1] = pos[1] + p->unk_1E4;
    p->unk_D0 = 1;
    func_8002E35C(pos);
    flags = 0x22000010;
    r = func_800208D0(p->unk_60, p->unk_BC, p->pos, pos, dir, &flags);
    p->unk_1FC = flags;
    if (r == 2) {
        d78 = p->unk_9C;
    }
    if (flags & 0x80000000) {
        D_80106C60 = p->unk_AC;
        D_80106C64 = p->unk_B4;
        D_80106C68 = -p->unk_B0;
        d70 = D_80106C60 * p->unk_7C + D_80106C64 * p->unk_80 + D_80106C68 * p->unk_84;
        if (D_80106C64 >= 0.0 && D_80106C64 < 0.707 && !(p->unk_200 & 0x10000)) {
            d74 = 0.707 - D_80106C64;
            p->unk_1E4 = p->unk_1E4 + d74 * 1.414 * p->unk_1F4;
        }
        if (D_80106C64 >= 0.707) {
            p->unk_1E4 = 0.0f;
            p->unk_200 = p->unk_200 & ~0x10000;
        } else {
            p->unk_1E4 = p->unk_1F4;
        }
    } else if (p->pos[1] > D_8013C720 + 75.0 && p->pos[1] - (D_8013C720 + 75.0) > 2.0) {
        p->unk_1E4 = p->unk_1F4;
    }
    p->unk_1AC = pos[0] - p->pos[0];
    p->unk_1B4 = pos[2] - p->pos[2];
    D_800A7A70 = D_800A7A70 + sqrtf(p->unk_1AC * p->unk_1AC + p->unk_1B4 * p->unk_1B4);
    if (D_8012F158 != 0 && D_800A7A70 > D_8012F158) {
        func_800375A8(D_80106C84, 0x22);
    }
    p->pos[0] = pos[0];
    p->pos[1] = pos[1];
    p->pos[2] = pos[2];
    p->unk_D8 = pos[0];
    p->unk_DC = -pos[2];
    p->unk_E0 = pos[1];
}
