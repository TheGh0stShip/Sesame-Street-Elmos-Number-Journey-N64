#include "common.h"
#include "codec/stream.h"

s32 osRecvMesg(void *, void *, s32);

/* streaming update: finishes a pending DMA, starts the next chunk, then decodes/clears ring-buffer blocks */
void func_8006AE74(stream_t *s) {
    s32 n;
    s32 len;
    u32 off;
    u32 wpos;
    u32 size;
    u32 used;

    n = s->frames_per_update;
    if (s->dma_busy != 0) {
        if (osRecvMesg((*(u8 (*)[0x11C8 - 0x11AC])&s->dma_mq), NULL, 0) >= 0) {
            if (s->dma_bytes == 0) {
                s->frames_left = (s->buf[1] >> 16) & 0xFFFF;
                s->sample_rate = s->buf[1] & 0xFFFF;
                s->words_left = s->buf[2] & 0xFFFFFF;
                s->bitpos = 0x60;
            }
            s->dma_bytes += 0x400;
            if (s->words_left > 0x100) {
                s->words_left -= 0x100;
            } else {
                s->words_left = 0;
            }
            s->dma_busy = 0;
        }
    }
    if (s->dma_busy == 0) {
        if (s->state == 1 || s->state == 2 || s->state == 3) {
            off = s->dma_bytes;
            if (off - s->read_bytes <= 0xC00) {
                if (off == 0) {
                    len = 0x400;
                } else if (s->words_left > 0x100) {
                    len = 0x400;
                } else {
                    len = s->words_left * 4;
                }
                if (len > 0) {
                    s->dma_busy = 1;
                    s->dma(s->rom_addr + off, (u8 *)s + ((off & 0xFFF) >> 3) * 8 + 0x1A0, len, (*(u8 (*)[0x11C8 - 0x11AC])&s->dma_mq));
                }
            }
        }
    }
    while ((s->write_pos - s->play_pos) < s->out_size - 0xA0
           && (s->state == 1 || s->frames_left > 0)
           && ((s->dma_bytes != 0 && s->words_left == 0) || s->dma_bytes - s->read_bytes > 0x24)
           && n > 0) {
        func_8006B800(s, s->out_buf + ((s->write_pos % s->out_size) >> 2) * 8);
        s->read_bytes = (s->bitpos >> 3) & ~3;
        s->write_pos += 0xA0;
        if (--s->frames_left == 0) {
            s->drain = s->out_size;
        }
        n--;
    }
    if ((s->state == 3 || s->state == 4) && s->frames_left <= 0) {
        while ((s->write_pos - s->play_pos) < s->out_size - 0xA0 && n > 0) {
            bzero(s->out_buf + ((s->write_pos % s->out_size) >> 2) * 8, 0x140);
            s->write_pos += 0xA0;
            n--;
        }
    }
    if (s->state == 1 && (s->write_pos - s->play_pos) >= s->out_size - 0x140) {
        s->state = 2;
    }
}
