/* osSetEventMesg */
typedef struct { void *messageQueue; void *message; } __OSEventState;
extern __OSEventState D_80133EF8[];
u32 func_80062C60(void); /* __osDisableInt */
void func_80062C80(u32 mask); /* __osRestoreInt */
void func_80058CD0(s32 event, void *mq, void *msg) {
    register u32 saveMask = func_80062C60();
    __OSEventState *es = &D_80133EF8[event];
    es->messageQueue = mq;
    es->message = msg;
    func_80062C80(saveMask);
}
