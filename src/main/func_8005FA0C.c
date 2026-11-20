/* __osContGetInitData */
typedef struct {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
    u8 dummy1;
} __OSContRequesFormat;
typedef struct {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;
extern OSPifRam D_800ACFC0; /* __osContPifRam */
extern u8 D_80109D48; /* __osMaxControllers */
#define CHNL_ERR(format) (((format).rxsize & 0xC0) >> 4)

void func_8005FA0C(u8 *pattern, OSContStatus *data)
{
    u8 *ptr;
    __OSContRequesFormat requestformat;
    int i;
    u8 bits;

    bits = 0;
    ptr = (u8 *)D_800ACFC0.ramarray;
    for (i = 0; i < D_80109D48; i++, ptr += sizeof(requestformat), data++) {
        requestformat = *(__OSContRequesFormat *)ptr;
        data->errno = CHNL_ERR(requestformat);
        if (data->errno == 0) {
            data->type = (requestformat.typel << 8) | requestformat.typeh;
            data->status = requestformat.status;
            bits |= 1 << i;
        }
    }
    *pattern = bits;
}
