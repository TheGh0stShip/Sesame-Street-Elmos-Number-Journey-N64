/* __osPiRelAccess: release the PI access queue */
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_8010E670; /* __osPiAccessQueue */
s32 func_80058BA0(OSMesgQueue *mq, OSMesg msg, s32 flag); /* osSendMesg */
void func_80060750(void)
{
    func_80058BA0(&D_8010E670, NULL, 0);
}
