/* Creates a message queue, binds events 10-13 to it, then creates and starts a manager thread (priority 150) */
typedef void *OSMesg;
typedef struct { s32 data[6]; } OSMesgQueue;
typedef struct { u8 data[0x1B0]; } OSThread;
extern OSMesgQueue D_8013EC88;
extern OSMesg D_800A39C0[8];
extern OSThread D_800A3810;
extern u8 D_800A79E0[];
extern u64 D_800A77E0[0x40];
extern OSMesg D_800A7A00[];
void func_800585B0(OSMesgQueue *mq, OSMesg *msg, s32 count); /* osCreateMesgQueue */
void func_80058CD0(s32 event, OSMesgQueue *mq, OSMesg msg);  /* osSetEventMesg */
void func_800585E0(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri); /* osCreateThread */
void func_8005FBD0(s32 pri, OSMesg *cmdBuf, void *evtQ, s32 cmdMsgCnt);
void func_80058EA0(OSThread *t); /* osStartThread */
void func_80060CC4(void *);

void func_8006614C(void) {
    func_800585B0(&D_8013EC88, D_800A39C0, 8);
    func_80058CD0(10, &D_8013EC88, (OSMesg)2);
    func_80058CD0(11, &D_8013EC88, (OSMesg)4);
    func_80058CD0(12, &D_8013EC88, (OSMesg)8);
    func_80058CD0(13, &D_8013EC88, (OSMesg)0);
    func_800585E0(&D_800A3810, 0, func_80060CC4, NULL, D_800A77E0 + 0x40, 255);
    func_8005FBD0(150, D_800A7A00, D_800A79E0, 8);
    func_80058EA0(&D_800A3810);
}
