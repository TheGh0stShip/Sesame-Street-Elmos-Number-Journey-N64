#include "common.h"

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
} KKBufferEvent;/* __rmonUtilityBuffer */

void __rmonSendReply(KKHeader *, u32, u32); /* __rmonSendReply */

static inline void str_copy(char *d, const char *s)
{
    while ((*d++ = *s++) != 0)
        ;
}

int __rmonGetExeName(KKHeader *req)
{
    KKObjectRequest *request = (KKObjectRequest *)req;
    KKBufferEvent *reply = &__rmonUtilityBuffer;

    reply->header.code = request->header.code;
    reply->header.error = 0;
    reply->object = request->object;
    if (req->method == 1)
        str_copy(reply->buffer, D_8007BBC0);
    else
        str_copy(reply->buffer, D_8007BBC8);
    __rmonSendReply(&reply->header, sizeof(KKBufferEvent), 1);
    return 0;
}
