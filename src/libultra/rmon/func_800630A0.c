#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/rdb.h"


extern s32 D_80077FE0; /* __osGetCause */

void send(u8 *buf, u32 size) {
    rdbPacket pkt;
    u32 i = 0;
    u32 len, j;
    u8 *src;
    s32 sent;

    if (D_80077FE0 == 0) {
        while (!(__osGetCause() & 0x2000)) {
        }
        *(volatile u32 *)0xC000000C = 0;
        while (__osGetCause() & 0x2000) {
        }
        sent = 0;
    } else {
        sent = 1;
    }
    while (size != 0) {
        len = (size < 4) ? size : 3;
        src = buf + i;
        pkt.type = 12;
        pkt.length = len;
        for (j = 0; j < len; j++) {
            pkt.buf[j] = src[j];
        }
        size -= len;
        i += len;
        *(volatile rdbPacket *)0xC0000000 = pkt;
        if (size != 0) {
            while (!(__osGetCause() & 0x2000)) {
            }
            *(volatile u32 *)0xC000000C = 0;
            while (__osGetCause() & 0x2000) {
            }
        }
    }
    if (sent) {
        while (!(__osGetCause() & 0x2000)) {
        }
        *(volatile u32 *)0xC000000C = 0;
        while (__osGetCause() & 0x2000) {
        }
    }
}
