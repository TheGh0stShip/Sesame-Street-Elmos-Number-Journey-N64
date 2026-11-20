#include "common.h"
#include "codec/stream.h"

void func_8006ADB0(stream_t *s, s32 a, s32 b, s32 c, s32 d) {
    bzero(s, 0x190);
    s->nrp = 0x28;
    s->dma = d;
    s->rom_addr = a;
    s->out_buf = b;
    s->out_size = c;
    s->drain = 0;
    osCreateMesgQueue(s->dma_mq, s->dma_msg, 1);
    if (a == 0 || d == 0) s->state = 0; else s->state = 1;
    s->dma_busy = 0;
    s->sample_rate = 0;
    s->frames_left = s->words_left = (u16)s->sample_rate;
    s->bitpos = 0;
    s->dma_bytes = 0;
    s->read_bytes = 0;
    s->write_pos = 0;
    s->play_pos = 0;
    s->frames_per_update = 2;
}
