/* __osRdbSend (os/rdbsend.c): send bytes to the debugger over the RDB port */
typedef struct {
    unsigned type : 6;
    unsigned length : 2;
    u8 buf[3];
} rdbPacket;

extern rdbPacket *D_8013C728;   /* __osRdb_IP6_Data */
extern u32 D_80107D18;          /* __osRdb_IP6_Size */
extern u32 D_8013ECF4;          /* __osRdb_IP6_Ct */
extern u32 D_800AD008;          /* __osRdb_IP6_CurWrite */
extern u32 D_80077FE0;          /* __osRdb_IP6_Empty */

u32 func_80062C60(void);        /* __osDisableInt */
void func_80062C80(u32 mask);   /* __osRestoreInt */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

u32 func_80063440(u8 *buf, u32 size, u32 type) {
    rdbPacket *pPtr;
    rdbPacket pkt;
    u32 len;
    u32 c;
    u32 inCt = 0;
    u32 needFirst = 0;
    u32 mask;
    u32 sent = 0;

    mask = func_80062C60();

    if (D_80077FE0 != 0) {
        D_80077FE0 = 0;
        pkt.type = type;

        len = MIN(size, sizeof(pkt.buf));

        pkt.length = len;

        c = 0;
        while (c < len) {
            pkt.buf[c++] = buf[inCt++];
        }

        size -= len;
        sent = len;
        needFirst = 1;
    }

    while (size != 0 && D_8013ECF4 < D_80107D18) {
        len = MIN(size, sizeof(pPtr->buf));

        pPtr = &D_8013C728[D_800AD008];

        pPtr->type = type;
        pPtr->length = len;

        c = 0;
        while (c < len) {
            pPtr->buf[c++] = buf[inCt++];
        }

        D_800AD008++;
        if (D_800AD008 >= D_80107D18) {
            D_800AD008 = 0;
        }

        D_8013ECF4++;
        size -= len;
        sent += len;
    }

    if (needFirst) {
        *(u32 *)0xC0000000 = *(u32 *)&pkt;
    }
    func_80062C80(mask);

    return sent;
}
