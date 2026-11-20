/* SDK audio/synthesizer.c */
/*====================================================================
 * synthesizer.c
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/
#include "synthInternals.h"
#include <assert.h>
#include <PR/os_version.h>
// TODO: this comes from a header
#ident "$Revision: 1.17 $"

#ifdef AUD_PROFILE
#include <os.h>
extern u32 cnt_index, drvr_num, drvr_cnt, drvr_max, drvr_min, lastCnt[];
extern u32 client_num, client_cnt, client_max, client_min;
#endif

#ifndef MIN
#   define MIN(a,b) (((a)<(b))?(a):(b))
#endif

s32 __nextSampleTime(ALSynth *drvr, ALPlayer **client);
s32 _timeToSamplesNoRound(ALSynth *ALSynth, s32 micros);
/***********************************************************************
 * Synthesis driver public interfaces
 ***********************************************************************/
void alSynNew(ALSynth *drvr, ALSynConfig *c);
/*
 * slAudioFrame() is called every video frame, and is based on the video
 * frame interrupt. It is assumed to be an accurate time source for the
 * clients.
 */
Acmd *alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen);
/***********************************************************************
 * Synthesis driver private interfaces
 ***********************************************************************/

ALParam *__allocParam();
void __freeParam(ALParam *param);
void _collectPVoices(ALSynth *drvr);
void _freePVoice(ALSynth *drvr, PVoice *pvoice);
/*
  Add 0.5 to adjust the average affect of
  the truncation error produced by casting
  a float to an int.
*/
s32 _timeToSamplesNoRound(ALSynth *synth, s32 micros);
s32 _timeToSamples(ALSynth *synth, s32 micros);

s32 __nextSampleTime(ALSynth *drvr, ALPlayer **client) 
{
    ALMicroTime delta = 0x7fffffff;     /* max delta for s32 */
    ALPlayer *cl;
#line 306
    assert(drvr->head);

    *client = 0;
    
    for (cl = drvr->head; cl != 0; cl = cl->next) {
        if ((cl->samplesLeft - drvr->curSamples) < delta) {
            *client = cl;
            delta = cl->samplesLeft - drvr->curSamples;
        }
    }

    return (*client)->samplesLeft;
}



