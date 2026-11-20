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
/* __rmonGetRegions: report memory regions to the debugger */
int func_80065F84(KKHeader* req) {
    KKObjectRequest* request = (KKObjectRequest*)req;
    KKRegionEvent* reply = (KKRegionEvent*)D_80115418;
    int numRegions;


    numRegions = (req->method == RMON_RSP) ? 2 : 6;

    reply->header.length = numRegions * sizeof(reply->regions[0]) + sizeof(*reply);
    reply->header.code = request->header.code;
    reply->header.error = TV_ERROR_NO_ERROR;
    reply->object = request->object;
    reply->number = numRegions;

    reply->regions[1].vaddr = SP_IMEM_START;
    reply->regions[1].size = SP_IMEM_END + 1 - SP_IMEM_START;
    reply->regions[1].flags = 1 | 2 | 4;
    reply->regions[1].paddr = SP_IMEM_START;

    reply->regions[0].vaddr = SP_DMEM_START;
    reply->regions[0].size = SP_DMEM_END + 1 - SP_DMEM_START;
    reply->regions[0].flags = 1 | 2;
    reply->regions[0].paddr = SP_DMEM_START;

    if (numRegions > 2) {
        reply->regions[2].vaddr = 0x88200000;
        reply->regions[2].size = 0x6130;
        reply->regions[2].flags = 1 | 4;
        reply->regions[2].paddr = 0;

        reply->regions[3].vaddr = 4;
        reply->regions[3].size = 0x200000;
        reply->regions[3].flags = 1 | 2;
        reply->regions[3].paddr = 0;

        reply->regions[4].vaddr = 0x4002000;
        reply->regions[4].size = 0x800000;
        reply->regions[4].flags = 1 | 2;
        reply->regions[4].paddr = 0;

        reply->regions[5].vaddr = 0x88206130;
        reply->regions[5].size = 0x9000;
        reply->regions[5].flags = 1 | 2;
        reply->regions[5].paddr = 0;
    }

    func_80060898(&reply->header, reply->header.length, KK_TYPE_REPLY);

    return TV_ERROR_NO_ERROR;
}
