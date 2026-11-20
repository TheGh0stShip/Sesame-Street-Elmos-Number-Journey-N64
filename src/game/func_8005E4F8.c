#include "common.h"

typedef struct { u32 w0, w1; } Gfx;
typedef struct {
    s16 width; s16 width_img; s16 s; s16 t; void *buf; s16 actualHeight; s16 LUToffset;
} Bitmap;
typedef struct {
    s16 x, y; s16 width, height; f32 scalex, scaley; s16 expx, expy;
    u16 attr; u16 zdepth; u8 red, green, blue, alpha; s16 startTLUT, nTLUT;
    s32 *LUT; s16 istart, istep; s16 nbitmaps, ndisplist; s16 bmheight, bmHsize;
    u8 bmfmt, bmsiz; u8 pad32[2]; Bitmap *bitmap; Gfx *rsp_dl; Gfx *rsp_dl_next;
    s16 frac_s, frac_t;
} Sprite;
void drawbitmap(Gfx **, Sprite *, Bitmap *, s32, s32, s32, s32, s32, s32, s32, s32);

#define EMIT(a, b) { Gfx *g_ = gfx++; g_->w0 = (a); g_->w1 = (b); }

/* libultra-style spDraw: builds the display list for a sprite */
Gfx *func_8005E4F8(Sprite *s) {
    Gfx *gfx;
    Gfx *dl_start;
    Bitmap *b;
    s32 i;
    s32 tx, ty;
    s32 sx, sy;
    s32 ex, ey;
    s32 s0, t0;
    s32 isx, isy;
    s32 rx, ry;
    s32 offx, offy;
    f32 scalex, scaley;
    f32 fx, fy;
    s32 bh;

    if (s->attr & 4) {
        return NULL;
    }
    gfx = s->rsp_dl_next;
    D_80077F10 = 0;
    if (gfx == NULL) {
        gfx = s->rsp_dl;
    }
    b = s->bitmap;
    offx = 0;
    dl_start = gfx;
    offy = 0;
    if (D_80077F14 & 0x400) {
        D_80077F14 = ~s->attr;
    }
    if (s->attr & 0x400) {
        D_80077F14 = s->attr;
    }
    if (s->attr != D_80077F14) {
        if (s->attr & 1) {
            if (!(D_80077F14 & 1)) {
                EMIT(0xB900031D, 0x504240);
            }
        } else if (D_80077F14 & 1) {
            EMIT(0xB900031D, 0x0F0A7008);
        }
        if (s->attr & 2) {
            if (!(D_80077F14 & 2)) {
                EMIT(0xF9000000, 0xFFFFFF01);
                EMIT(0xB9000002, 1);
            }
        } else if (D_80077F14 & 2) {
            EMIT(0xB9000002, 0);
        }
        if (s->attr & 0x80) {
            if (!(D_80077F14 & 0x80)) {
                EMIT(0xBA000C02, 0x3000);
            }
        } else if (D_80077F14 & 0x80) {
            EMIT(0xBA000C02, 0x2000);
        }
    }
    D_80077F14 = s->attr;
    if (D_80077F14 & 8) {
        EMIT(0xEE000000, s->zdepth << 16);
    }
    EMIT(0xFA000000, (s->red << 24) | (s->green << 16) | (s->blue << 8) | s->alpha);
    if (s->bmfmt == 3 || s->bmfmt == 4) {
        if (s->alpha == 0xFF) {
            EMIT(0xFC11FE23, 0xFFFFF3F9);
        } else {
            EMIT(0xFC119623, 0xFF2FFFFF);
        }
    } else if (s->alpha == 0xFF) {
        EMIT(0xFC11FE23, 0xFFFFF3F9);
    } else {
        EMIT(0xFC119623, 0xFF2FFFFF);
    }
    if (s->bmfmt == 2) {
        EMIT(0xBA000E02, 0x8000);
        EMIT(0xFD100000, (s32)s->LUT);
        EMIT(0xE8000000, 0);
        EMIT(0xF5000000 | ((s->startTLUT + 0x100) & 0x1FF), 0x07000000);
        EMIT(0xE6000000, 0);
        EMIT(0xF0000000, 0x07000000 | (((s->nTLUT - 1) & 0x3FF) << 14));
        EMIT(0xE7000000, 0);
        EMIT(0xE6000000, 0);
    }
    if (s->bmfmt == 1) {
        if (s->attr & 0x20) {
            EMIT(0xBA000903, 0);
            EMIT(0xBA000C02, 0);
            EMIT(0xFC17FE2F, 0x77FCF87C);
            EMIT(0xEC15FD5D, 0x3B78E42A);
        } else {
            EMIT(0xBA001402, 0x100000);
            EMIT(0xBA000C02, 0x2000);
            EMIT(0xBA000903, 0xA00);
            EMIT(0xEC15FD5D, 0x3B78E42A);
            EMIT(0xFC27FFFF, 0x7FFD7E38);
        }
    }
    if (s->attr & 0x10) {
        scalex = s->scalex;
        scaley = s->scaley;
    } else {
        scalex = scaley = 1.0f;
    }
    isx = (s32)(1024.0f / scalex + 0.5f);
    isy = (s32)(1024.0f / scaley + 0.5f);
    if (b != NULL) {
        if ((s->attr & 0x20) && s->bmfmt != 1) {
            EMIT(0xBA001402, 0x200000);
        }
        ty = 0;
        fy = s->y + ty * scaley;
        sy = (s32)(fy + 0.99999f);
        t0 = (s32)(isy * (sy - fy));
        t0 = (t0 + 0x10) >> 5;
        tx = 0;
        if (s->attr & 0x80) {
            t0 += 0x10;
        }
        if (s->attr & 0x100) {
            t0 += s->frac_t;
        }
        ry = ty + s->bmheight;
        ey = (s32)(s->y + ry * scaley + 0.99999f);
        if (s->attr & 0x20) {
            ey -= s->bmfmt != 1;
        }
        for (i = 0; i < s->nbitmaps; i++, b++) {
            if (b->width <= 0) {
                break;
            }
            if (s->width < tx + b->width) {
                ty += s->bmheight;
                fy = s->y + ty * scaley;
                sy = (s32)(fy + 0.9999f);
                tx = 0;
                offx = 0;
                offy += s->expy;
                t0 = (s32)(isy * (sy - fy));
                t0 = (t0 + 0x10) >> 5;
                ry = ty + s->bmheight;
                if (s->attr & 0x80) {
                    t0 += 0x10;
                }
                if (s->attr & 0x100) {
                    t0 += s->frac_t;
                }
                sy += offy;
                ey = (s32)(s->y + ry * scaley + 0.9999f);
                ey += offy;
                if (s->attr & 0x20) {
                    ey -= s->bmfmt != 1;
                }
                if (b->actualHeight != 0) {
                    bh = b->actualHeight;
                } else {
                    bh = s->bmheight;
                }
                if (s->height < ty + bh) {
                    break;
                }
            }
            fx = s->x + tx * scalex;
            sx = (s32)(fx + 0.9999f);
            s0 = (s32)(isx * (sx - fx));
            s0 = (s0 + 0x10) >> 5;
            if (s->attr & 0x80) {
                s0 += 0x10;
            }
            if (s->attr & 0x100) {
                s0 += s->frac_s;
            }
            rx = tx + b->width;
            ex = (s32)(s->x + rx * scalex + 0.9999f);
            if (b->actualHeight != 0) {
                ry = ty + b->actualHeight;
                ey = (s32)(s->y + ry * scaley + 0.9999f);
                ey += offy;
                if (s->attr & 0x20) {
                    ey -= s->bmfmt != 1;
                }
            }
            sx += offx;
            ex += offx;
            if (s->attr & 0x20) {
                ex -= s->bmfmt != 1;
            }
            if (b->buf != NULL) {
                drawbitmap(&gfx, s, b, sx, sy, ex, ey, s0, t0, isx, isy);
            }
            tx += b->width;
            offx += s->expx;
        }
        if (s->attr & 0x20) {
            EMIT(0xBA001402, 0);
        }
        if (s->bmfmt == 1) {
            EMIT(0xBA001402, 0);
            EMIT(0xBA000C02, 0x2000);
            EMIT(0xBA000903, 0xC00);
        }
    } else {
        tx = s->x;
        ty = s->y;
        rx = (s32)(tx + s->width * scalex - 1.0f);
        ry = (s32)(ty + s->height * scaley - 1.0f);
        if (tx < D_800A07E0 && ty < D_800A07E4 && rx >= D_800A07E8 && ry >= D_800A07EC) {
            u32 c;
            if (tx < D_800A07E8) tx = D_800A07E8;
            if (rx >= D_800A07E0) rx = D_800A07E0 - 1;
            if (ty < D_800A07EC) ty = D_800A07EC;
            if (ry >= D_800A07E4) ry = D_800A07E4 - 1;
            c = ((s->red << 8) & 0xF800) | ((s->green << 3) & 0x7C0) | ((s->blue >> 2) & 0x3E) | (s->alpha >> 7);
            EMIT(0xBB000000, 0x80008000);
            EMIT(0xBA001402, 0x300000);
            EMIT(0xF7000000, (c << 16) | c);
            EMIT(0xF6000000 | ((rx & 0x3FF) << 14) | ((ry & 0x3FF) << 2), ((tx & 0x3FF) << 14) | ((ty & 0x3FF) << 2));
            EMIT(0xBA001402, 0);
            EMIT(0xBB000001, 0x80008000);
        }
    }
    if (s->bmfmt == 2) {
        EMIT(0xBA000E02, 0);
    }
    EMIT(0xB8000000, 0);
    s->rsp_dl_next = gfx;
    return dl_start;
}
