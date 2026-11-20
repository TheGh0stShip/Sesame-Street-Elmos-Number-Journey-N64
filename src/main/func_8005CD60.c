/* moves every node of the list at +0x14 back onto the free list at +0x4 (alUnlink / alLink) */
typedef struct ALLink_s { struct ALLink_s *next; struct ALLink_s *prev; } ALLink;
typedef struct { s32 unk_00; ALLink freeList; s32 unk_0C; s32 unk_10; ALLink *head; } S;
void func_8005C79C(ALLink *ln); /* alUnlink */
void func_8005C77C(ALLink *ln, ALLink *to); /* alLink */
void func_8005CD60(S *s) {
    ALLink *n;
    while ((n = s->head) != NULL) {
        func_8005C79C(n);
        func_8005C77C(n, &s->freeList);
    }
}
