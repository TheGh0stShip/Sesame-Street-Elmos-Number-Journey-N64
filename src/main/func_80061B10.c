/* __scSchedule (sample scheduler): pick audio/graphics tasks for the free RSP/RDP, recursively */
typedef struct OSScTask_s {
    struct OSScTask_s *next; u32 state; u32 flags;
} OSScTask;
typedef struct {
    u8 pad[0x264];
    OSScTask *audioListHead;  /* 0x264 */
    OSScTask *gfxListHead;    /* 0x268 */
    OSScTask *audioListTail;  /* 0x26C */
    OSScTask *gfxListTail;    /* 0x270 */
    s32 unk_274; s32 unk_278; s32 unk_27C;
    s32 doAudio;              /* 0x280 */
} OSSched;

void *func_80063C00(void); /* osViGetCurrentFramebuffer */
void *func_80063C40(void); /* osViGetNextFramebuffer */

#define OS_SC_DP 0x0001
#define OS_SC_SP 0x0002
#define OS_SC_YIELD 0x0010
#define OS_SC_YIELDED 0x0020
#define OS_SC_NEEDS_RDP 0x0001
#define OS_SC_NEEDS_RSP 0x0002
#define OS_SC_DRAM_DLIST 0x0004
#define OS_SC_PARALLEL_TASK 0x0010
#define OS_SC_TYPE_MASK 0x0007
#define OS_SC_DP_XBUS (OS_SC_NEEDS_RDP)
#define OS_SC_DP_DRAM (OS_SC_NEEDS_RDP | OS_SC_DRAM_DLIST)
#define OS_SC_SP_XBUS (OS_SC_NEEDS_RSP)
#define OS_SC_SP_DRAM (OS_SC_NEEDS_RSP | OS_SC_DRAM_DLIST)
#define OS_SC_XBUS (OS_SC_SP_XBUS | OS_SC_DP_XBUS)
#define OS_SC_DRAM (OS_SC_SP_DRAM | OS_SC_DP_DRAM)

static OSScTask *__scTaskReady(OSScTask *t)
{
    if (t) {
        if (func_80063C00() != func_80063C40()) {
            return 0;
        }
        return t;
    }
    return 0;
}

s32 func_80061B10(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP)
{
    s32 avail = availRCP;
    OSScTask *gfx = sc->gfxListHead;
    OSScTask *audio = sc->audioListHead;

    if (sc->doAudio && (avail & OS_SC_SP)) {
        if (gfx && (gfx->flags & OS_SC_PARALLEL_TASK)) {
            *sp = gfx;
            avail &= ~OS_SC_SP;
        } else {
            *sp = audio;
            avail &= ~OS_SC_SP;
            sc->doAudio = 0;
            sc->audioListHead = sc->audioListHead->next;
            if (sc->audioListHead == NULL)
                sc->audioListTail = NULL;
        }
    } else {
        if (__scTaskReady(gfx)) {
            switch (gfx->flags & OS_SC_TYPE_MASK) {
            case OS_SC_XBUS:
                if (gfx->state & OS_SC_YIELDED) {
                    if (avail & OS_SC_SP) {
                        *sp = gfx;
                        avail &= ~OS_SC_SP;
                        if (gfx->state & OS_SC_DP) {
                            *dp = gfx;
                            avail &= ~OS_SC_DP;
                        }
                        sc->gfxListHead = sc->gfxListHead->next;
                        if (sc->gfxListHead == NULL)
                            sc->gfxListTail = NULL;
                    }
                } else {
                    if (avail == (OS_SC_SP | OS_SC_DP)) {
                        *sp = *dp = gfx;
                        avail &= ~(OS_SC_SP | OS_SC_DP);
                        sc->gfxListHead = sc->gfxListHead->next;
                        if (sc->gfxListHead == NULL)
                            sc->gfxListTail = NULL;
                    }
                }
                break;
            case OS_SC_DRAM:
            case OS_SC_SP_DRAM:
            case OS_SC_SP_XBUS:
                if (gfx->state & OS_SC_SP) {
                    if (avail & OS_SC_SP) {
                        *sp = gfx;
                        avail &= ~OS_SC_SP;
                    }
                } else if (gfx->state & OS_SC_DP) {
                    if (avail & OS_SC_DP) {
                        *dp = gfx;
                        avail &= ~OS_SC_DP;
                        sc->gfxListHead = sc->gfxListHead->next;
                        if (sc->gfxListHead == NULL)
                            sc->gfxListTail = NULL;
                    }
                }
                break;
            case OS_SC_DP_DRAM:
            case OS_SC_DP_XBUS:
            default:
                break;
            }
        }
    }

    if (avail != availRCP)
        avail = func_80061B10(sc, sp, dp, avail);

    return avail;
}
