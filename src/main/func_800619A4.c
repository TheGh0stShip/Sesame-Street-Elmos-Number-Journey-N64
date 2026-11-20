/* __scAppendList (scheduler): append a task to the audio or graphics list */
typedef struct { u32 type; } OSTaskHdr;
typedef struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    OSTaskHdr list;
} OSScTask;
typedef struct {
    u8 pad[0x264];
    OSScTask *audioListHead;
    OSScTask *gfxListHead;
    OSScTask *audioListTail;
    OSScTask *gfxListTail;
    u8 pad2[0x280 - 0x274];
    s32 doAudio;
} OSSched;
void func_800619A4(OSSched *sc, OSScTask *t)
{
    long type = t->list.type;
    if (type == 2) {
        if (sc->audioListTail)
            sc->audioListTail->next = t;
        else
            sc->audioListHead = t;
        sc->audioListTail = t;
        sc->doAudio = 1;
    } else {
        if (sc->gfxListTail)
            sc->gfxListTail->next = t;
        else
            sc->gfxListHead = t;
        sc->gfxListTail = t;
    }
    t->next = NULL;
    t->state = t->flags & 3;
}
