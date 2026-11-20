typedef int TVid;
typedef struct __KKHeaderStruct {
    int length;
    u8 code;
    u8 type;
    short error;
    u8 rev;
    u8 method;
    short notused2;
} KKHeader;
typedef struct { KKHeader header; TVid object; } KKObjectRequest;
typedef struct { KKHeader header; TVid object; } KKObjectEvent;
typedef struct { KKHeader header; TVid object; unsigned int addr; unsigned int nbytes; } KKWriteHeader;
typedef struct { KKWriteHeader writeHeader; u8 buffer[1]; } KKWriteRequest;
typedef struct { unsigned int vaddr; unsigned int size; short flags; unsigned int paddr; } KKRegion;
typedef struct { KKHeader header; TVid object; unsigned int number; KKRegion regions[1]; } KKRegionEvent;
#define TV_ERROR_NO_ERROR 0
#define TV_ERROR_INVALID_ADDRESS -5
#define TV_ERROR_INVALID_CAPABILITY -8
#define KK_TYPE_REPLY 1
#define RMON_CPU 0
#define RMON_RSP 1
#define RMON_MAX_XFER_SIZE 1024
#define SP_DMEM_START 0x04000000
#define SP_DMEM_END 0x04000FFF
#define SP_IMEM_START 0x04001000
#define SP_IMEM_END 0x04001FFF
#define FALSE 0
#define TRUE 1
extern u8 D_80115418[256]; /* __rmonUtilityBuffer */
void func_80060898(KKHeader *const block, u32 blockSize, u32 replyType); /* __rmonSendReply */
s32 func_800684B0(u32 addr, u32 val); /* __osSpRawWriteIo */
s32 func_800637F0(u32 addr, u32 *data); /* __osSpRawReadIo */
u32 func_80059460(void *); /* osVirtualToPhysical */

static void __rmonWriteWordTo(u32 *addr, u32 val) {
    while (func_800684B0((u32)addr, val) != 0) {
        ;
    }
}

static u32 __rmonReadWordAt(u32 *addr) {
    u32 data;

    if ((u32)addr >= SP_DMEM_START && (u32)addr < 0x05000000) {
        func_800637F0((u32)addr, &data);
        return data;
    }
    return 0;
}

static void __rmonMemcpy(u8 *dest, u8 *srce, u32 count) {
    while (count-- > 0) {
        *dest++ = *srce++;
    }
}

/* __rmonWriteMem: debugger memory write request handler */
int func_80065BF0(KKHeader *req) {
    register KKWriteRequest *request = (KKWriteRequest *)req;
    KKObjectEvent reply;

    if (req->method == RMON_CPU && func_80059460((u32 *)request->writeHeader.addr) == (u32)-1) {
        return TV_ERROR_INVALID_ADDRESS;
    }
    if (request->writeHeader.nbytes > RMON_MAX_XFER_SIZE) {
        return TV_ERROR_INVALID_CAPABILITY;
    }

    if (((request->writeHeader.addr < SP_DMEM_START ||
         (request->writeHeader.addr + request->writeHeader.nbytes) > 0x04FFFFFF) ? FALSE : TRUE)) {
        int align;
        u32 word;

        if ((align = request->writeHeader.addr & 3) != 0) {
            if (request->writeHeader.nbytes != 1) {
                return TV_ERROR_INVALID_ADDRESS;
            }
            word = __rmonReadWordAt((u32 *)(request->writeHeader.addr & ~3));
            if (align == 1) {
                word = (word & ~0xFF0000) | (request->buffer[0] << 0x10);
            } else if (align == 2) {
                word = (word & ~0xFF00) | (request->buffer[0] << 8);
            } else {
                word = (word & ~0xFF) | (request->buffer[0] << 0);
            }
            __rmonWriteWordTo((u32 *)(request->writeHeader.addr & ~3), word);
        } else {
            int wordCount = request->writeHeader.nbytes / sizeof(u32);
            u32 *wordPointer = (u32 *)request->buffer;

            if (request->writeHeader.nbytes % sizeof(u32) != 0) {
                return TV_ERROR_INVALID_ADDRESS;
            }
            while (wordCount--) {
                __rmonWriteWordTo((u32 *)request->writeHeader.addr, *(wordPointer++));
                request->writeHeader.addr += sizeof(*wordPointer);
            }
        }
    } else {
        __rmonMemcpy((u8 *)request->writeHeader.addr, (u8 *)request->buffer, request->writeHeader.nbytes);
    }

    reply.header.code = request->writeHeader.header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.object = request->writeHeader.object;
    func_80060898(&reply.header, sizeof(reply), KK_TYPE_REPLY);

    return TV_ERROR_NO_ERROR;
}
