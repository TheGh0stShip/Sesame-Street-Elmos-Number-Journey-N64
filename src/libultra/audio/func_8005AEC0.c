/* SDK audio/env.c */
/*====================================================================
 * envmixer.c
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
#include <math.h>
#include <assert.h>
// TODO: these come from headers
#ident "$Revision: 1.49 $"
#ident "$Revision: 1.17 $"
#ifdef AUD_PROFILE
extern u32 cnt_index, env_num, env_cnt, env_max, env_min, lastCnt[];
extern u32 rate_num, rate_cnt, rate_max, rate_min;
extern u32 vol_num, vol_cnt, vol_max, vol_min;
#endif
#define EQPOWER_LENGTH 128

extern s16 eqpower[ EQPOWER_LENGTH ];

extern	f64	__pow(f64, f64);

/*
 * prototypes for private enveloper functions
 */
static  Acmd *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount,
                            s32 sampleOffset, Acmd *p) ;
s16 _getRate(f64 vol, f64 tgt, s32 count, u16* ratel);

f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel);
/***********************************************************************
 * Enveloper filter public interfaces
 ***********************************************************************/
Acmd *alEnvmixerPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset,
                     Acmd *p);

s32 alEnvmixerParam(void *filter, s32 paramID, void *param)
{
    ALFilter      *f = (ALFilter *) filter;
    ALEnvMixer	*e = (ALEnvMixer *) filter;
    
    switch (paramID) {

      case (AL_FILTER_ADD_UPDATE):
          if (e->ctrlTail) {
              e->ctrlTail->next = (ALParam *)param;
          } else {
              e->ctrlList = (ALParam *)param;
          }            
          e->ctrlTail = (ALParam *)param;
          
          break;
        
      case (AL_FILTER_RESET):
          e->first = 1;
          e->motion = AL_STOPPED;
          e->volume = 1;
          if (f->source)
              (*f->source->setParam)(f->source, AL_FILTER_RESET, param);
          break;

      case (AL_FILTER_START):
          e->motion = AL_PLAYING;
          if (f->source)
              (*f->source->setParam)(f->source, AL_FILTER_START, param);
          break;
            
      case (AL_FILTER_SET_SOURCE):
          f->source = (ALFilter *) param;
          break;

      default:
          if (f->source)
              (*f->source->setParam)(f->source, paramID, param);
    }
    return 0;
}
#line 350

Acmd* _pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount,
                    s32 sampleOffset, Acmd *p);

#define EXP_MASK  0x7f800000
#define MANT_MASK 0x807fffff

f64
_frexpf(f64 value, s32 *eptr);
f64
_ldexpf(f64 in, s32 ex);
/*
    _getRate()  --  This function determines how to go from the
		    current volume level (vol) to the target
		    volume level (tgt) in some number of steps
		    (count).  Two values are returned that are
		    used as multipliers to incrementally scale
		    the volume.  Some tricky math is used and
		    is explained below.
		    RWW 28jun95
*/

static
s16 _getRate(f64 vol, f64 tgt, s32 count, u16* ratel);
f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel);







