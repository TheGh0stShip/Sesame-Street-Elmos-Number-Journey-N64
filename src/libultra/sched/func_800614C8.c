#include <ultralog.h>
#include <assert.h>
#include <sched.h>
 /* osSendMesg */
void __scHandleRetrace(OSSched *); /* __scHandleRetrace */
void __scHandleRSP(OSSched *); /* __scHandleRSP */
void __scHandleRDP(OSSched *); /* __scHandleRDP */
void __scMain(void *arg)
{
    OSMesg msg;
    OSSched *sc = (OSSched *)arg;
    OSScClient *client;
    static int count = 0;
    while (1) {
        osRecvMesg(&sc->interruptQ, (OSMesg *)&msg, 1);
        switch ((int)msg) {
        case 666: __scHandleRetrace(sc); break;
        case 667: __scHandleRSP(sc); break;
        case 668: __scHandleRDP(sc); break;
        case 669:
            for (client = sc->clientList; client != 0; client = client->next) {
                osSendMesg(client->msgQ, (OSMesg)&sc->prenmiMsg, 0);
            }
            break;
        }
    }
}
