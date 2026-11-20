/* __rmonListThreads */
typedef struct { s32 length; u8 code; u8 type; s16 error; u8 rev; u8 method; s16 notused; } KKHeader;
typedef struct { KKHeader header; s32 object; } KKObjectRequest;
typedef struct { u16 number; s32 objects[1]; } KKObjs;
typedef struct { KKHeader header; s32 object; KKObjs objs; } KKObjsEvent;
typedef struct OSThread_s {
    struct OSThread_s *next; s32 priority; void *queue; struct OSThread_s *tlnext;
    u16 state; u16 flags; s32 id;
} OSThread;
extern u8 D_80115418[];             /* __rmonUtilityBuffer */
OSThread *func_80068350(void);      /* __osGetActiveQueue */
s32 func_80060898(KKHeader *h, s32 size, s32 type); /* __rmonSendReply */

int func_80066ED4(KKHeader *req) {
    register KKObjectRequest *request = (KKObjectRequest *)req;
    KKObjsEvent *reply = (KKObjsEvent *)D_80115418;

    reply->object = (request->object == -1) ? 1002 : request->object;

    if (req->method == 1) {
        reply->objs.number = 1;
        reply->objs.objects[0] = 1000;
    } else {
        register OSThread *tptr = func_80068350();

        reply->objs.number = 0;
        while (tptr->priority != -1) {
            if (tptr->id != 0) {
                reply->objs.objects[reply->objs.number] = tptr->id;
                reply->objs.number++;
            }
            tptr = tptr->tlnext;
        }
    }
    reply->header.code = request->header.code;
    reply->header.error = 0;
    func_80060898(&reply->header, sizeof(*reply) + sizeof(reply->objs.objects[0]) * (reply->objs.number - 1), 1);
    return 0;
}
