/* rmonMain: rmon thread loop dispatching break/fault messages */
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;
extern s32 func_80058A70(OSMesgQueue *, OSMesg *, s32); /* osRecvMesg */
extern void func_8006614C(void); /* __rmonInit */
extern void func_80065574(void); /* __rmonHitBreak */
extern void func_80065650(void); /* __rmonHitSpBreak */
extern void func_800656DC(void); /* __rmonHitCpuFault */
extern OSMesgQueue D_8013EC88;   /* __rmonMQ */
extern int D_80077F80;           /* __rmonActive */
extern volatile u32 D_800A1FC0;  /* somethingToDo */
extern u32 D_800A1FD0[280];      /* inbuffer */
extern u8 D_800A2430;            /* cmdinptr */
extern u8 D_800A2431;            /* cmdoutptr */
extern int D_800A2434;           /* state */
extern char *D_800A2438;         /* inPointer */

void func_80060A40(void)
{
    register int newChars;

    D_800A1FC0 = 0;
    D_800A2431 = 0;
    D_800A2430 = 0;

    func_8006614C();
    D_80077F80 = 1;

    D_800A2434 = 0, newChars = 0, D_800A2438 = (void *)&D_800A1FD0;
    for (;;) {
        OSMesg work;

        func_80058A70(&D_8013EC88, &work, 1);

        D_800A1FC0 |= (u32)work;

        if (D_800A1FC0 & 2) {
            D_800A1FC0 &= ~2;
            func_80065574();
        }
        if (D_800A1FC0 & 4) {
            D_800A1FC0 &= ~4;
            func_80065650();
        }
        if (D_800A1FC0 & 8) {
            D_800A1FC0 &= ~8;
            func_800656DC();
        }
        if (D_800A1FC0 & 0x10) {
            D_800A1FC0;
            D_800A1FC0 &= (u8)~0x10;
        }
        if (D_800A1FC0 & 0x20) {
            D_800A1FC0;
            D_800A1FC0 &= (u8)~0x20;
        }
    }
}
