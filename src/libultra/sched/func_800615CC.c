/* SDK sched/sched.c */
/*====================================================================
 * 
 * Copyright 1995, Silicon Graphics, Inc.
 * All Rights Reserved.
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/
#include <ultralog.h>
#include <assert.h>
#include <sched.h>
// TODO: this comes from a header
#ident "$Revision: 1.17 $"

/*
 * private typedefs and defines
 */
#define VIDEO_MSG       666
#define RSP_DONE_MSG    667
#define RDP_DONE_MSG    668
#define PRE_NMI_MSG     669

/*
 * OSScTask state
 */
#define OS_SC_DP                0x0001  /* set if still needs dp        */
#define OS_SC_SP                0x0002  /* set if still needs sp        */
#define OS_SC_YIELD             0x0010  /* set if yield requested       */
#define OS_SC_YIELDED           0x0020  /* set if yield completed       */

/*
 * OSScTask->flags type identifier
 */
#define OS_SC_XBUS      (OS_SC_SP | OS_SC_DP)
#define OS_SC_DRAM      (OS_SC_SP | OS_SC_DP | OS_SC_DRAM_DLIST)
#define OS_SC_DP_XBUS   (OS_SC_SP)
#define OS_SC_DP_DRAM   (OS_SC_SP | OS_SC_DRAM_DLIST)
#define OS_SC_SP_XBUS   (OS_SC_DP)
#define OS_SC_SP_DRAM   (OS_SC_DP | OS_SC_DRAM_DLIST)

/*
 * private functions
 */
static void     __scMain(void *arg);
void     __scHandleRetrace(OSSched *s);
void     __scHandleRSP(OSSched *s);
void     __scHandleRDP(OSSched *s);

void     __scAppendList(OSSched *s, OSScTask *t);
OSScTask        *__scTaskReady(OSScTask *t);
s32      __scTaskComplete(OSSched *s,OSScTask *t);
void     __scExec(OSSched *sc, OSScTask *sp, OSScTask *dp);
void	__scYield(OSSched *s);
s32      __scSchedule(OSSched *sc, OSScTask **sp, OSScTask **dp,
                             s32 availRCP);


/*
#define SC_LOGGING 1
*/


#ifdef SC_LOGGING
#define SC_LOG_LEN      32*1024

extern OSLog    scLog;
extern OSLog    *l;
u32      logArray[SC_LOG_LEN/sizeof(u32)];
#endif

/***********************************************************************
 * Scheduler API
 **********************************************************************/
void osCreateScheduler(OSSched *sc, void *stack, OSPri priority,
                       u8 mode, u8 numFields);
/*
 * Add a client to the scheduler.  Clients receive messages at retrace time
 */
void osScAddClient(OSSched *sc, OSScClient *c, OSMesgQueue *msgQ);
void osScRemoveClient(OSSched *sc, OSScClient *c);
OSMesgQueue *osScGetCmdQ(OSSched *sc);
/***********************************************************************
 * Scheduler implementation
 **********************************************************************/
static void __scMain(void *arg);

/*
 * scHandleRetrace()
 */
static int dp_busy = 0;
extern int dpCount;

void __scHandleRetrace(OSSched *sc)
{
    OSScTask    *rspTask;
    OSScClient  *client;
    s32         i;
    s32         state;
    OSScTask    *sp = 0;
    OSScTask    *dp = 0;
    
    sc->frameCount++;
    
#ifdef SC_LOGGING
    osLogEvent(l, 500, 4, sc->frameCount, sc->curRSPTask, sc->curRDPTask);
#endif

    /*
     * Read the task command queue and schedule tasks
     */
    while (osRecvMesg(&sc->cmdQ, (OSMesg *)&rspTask, OS_MESG_NOBLOCK) != -1) {
        __scAppendList(sc, rspTask);
    }
    
    if (sc->doAudio && sc->curRSPTask) {
        /*
         * Preempt the running gfx task.  Note: if the RSP
         * component of the graphics task has finished, but the
         * RDP component is still running, we can start an audio
         * task which will freeze the RDP (and save the RDP cmd
         * FIFO) while the audio RSP code is running.
         */
        __scYield(sc);
    } else {
        state = ((sc->curRSPTask == 0) << 1) | (sc->curRDPTask == 0);
        if ( __scSchedule (sc, &sp, &dp, state) != state)
            __scExec(sc, sp, dp);
    }
    
    /*
     * notify audio and graphics threads to start building the command
     * lists for the next frame (client threads may choose not to
     * build the list in overrun case)
     */
    for (client = sc->clientList; client != 0; client = client->next) {
        osSendMesg(client->msgQ, (OSMesg) &sc->retraceMsg, OS_MESG_NOBLOCK);
    }
}
/*
 * __scHandleRSP is called when an RSP task signals that it has
 * finished or yielded (at the hosts request)
 */
void __scHandleRSP(OSSched *sc);
/*
 * __scHandleRDP is called when an RDP task signals that it has
 * finished
 */
void __scHandleRDP(OSSched *sc);
/*
 * __scTaskReady checks to see if the graphics task is able to run
 * based on the current state of the RCP.
 */
OSScTask *__scTaskReady(OSScTask *t);
/*
 * __scTaskComplete checks to see if the task is complete (all RCP
 * operations have been performed) and sends the done message to the
 * client if it is.
 */
s32 __scTaskComplete(OSSched *sc, OSScTask *t);
/*
 * Place task on either the audio or graphics queue
 */
void __scAppendList(OSSched *sc, OSScTask *t);
/*
 * 
 */
void __scExec(OSSched *sc, OSScTask *sp, OSScTask *dp);
void __scYield(OSSched *sc);
/*
 * Schedules the tasks to be run on the RCP
 */
s32 __scSchedule(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP);

