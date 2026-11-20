/* event handler: on message type 2 append data to the object's list */
typedef struct { u8 pad[0x14]; s32 unk_14; u8 pad2[4]; void **unk_1C; } Obj;
s32 func_8005A98C(Obj *o, s32 type, void *data)
{
    void **list = o->unk_1C;
    if (type == 2) {
        list[o->unk_14++] = data;
    }
    return 0;
}
