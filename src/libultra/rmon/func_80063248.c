#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/rdb.h"
/* not in the SDK headers */
extern u32 D_80077FD0;
void send(u8 *, u32);
extern u8 D_800A2460[];
/* numChars */
         /* debug_buffer */
extern OSThread *D_80077C30;/* __osRunningThread */
  /* send */

static u32 string_to_u32(u8 *s)
{
    u32 k;
    k = ((s[0] & 0xFF) << 24);
    k |= ((s[1] & 0xFF) << 16);
    k |= ((s[2] & 0xFF) << 8);
    k |= (s[3] & 0xFF);
    return k;
}

static void process_command_memory(void)
{
    u32 addr = string_to_u32(&D_800A2460[1]);
    u32 length = string_to_u32(&D_800A2460[5]);
    send((u8 *)addr, length);
}

static void process_command_register(void)
{
    send((u8 *)&D_80077C30->context, 0x190);
}

void kdebugserver(rdbPacket packet)
{
    u32 i;

    for (i = 0; i < 3; i++) {
        D_800A2460[D_80077FD0++] = packet.buf[i];
    }
    if (D_800A2460[0] == 2) {
        process_command_register();
        D_80077FD0 = 0;
    } else if (D_80077FD0 >= 9 && D_800A2460[0] == 1) {
        process_command_memory();
        D_80077FD0 = 0;
    }
}
