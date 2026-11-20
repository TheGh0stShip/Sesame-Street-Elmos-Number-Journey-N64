u32 func_80062CA0(void *); /* __osProbeTLB */
/* osVirtualToPhysical */
u32 func_80059460(void *addr) {
    if ((u32)addr - 0x80000000 < 0x20000000) {
        return (u32)addr & 0x1FFFFFFF;
    } else if ((u32)addr - 0xA0000000 < 0x20000000) {
        return (u32)addr & 0x1FFFFFFF;
    } else {
        return func_80062CA0(addr);
    }
}
