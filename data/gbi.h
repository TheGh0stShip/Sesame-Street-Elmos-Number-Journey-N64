/* gbi.h: the F3DEX (GBI 1.x) display-list macros used by the program data (subset of the SDK's gbi.h, same encodings). */
#ifndef DATA_GBI_H
#define DATA_GBI_H
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((1U << (w)) - 1)) << (s)))
#define G_MOVEMEM            0x03
#define G_NOOP               0xC0
#define G_RDPPIPESYNC        0xE7
#define G_SETSCISSOR         0xED
#define G_SETCOMBINE         0xFC
#define G_TEXTURE            0xBB
#define G_SETOTHERMODE_H     0xBA
#define G_SETOTHERMODE_L     0xB9
#define G_ENDDL              0xB8
#define G_SETGEOMETRYMODE    0xB7
#define G_CLEARGEOMETRYMODE  0xB6
#define G_MV_VIEWPORT        0x80
/* othermode shifts (GBI 1) */
#define G_MDSFT_ALPHACOMPARE 0
#define G_MDSFT_ZSRCSEL      2
#define G_MDSFT_RENDERMODE   3
#define G_MDSFT_ALPHADITHER  4
#define G_MDSFT_RGBDITHER    6
#define G_MDSFT_COMBKEY      8
#define G_MDSFT_TEXTCONV     9
#define G_MDSFT_TEXTFILT     12
#define G_MDSFT_TEXTLUT      14
#define G_MDSFT_TEXTLOD      16
#define G_MDSFT_TEXTDETAIL   17
#define G_MDSFT_TEXTPERSP    19
#define G_MDSFT_CYCLETYPE    20
#define G_MDSFT_PIPELINE     23
#define gsDma1p(c, s, l, p)            { _SHIFTL(c, 24, 8) | _SHIFTL(p, 16, 8) | _SHIFTL(l, 0, 16), (unsigned int)(s) }
#define gsSPViewport(v)                gsDma1p(G_MOVEMEM, v, 16, G_MV_VIEWPORT)
#define gsSPSetGeometryMode(word)      { _SHIFTL(G_SETGEOMETRYMODE, 24, 8), (unsigned int)(word) }
#define gsSPClearGeometryMode(word)    { _SHIFTL(G_CLEARGEOMETRYMODE, 24, 8), (unsigned int)(word) }
#define gsSPTexture(s, t, level, tile, on) \
    { _SHIFTL(G_TEXTURE, 24, 8) | _SHIFTL(level, 11, 3) | _SHIFTL(tile, 8, 3) | _SHIFTL(on, 0, 8), _SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16) }
#define gsSPEndDisplayList()           { _SHIFTL(G_ENDDL, 24, 8), 0 }
#define gsDPNoOp()                     { _SHIFTL(G_NOOP, 24, 8), 0 }
#define gsDPPipeSync()                 { _SHIFTL(G_RDPPIPESYNC, 24, 8), 0 }
#define gsSPSetOtherMode(cmd, sft, len, data) { _SHIFTL(cmd, 24, 8) | _SHIFTL(sft, 8, 8) | _SHIFTL(len, 0, 8), (unsigned int)(data) }
#define gsDPSetAlphaDither(m)    gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_ALPHADITHER, 2, m)
#define gsDPSetColorDither(m)    gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_RGBDITHER, 2, m)
#define gsDPSetCombineKey(m)     gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_COMBKEY, 1, m)
#define gsDPSetTextureConvert(m) gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTCONV, 3, m)
#define gsDPSetTextureFilter(m)  gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, m)
#define gsDPSetTextureLUT(m)     gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, m)
#define gsDPSetTextureLOD(m)     gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTLOD, 1, m)
#define gsDPSetTextureDetail(m)  gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTDETAIL, 2, m)
#define gsDPSetTexturePersp(m)   gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_TEXTPERSP, 1, m)
#define gsDPSetCycleType(m)      gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_CYCLETYPE, 2, m)
#define gsDPPipelineMode(m)      gsSPSetOtherMode(G_SETOTHERMODE_H, G_MDSFT_PIPELINE, 1, m)
#define gsDPSetAlphaCompare(m)   gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 2, m)
#define gsDPSetDepthSource(m)    gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ZSRCSEL, 1, m)
#define gsDPSetRenderMode(c0, c1) gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_RENDERMODE, 29, (c0) | (c1))
#define gsDPSetScissorFrac(mode, ulx, uly, lrx, lry) \
    { _SHIFTL(G_SETSCISSOR, 24, 8) | _SHIFTL(ulx, 12, 12) | _SHIFTL(uly, 0, 12), _SHIFTL(mode, 24, 8) | _SHIFTL(lrx, 12, 12) | _SHIFTL(lry, 0, 12) }
#define gsDPSetCombineLERP(a0, b0, c0, d0, Aa0, Ab0, Ac0, Ad0, a1, b1, c1, d1, Aa1, Ab1, Ac1, Ad1) \
    { _SHIFTL(G_SETCOMBINE, 24, 8) | _SHIFTL(a0, 20, 4) | _SHIFTL(c0, 15, 5) | _SHIFTL(Aa0, 12, 3) | _SHIFTL(Ac0, 9, 3) | _SHIFTL(a1, 5, 4) | _SHIFTL(c1, 0, 5), \
      _SHIFTL(b0, 28, 4) | _SHIFTL(b1, 24, 4) | _SHIFTL(Aa1, 21, 3) | _SHIFTL(Ac1, 18, 3) | _SHIFTL(d0, 15, 3) | _SHIFTL(Ab0, 12, 3) | _SHIFTL(Ad0, 9, 3) | _SHIFTL(d1, 6, 3) | _SHIFTL(Ab1, 3, 3) | _SHIFTL(Ad1, 0, 3) }
#endif
