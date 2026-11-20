/* rmon/rdb thread loop: wait for the RDB event, process, acknowledge with a one-byte send */
typedef void *OSMesg;
typedef struct { u8 pad[0x18]; } OSMesgQueue;
extern OSMesgQueue D_800A2440;
extern OSMesg D_800A2458[1];
extern u8 D_80117A40[];
extern u8 *D_800A7F04;
void func_800585B0(OSMesgQueue *, OSMesg *, s32); /* osCreateMesgQueue */
void func_80058CD0(s32, OSMesgQueue *, OSMesg); /* osSetEventMesg */
s32 func_80058A70(OSMesgQueue *, OSMesg *, s32); /* osRecvMesg */
void func_80065910(u8 *);
s32 func_80063440(char *, u32, u32); /* __osRdbSend */

void func_80060CC4(void)
{
    char buf;
    int sent;

    func_800585B0(&D_800A2440, D_800A2458, 1);
    func_80058CD0(0x14, &D_800A2440, 0);
    D_800A7F04 = D_80117A40;
    for (;;) {
        func_80058A70(&D_800A2440, NULL, 1);
        func_80065910(D_80117A40);
        D_800A7F04 = D_80117A40;
        sent = 0;
        while (sent <= 0)
            sent += func_80063440(&buf, 1, 11);
    }
}
