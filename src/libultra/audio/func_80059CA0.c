/* SDK audio/drvrnew.c */
/*====================================================================
 * drvrNew.c
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
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
#include <libaudio.h>
#include "synthInternals.h"
#include <os.h>
#include <stdio.h>
#include "initfx.h"
// TODO: this comes from a header
#ident "$Revision: 1.49 $"
/*
 * WARNING: THE FOLLOWING CONSTANT MUST BE KEPT IN SYNC
 * WITH SCALING IN MICROCODE!!!
 */
#define	SCALE 16384

/*
 * the following arrays contain default parameters for
 * a few hopefully useful effects.
 */
#define ms *(((s32)((f32)44.1))&~0x7)

extern s32 SMALLROOM_PARAMS[26];
extern s32 BIGROOM_PARAMS[34];
extern s32 ECHO_PARAMS[10];
extern s32 CHORUS_PARAMS[10];
extern s32 FLANGE_PARAMS[10];
extern s32 NULL_PARAMS[10];
void _init_lpfilter(ALLowPass *lp);
void alFxNew(ALFx *r, ALSynConfig *c, ALHeap *hp);
void alEnvmixerNew(ALEnvMixer *e, ALHeap *hp);
void alLoadNew(ALLoadFilter *f, ALDMANew dmaNew, ALHeap *hp);
void alResampleNew(ALResampler *r, ALHeap *hp);

void alAuxBusNew(ALAuxBus *m, void *sources, s32 maxSources)
{
    alFilterNew((ALFilter *) m, alAuxBusPull, alAuxBusParam, AL_AUXBUS);
    m->sourceCount = 0;
    m->maxSources = maxSources;
    m->sources = (ALFilter **)sources;
}
void alMainBusNew(ALMainBus *m, void *sources, s32 maxSources);
void alSaveNew(ALSave *f);

