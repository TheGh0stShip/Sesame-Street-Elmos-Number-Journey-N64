/* osSpTaskStartGo: wait for SP not busy, then clear halt/sstep and set intr-break */
extern u32 func_80068440(void);   /* __osSpDeviceBusy */
extern void func_80068470(u32);   /* __osSpSetStatus */
void func_80063A4C(void *tp)
{
    while (func_80068440())
        ;
    func_80068470(0x125);
}
