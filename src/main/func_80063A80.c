/* osSpTaskYield: set SP_SET_YIELD (0x400) */
extern void func_80068470(u32); /* __osSpSetStatus */
void func_80063A80(void) { func_80068470(0x400); }
