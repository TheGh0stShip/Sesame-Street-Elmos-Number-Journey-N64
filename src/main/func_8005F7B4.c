/* __osPackReadData (contreaddata.c) */
typedef struct {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u16 button;
    s8 stick_x;
    s8 stick_y;
} __OSContReadFormat;
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;
extern OSPifRam D_800ACFC0; /* __osContPifRam */
extern u8 D_80109D48; /* __osMaxControllers */

void func_8005F7B4(void)
{
    u8 *ptr;
    __OSContReadFormat readformat;
    int i;

    ptr = (u8 *)D_800ACFC0.ramarray;
    for (i = 0; i < 15; i++)
        D_800ACFC0.ramarray[i] = 0;
    D_800ACFC0.pifstatus = 1;
    readformat.dummy = 0xFF;
    readformat.txsize = 1;
    readformat.rxsize = 4;
    readformat.cmd = 1;
    readformat.button = 0xFFFF;
    readformat.stick_x = -1;
    readformat.stick_y = -1;
    for (i = 0; i < D_80109D48; i++) {
        *(__OSContReadFormat *)ptr = readformat;
        ptr += sizeof(readformat);
    }
    *ptr = 0xFE;
}
