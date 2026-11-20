#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "structs.h"


void func_8000E1B0(s32, s32);
void func_800013B0(void);
void func_80041F84(s32 a);
void func_800016C0(void);
void func_8001AC1C(void);
void func_800125D4(s32 a);
void func_80003E18(s32 a);
void func_80003B10(void);
void func_80001F60(void);
void osWritebackDCacheAll(void);
void func_8000C188(void *);
void func_8000C60C(void);
void func_80001000(void);
void func_80013410(void *);

s32 func_80069D40(void);
void func_80069D74(s32);












void func_80003D68(void);
void func_80012E08(void);
void func_80018F60(void);
void func_8001AA30(void);
void func_8001E2CC(s32);
void func_800495C0(s32);




s32 func_80049970(s32, s32, f32, f32);
s32 func_8004B208(s32, s32, s32, s32, s32, f32);




void func_80010D54(s32, s32, s32);






void func_80018CB0(void);

void func_80018E08(void);


s32 player_text_329C();




extern void D_80052748(void *);
u32 func_8005F230(void);
s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel);


s32 func_80054BA4(s32, s32);


void func_80062BA8(void);
u32 func_80062BD0(void);
void __osSetSR(u32);
void __osSetFpcCsr(u32);
s32 __osSiRawReadIo(u32, u32 *);
s32 __osSiRawWriteIo(u32, u32);
void osMapTLBRdb(void);
void __osDispatchThread(void);
u32 __osProbeTLB(void *);
f32 func_8005B5EC(f32 ivol, s32 samples, s16 ratem, u16 ratel);

s32 alEnvmixerParam(void *f, s32 id, void *p);
void func_8005D060(void *, s32);
s32 func_80063730(void);
void __osSiGetAccess(void);
void func_80064520(void);
void __osPackReadData(void);
s32 __osSiRawStartDma(s32, void *);
void __osSiCreateAccessQueue(void);
void __osContGetInitData(u8 *, OSContStatus *);
void __osPackRequestData(u8);
void __osPiCreateAccessQueue(void);
s32 func_80064550(s32, u32, void *, u32);
void __osDevMgrMain(void *);
void __osPiGetAccess(void);
void func_80060750(void);
extern void __osSetGlobalIntMask(u32);
extern void osYieldThread(void);
extern void __rmonIOputw(u32 w);
extern void __rmonInit(void);
extern void __rmonHitBreak(void);
extern void __rmonHitSpBreak(void);
extern void __rmonHitCpuFault(void);
void osCreateViManager(OSPri pri);
void __scMain(void *arg);

s32 osDpSetNextBuffer(void *, u64);
void func_80063A80(void);
extern u32 __osGetCause(void);
void send(u8 *, u32);

s32 func_80068420(void);
#ifndef _ULTRA64_H_
void bcopy(void *src, void *dst, s32 len);
#endif
s32 __osSpSetPc(u32 pc);
s32 __osSpRawStartDma(s32 dir, u32 devAddr, void *dramAddr, u32 size);
extern u32 func_80068460(void);
void __osViSwapContext(void);
void __osTimerServicesInit(void);
void __osViInit(void);
void viMgrMain(void *arg);
void __osTimerInterrupt(void);
void __osLeoAbnormalResume(void);
void __osLeoResume(void);
void rmonFindFaultedThreads(void);

void __rmonMaskIdleThreadInts(void);
s32 __osSpRawWriteIo(u32 addr, u32 val);

void __rmonIdleRCP(void);

void __rmonRunRCP(void);
f64 __floatdidf(s64);
u64 __fixdfdi(f64);
f64 func_80069808(f64 x);
void func_8006C7F4(void *ctx, s16 a, s16 b, void *m);

/* unified declarations (work/ph_unify.py): one per name; files no longer declare these */
f32 __cosf(f32);
u32 __osDisableInt(void);
u32 __osGetCount(void);
void __osResetGlobalIntMask(s32);
void __osRestoreInt(u32);
void __osSetCompare(u32);
void __osSetTimerIntr(OSTime);
s32 __osSpRawReadIo(u32, u32 *);
void __rmonCopyWords(u32 *, u32 *, s32);
u32 __rmonGetBranchTarget(int, int, char *);
u32 __rmonGetRegisterContents(s32, s32, s32);
void __rmonIOflush(void);
void __rmonIOhandler(void);
s32 __rmonRCPrunning(void);
void __rmonSendData(char *const, unsigned int);
void __rmonSendFault(u8 *);
void __rmonStepRCP(void);
f32 __sinf(f32);
u64 __udivdi3(u64, u64);
s32 _getVol(s32, s32, s16, s32);
void addActorDL(s32, s32, s32, s32, f32);
void alCopy(void *, void *, s32);
s32 alFxParamHdl(void *, s32, void *);
s32 alLoadParam(void *, s32, void *);
s32 alResampleParam(void *, s32, void *);
#ifndef _ULTRA64_H_
void alUnlink(struct Struct_func_8005648C_arg0 *);
#endif
#ifndef _ULTRA64_H_
void bzero(void *, s32);
#endif
double cos(double);
double fabs(double);
void func_80000450(s32, s32, register s32, s32, s32);
s32 func_8000092C(s32, s32, s32, s32, f32, f32, f32, s32, s32);
void func_80000EDC(u32 *, u32 *, u32);
void func_80000F60(void);
void func_80001818(void *);
void func_80001AB4(s32);
void func_80001D38(s32);
void func_80001EE8(void);
void func_80002EA8(void);
void func_8000ADBC(s32);
void func_8000C588(s32, s32, s32);
void func_8000D3C0(void);
void func_8000DC80(s32, s32, s32, s32, s32);
s32 func_8000DFA0(s32, u8);
s32 func_8000E030(void);
void func_8000E16C(s32);
void func_8000E1EC(s32, s32);
void func_8000E264(u8, u8, u8, u8);
void func_8000E2D0(s32);
void func_8000E32C(s32);
s32 func_8000E480(s32, s32);
void func_8000EC10(u8);
void func_8000EDB0(s32);
void func_80011554(Effect *);
void func_800121C0(s32, s32, s32);
s32 func_80012E78(void);
void func_80012F14(s32);
s32 func_80012FD0(s32);
void func_800131A0(void);
s32 func_80013BCC(s32, s32, s32);
s32 func_80013CBC(s32, s32, s32);
s32 func_80013F48(struct IB *, struct ZS *, s32);
s32 func_80015308(s32, s32, s32, s32);
s32 func_80015380(s32, s32, s32);
s32 func_8001649C(uInt *, uInt, uInt, uInt *, uInt *, inflate_huft * *, uInt *, inflate_huft *, uInt *, uInt *);
s32 func_80016D34(s32, s32, s32, s32);
s32 func_80016DBC(s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_80016EE4(s32, s32, s32, s32);
void func_80017520(void);
void func_80017788(s32, s32, s32);
void func_800178B8(s32);
void func_8001791C(void);
void func_80017990(s32, s32);
s32 func_800179D8(s32);
void func_80017C00(s32, s32);
void func_80017F98(s32);
void func_800186E8(s32);
s32 func_800189A0(s32, s32);
void func_80019258(void);
void func_8001933C(s32, s32, register s32, register s32);
void func_8001D3AC(s32, s32);
void func_8001E1E4(s32);
void func_8001E5F0(s32, s32);
void func_8001E74C(s32, s32, s32, s32, s32);
s32 func_8001E9E0(Seg *);
s32 func_8001EBF4(s32, f32, f32, f32);
s32 func_8001F0CC(s32, s32);
void func_8001FAA8(s32, s32, s32, s32, s32);
void func_8001FBE0(s32);
void func_8001FDA0(f32 (*)[4], AnimTrack *, u32);
void func_80021334(s32, s32, s32, s32, s32);
s32 func_80021640(void);
s32 func_800217C4(s32);
void func_80021A20(s32);
void func_80023308(void);
void func_80023408(s32);
void func_80023594(void *);
f64 func_800236B4(s32);
void func_80025390(s32);
void func_800254C4(void *);
void func_80025700(s32);
void func_800258C0(s32);
void func_80025A54(s32);
void func_80026110(s32);
void func_80026494(s32, s32, s32);
s32 func_80026864(s32);
s32 func_80026C04(s32, s32, s32, s32);
void func_80026FCC(s32, f64);
void func_80027D90(s32);
void func_80027F2C(s32);
void func_80027F68(s32);
void func_80027FA4(s32);
void func_8002C424(s32);
s32 func_8002C68C(s32, f32, f32, f32);
void func_8002C800(s32);
void func_8002E154(s32);
void func_8002E568(s32, s32);
void func_8002F640(register f32);
void func_8003086C(void);
void func_80032894(s32);
void func_80032B28();
void func_80033058(s32);
void func_80033224(s32);
void func_80033368(s32);
void func_80033528(s32);
void func_80033A6C(s32);
void func_80033C68(s32, s32);
void func_80033CF0(s32, s32);
void func_80033D70(s32, s32);
void func_80033EC8(s32, s32, s32, s32);
Row *func_80034164(s32, s32, s32, s32);
void func_800346CC(s32, s32);
void func_80034788(s32);
void func_800347CC(s32);
s32 func_80034804(s32, register s32, s32);
void func_80034EE8(s32);
void func_80035260(s32, s32, s32);
void func_8003669C(s32);
void func_80036A90(s32);
void func_80037450(s32, s32);
void func_800374D0(s32);
void func_80037508(s32);
void func_800375A8(s32, s32);
void func_800377A8(void);
void func_8003BEA4(s32);
void func_8003EE1C(s32);
void func_8003EF9C(s32);
void func_8003FA80(s32);
void func_800408D0(s32);
void func_800409A0(s32, s32);
void func_80041A50(s32);
void func_80041A80(s32, s32, s32, s32);
s32 func_80041CAC(s32, u16, u16);
void func_80041E10(s32, s32);
void func_80041F04(s32);
void func_8004200C(s32);
s32 func_80042080(s32);
void func_80042300(s32);
void func_80042620(void);
void func_80042648(s32, s32);
f32 func_80042780(s32, s32);
void func_80042EB0(s32, s32);
void func_80043170(s32);
void func_800432EC(s32);
void func_80043490(s32);
void func_800437CC(s32);
void func_80044018(s32, s32, s32);
void func_800441EC(s32, s32, s32);
void func_80044348(s32);
void func_800443A0(s32, s32, s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void func_80046250(s32, s32, s32, s32);
void func_80046590(s32, s32);
void func_80046810(s32, s32);
void func_80046ABC(s32, s32, s32);
void func_800496B8(s32, s32, s32, s32);
void func_80049894(s32);
void func_80049920(s32);
void func_80049B34(s32, s32);
void func_80049D60(s32);
void func_8004A318(s32, s32);
void func_8004A478(s32);
void func_8004B3E4(s32);
s32 func_8004B5C8(s32, s32);
void func_8004B62C(s32);
void func_8004B6B8(s32, s32);
f32 func_8004B828(f32, f32);
void func_8004BC54(s32, s32, s32);
void func_8004BCEC(s32, f32);
void func_8004BD1C(s32, f32);
s32 func_8004C0A4(s32);
s32 func_8004CAA0(s32, s32, s32, s32, s32, f32, f32, f32, f32);
s32 func_8004CBF4(s32);
void func_8004CE30(s32, f32, f32, s32);
s32 func_8004CF28(s32, f32, f32, f32);
s32 func_8004D340(s32, s32, s32, s32);
s32 func_8004DA14(s32);
void func_8004DCD0(s32, s32, s32);
void func_8004DE00(s32, s32);
void func_8004E06C(Config *);
s32 func_8004E5A0(s32);
s32 func_8004E620(s32, s32, s32, s32, s32);
void func_8004E738(s32, s32);
s32 func_8004E884(s32, s32);
s32 func_8004E938(s32);
s32 func_8004E990(s32, s16);
s32 func_8004E9F0(s32, s16);
s32 func_8004EA60(s32, f32);
s32 func_8004EB5C(s32, s32);
void func_8004EBE4(s32);
void *func_8004EC20(void *);
void func_8004EDAC(RelHdr *);
s32 func_800504E8(s32);
DmaBuf *func_800522CC(u32, s32);
void func_80052610(Params *, s32, u8);
void func_80059540(const char *, const char *, int);
s32 func_8005BE24(s32 *, s32, s32);
void func_8005D068(struct Struct_func_8005D068_arg0 *, s32, s32);
u32 func_8005F240(void);
int func_800643A0(void);
s32 func_80068350(void);
int func_80068440(void);
void func_80068470(s32);
s32 func_80068590(void);
f64 func_800694D0(f64);
f64 func_80069920(f64, f64);
f64 func_80069A90(f64);
f64 func_80069B18(f64, f64);
void *func_80069C70(void *, s32, u32);
f64 func_8006A1F0(f64);
void guLookAtF(float (*)[4], float, float, float, float, float, float, float, float, float);
void guMtxCatF(float (*)[4], float (*)[4], float (*)[4]);
void guMtxXFMF(float (*)[4], float, float, float, float *, float *, float *);
void guNormalize(float *, float *, float *);
void guPerspectiveF(f32 (*)[4], u16 *, f32, f32, f32, f32, f32);
void *kmc_proutSyncPrintf(void *, const u8 *, int);
void *memcpy(void *, const void *, u32);
#ifndef _ULTRA64_H_
void osContGetReadData(OSContPad *);
#endif
#ifndef _ULTRA64_H_
void osCreateMesgQueue(void *, s32, s32);
#endif
u64 osGetTime(void);
#ifndef _ULTRA64_H_
void osInitRdb(u32, u32);
#endif
void osInitialize(void);
#ifndef _ULTRA64_H_
s32 osInvalDCache();
#endif
void osInvalICache(void *, s32);
s32 osPiGetCmdQueue(void);
s32 osPiRawReadIo(s32, s32 *);
#ifndef _ULTRA64_H_
void osSetEventMesg(s32, void *, void *);
#endif
OSIntMask osSetIntMask(OSIntMask);
#ifndef _ULTRA64_H_
void osSpTaskStartGo(void *);
#endif
void osViBlack(u8);
#ifndef _ULTRA64_H_
s32 osViGetCurrentFramebuffer(void);
#endif
#ifndef _ULTRA64_H_
s32 osViGetNextFramebuffer(void);
#endif
#ifndef _ULTRA64_H_
void osViSetEvent(s32, s32, s32);
#endif
#ifndef _ULTRA64_H_
void osViSetMode(void *);
#endif
void osViSetSpecialFeatures(u32);
#ifndef _ULTRA64_H_
void osViSwapBuffer(s32);
#endif
u32 osVirtualToPhysical(void *);
void osWritebackDCache(void *, s32);
s32 proutSyncPrintf(void *, u8 *, u32);
void ramromMain(void);
u32 rmonGetRcpRegister(int);
#ifndef _ULTRA64_H_
void rmonMain(void);
#endif
double sin(double);
#ifndef _ULTRA64_H_
s32 sprintf(char *, const char *, ...);
#endif
f32 sqrtf(f32);
const u8 *strchr(const u8 *, s32);
u32 strlen(const u8 *);

#include "libmus/player.h" /* sm */
#include "codec/stream.h" /* sm */
#include "libc/libc.h" /* sm */
#endif
