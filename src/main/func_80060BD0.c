extern u32 func_80063440(u8 *, u32, s32);
void func_80060BD0(u8 *buf)
{
    volatile s32 pad;
    u32 done;
    u8 *p;
    done = 0;
    pad = 0;
    p = buf;
    do {
        done += func_80063440(p + done, 0x1B0 - done, 2);
    } while (done < 0x1B0);
}
