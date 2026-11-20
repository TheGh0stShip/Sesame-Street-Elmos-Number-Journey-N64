/* osContGetReadData: copies button/stick/errno of each controller out of the PIF RAM buffer */
typedef struct { u32 ramarray[15]; u32 pifstatus; } OSPifRam;
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u16 button; s8 stick_x; s8 stick_y; } __OSContReadFormat;
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errno; } OSContPad;
#define CHNL_ERR(format) (((format).rxsize & 0xC0) >> 4)
extern OSPifRam D_800ACFC0; /* __osContPifRam */
extern u8 D_80109D48;       /* __osMaxControllers */
void func_8005F718(OSContPad *data) {
    u8 *ptr = (u8 *)D_800ACFC0.ramarray;
    __OSContReadFormat readformat;
    int i;

    for (i = 0; i < D_80109D48; i++, ptr += sizeof(__OSContReadFormat), data++) {
        readformat = *(__OSContReadFormat *)ptr;
        data->errno = CHNL_ERR(readformat);
        if (data->errno != 0) {
            continue;
        }
        data->button = readformat.button;
        data->stick_x = readformat.stick_x;
        data->stick_y = readformat.stick_y;
    }
}
