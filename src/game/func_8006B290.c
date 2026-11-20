#include "common.h"
#include "codec/stream.h"

extern void func_8006CA74(stream_t *, s32, s32);
/* converts 8 raw parameters to Q15 coefficients into the inactive bank, then flips banks */
void func_8006B290(stream_t *ctx, s16 *in, s32 arg2, s32 arg3) {
    s16 *out = (*(Coefs (*)[2])&ctx->LARpp)[ctx->j].v;
    out[0] = ((((in[0] - 32) << 10) * 0x3333 + 0x4000) >> 15) * 2;
    out[1] = ((((in[1] - 32) << 10) * 0x3333 + 0x4000) >> 15) * 2;
    out[2] = ((((in[2] - 16) << 10) * 0x3333 + (s32)0xFCCD1000) >> 15) * 2;
    out[3] = ((((in[3] - 16) << 10) * 0x3333 + 0x04003C00) >> 15) * 2;
    out[4] = ((((in[4] - 8) << 10) * 0x4B17 + (s32)0xFFC91B1C) >> 15) * 2;
    out[5] = ((((in[5] - 8) << 10) * 0x4444 + 0x03BBF800) >> 15) * 2;
    out[6] = ((((in[6] - 4) << 10) * 0x7ADE + 0x0147936C) >> 15) * 2;
    out[7] = ((((in[7] - 4) << 10) * 0x740C + 0x040D6B40) >> 15) * 2;
    func_8006CA74(ctx, arg2, arg3);
    ctx->j ^= 1;
}
