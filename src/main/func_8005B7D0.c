/* appends a value to a pointer list when the command is 2; returns 0 */
typedef struct { u8 pad[0x14]; s32 count; u8 pad2[4]; void **list; } S;
s32 func_8005B7D0(S *s, s32 cmd, void *v) {
    void **l = s->list;
    if (cmd == 2) {
        l[s->count++] = v;
    }
    return 0;
}
