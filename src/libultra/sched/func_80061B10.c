#include <ultralog.h>
#include <assert.h>
#include <sched.h>
/* osViGetCurrentFramebuffer */
 /* osViGetNextFramebuffer */

#define OS_SC_DP 0x0001
#define OS_SC_SP 0x0002
#define OS_SC_YIELD 0x0010
#define OS_SC_YIELDED 0x0020
#define OS_SC_DP_XBUS (OS_SC_NEEDS_RDP)
#define OS_SC_DP_DRAM (OS_SC_NEEDS_RDP | OS_SC_DRAM_DLIST)
#define OS_SC_SP_XBUS (OS_SC_NEEDS_RSP)
#define OS_SC_SP_DRAM (OS_SC_NEEDS_RSP | OS_SC_DRAM_DLIST)
#define OS_SC_XBUS (OS_SC_SP_XBUS | OS_SC_DP_XBUS)
#define OS_SC_DRAM (OS_SC_SP_DRAM | OS_SC_DP_DRAM)

static OSScTask *__scTaskReady(OSScTask *t)
{
    if (t) {
        if (osViGetCurrentFramebuffer() != osViGetNextFramebuffer()) {
            return 0;
        }
        return t;
    }
    return 0;
}

s32 __scSchedule(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP)
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
        avail = __scSchedule(sc, sp, dp, avail);

    return avail;
}
