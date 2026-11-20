/* __osTimerInterrupt (libultra os/timerintr.c): expire due timers, with the debug profiler sampling */
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;

typedef struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    u64 interval;
    u64 value;
    OSMesgQueue *mq;
    OSMesg msg;
} OSTimer;

typedef struct {
    u8 unk_00[0x11C];
    u32 pc;              /* context.pc */
} OSThread;

typedef struct {
    u16 *histo_base;
    u32 histo_size;
    u32 *text_start;
    u32 *text_end;
} OSProf;

extern OSTimer *D_80077C40;      /* __osTimerList */
extern u32 D_8012C404;           /* __osTimerCounter */
extern OSMesgQueue D_8012FE50;   /* __osProfTimerQ */
extern OSProf *D_800AD03C;       /* __osProfileList */
extern OSProf *D_8012FE40;       /* __osProfileListEnd */
extern u32 D_800A914C;           /* __osProfileOverflowBin */
extern OSThread *D_80077C28;     /* __osRunQueue */

void func_80062D60(u32);                        /* __osSetCompare */
u32 func_80062BC0(void);                        /* osGetCount */
void func_800592F8(u64);                        /* __osSetTimerIntr */
s32 func_80058BA0(OSMesgQueue *, OSMesg, s32);  /* osSendMesg */
u64 func_80059358(OSTimer *);                   /* __osInsertTimer */

void func_80059114(void) {
    OSTimer *t;
    u32 count;
    u32 elapsed_cycles;
    u32 pc;
    s32 offset;
    OSProf *prof = D_800AD03C;

    if (D_80077C40->next == D_80077C40) {
        return;
    }
    for (;;) {
        t = D_80077C40->next;

        if (t == D_80077C40) {
            func_80062D60(0);
            D_8012C404 = 0;
            break;
        }

        count = func_80062BC0();
        elapsed_cycles = count - D_8012C404;
        D_8012C404 = count;

        if (elapsed_cycles < t->value) {
            t->value -= elapsed_cycles;
            func_800592F8(t->value);
            break;
        }

        t->prev->next = t->next;
        t->next->prev = t->prev;
        t->next = NULL;
        t->prev = NULL;

        if (t->mq != NULL) {
            if (t->mq != &D_8012FE50) {
                func_80058BA0(t->mq, t->msg, 0);
            } else {
                pc = D_80077C28->pc;
                for (prof = D_800AD03C; prof < D_8012FE40; prof++) {
                    offset = pc - (u32)prof->text_start;

                    if (offset >= 0) {
                        if ((s32)prof->text_end - (s32)pc > 0) {
                            (*(u16 *)(u32)((offset >> 2) + prof->histo_base))++;
                            goto __ProfDone;
                        }
                    }
                }

                D_800A914C++;
            }
        }

    __ProfDone:

        if (t->interval != 0) {
            t->value = t->interval;
            func_80059358(t);
        }
    }
}
