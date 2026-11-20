/* audio player event handler (message switch) */
typedef struct Node_s { struct Node_s *next; } Node;
typedef struct {
    u8 pad0[0x48];
    s32 unk_48;
    s32 unk_4C;
    s32 unk_50;
    u8 pad54[0x5A - 0x54];
    s16 unk_5A;
    u8 pad5C[0x74 - 0x5C];
    s32 unk_74;
    s32 unk_78;
    Node *unk_7C;
    Node *unk_80;
    s32 unk_84;
} Player;
void func_80054B28(Player *p, s32 msg);

s32 func_80054288(Player *p, s32 msg, Node *arg)
{
    switch (msg) {
    case 3:
        if (p->unk_80 != NULL)
            p->unk_80->next = arg;
        else
            p->unk_7C = arg;
        p->unk_80 = arg;
        return 0;
    case 4:
        p->unk_78 = 1;
        p->unk_84 = 0;
        p->unk_5A = 1;
        p->unk_74 = 0;
        p->unk_4C = 0;
        p->unk_50 = 1;
        p->unk_48 = 0;
        func_80054B28(p, 4);
        return 0;
    case 9:
        p->unk_84 = 1;
        return 0;
    default:
        func_80054B28(p, msg);
        break;
    }
    return 0;
}
