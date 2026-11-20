/* __osPackRequestData (libultra io/controller.c) */
typedef void *OSMesg;
typedef u64 OSTime;
typedef struct OSThread_s OSThread;
typedef struct { OSThread *mtqueue; OSThread *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct OSTimer_s { struct OSTimer_s *next, *prev; OSTime interval; OSTime value; OSMesgQueue *mq; OSMesg msg; } OSTimer;
typedef struct { u16 type; u8 status; u8 errno; } OSContStatus;
typedef struct { u32 ramarray[15]; u32 pifstatus; } OSPifRam;
typedef struct { u8 dummy, txsize, rxsize, cmd, typeh, typel, status, dummy1; } __OSContRequesFormat;
#define OS_USEC_TO_CYCLES(n) (((u64)(n)*(46875000LL/15625LL))/(1000000LL/15625LL))
#define CHNL_ERR(f) (((f).rxsize & 0xC0) >> 4)
extern OSPifRam D_800ACFC0;      /* __osContPifRam */
extern u8 D_80118241;            /* __osContLastCmd */
extern u8 D_80109D48;            /* __osMaxControllers */
extern OSMesgQueue D_8013E7D8;   /* __osEepromTimerQ */
extern OSMesg D_80128444;        /* __osEepromTimerMsg */
extern s32 D_80077F30;           /* __osContinitialized */
OSTime func_800586B0(void);      /* osGetTime */
void func_800585B0(OSMesgQueue *, OSMesg *, s32); /* osCreateMesgQueue */
s32 func_80058E00(OSTimer *, OSTime, OSTime, OSMesgQueue *, OSMesg); /* osSetTimer */
s32 func_80058A70(OSMesgQueue *, OSMesg *, s32); /* osRecvMesg */
s32 func_800643C0(s32, void *);  /* __osSiRawStartDma */
void func_80064460(void);        /* __osSiCreateAccessQueue */
void func_8005FA0C(u8 *, OSContStatus *); /* __osContGetInitData */
void func_8005FAC0(u8);          /* __osPackRequestData */
void func_8005FAC0(u8 cmd) {
    u8 *ptr;
    __OSContRequesFormat requestHeader;
    s32 i;

    for (i = 0; i < 15; i++) {
        D_800ACFC0.ramarray[i] = 0;
    }
    D_800ACFC0.pifstatus = 1;
    ptr = (u8 *)D_800ACFC0.ramarray;
    requestHeader.dummy = 0xFF;
    requestHeader.txsize = 1;
    requestHeader.rxsize = 3;
    requestHeader.cmd = cmd;
    requestHeader.typeh = 0xFF;
    requestHeader.typel = 0xFF;
    requestHeader.status = 0xFF;
    requestHeader.dummy1 = 0xFF;
    for (i = 0; i < D_80109D48; i++) {
        *(__OSContRequesFormat *)ptr = requestHeader;
        ptr += sizeof(requestHeader);
    }
    *ptr = 0xFE;
}
