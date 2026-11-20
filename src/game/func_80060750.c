#include "common.h"

/* __osPiRelAccess: release the PI access queue */

typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_8010E670; /* __osPiAccessQueue */
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag); /* osSendMesg */
void func_80060750(void)
{
    osSendMesg(&D_8010E670, NULL, 0);
}
