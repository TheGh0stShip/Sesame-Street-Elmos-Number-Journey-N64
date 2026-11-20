/* rmon request handler: reply with the object's executable name ("rmon"-style string per method) */
typedef struct {
    s32 length;
    u8 code;
    u8 type;
    s16 error;
    u8 rev;
    u8 method;
    u8 unk_A;
    u8 unk_B;
} KKHeader;
typedef struct {
    KKHeader header;
    s32 object;
} KKObjectRequest;
typedef struct {
    KKHeader header;
    s32 object;
    char buffer[8];
} KKBufferEvent;
extern KKBufferEvent D_80115418; /* __rmonUtilityBuffer */
extern char D_8007BBC0[];
extern char D_8007BBC8[];
void func_80060898(KKHeader *, u32, u32); /* __rmonSendReply */

static inline void str_copy(char *d, const char *s)
{
    while ((*d++ = *s++) != 0)
        ;
}

int func_80065E74(KKHeader *req)
{
    KKObjectRequest *request = (KKObjectRequest *)req;
    KKBufferEvent *reply = &D_80115418;

    reply->header.code = request->header.code;
    reply->header.error = 0;
    reply->object = request->object;
    if (req->method == 1)
        str_copy(reply->buffer, D_8007BBC0);
    else
        str_copy(reply->buffer, D_8007BBC8);
    func_80060898(&reply->header, sizeof(KKBufferEvent), 1);
    return 0;
}
