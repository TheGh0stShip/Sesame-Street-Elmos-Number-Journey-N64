typedef struct { u8 pad[8]; u8 mq[1]; } Sys;
extern Sys *D_800A05E4;
s32 func_80058A70(void *mq, void *msg, s32 flag);
/* receive messages until one whose first halfword is 1 */
void func_800524D0(void) {
    s16 *msg;
    do {
        func_80058A70(D_800A05E4->mq, &msg, 1);
        func_80058A70(D_800A05E4->mq, NULL, 0);
    } while (*msg != 1);
}
