/* __rmonHitCpuFault: stop user threads and report faulted threads */
void func_80066D20(void);   /* __rmonMaskIdleThreadInts */
void func_80066DEC(int);    /* __rmonStopUserThreads */
void func_80065708(void);   /* rmonFindFaultedThreads */
void func_800656DC(void) {
    func_80066D20();
    func_80066DEC(0);
    func_80065708();
}
