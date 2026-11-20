/* writes a value to PI/AI register 0xA4040010 (SP status) */
void func_80068470(s32 arg0) {
    *(volatile s32 *)0xA4040010 = arg0;
}
