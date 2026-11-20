/* SDK audio/reverb.c */
/*====================================================================
 * reverb.c
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
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
#include <libaudio.h>
#include <ultraerror.h>
#include "synthInternals.h"
#include <os.h>
#include <os_internal.h>
#include <stdio.h>
#include <assert.h>
#include "initfx.h"
// TODO: these come from headers
#ident "$Revision: 1.49 $"
#ident "$Revision: 1.17 $"
#define RANGE 2.0
extern ALGlobals *alGlobals;

#ifdef AUD_PROFILE
extern u32 cnt_index, reverb_num, reverb_cnt, reverb_max, reverb_min, lastCnt[];
extern u32 load_num, load_cnt, load_max, load_min, save_num, save_cnt, save_max, save_min;
#endif

/*
 * macros
 */
#define SWAP(in, out)	\
{			\
     s16 t = out;	\
     out = in;		\
     in = t;		\
}


Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p);
Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_filterBuffer(ALLowPass *lp, s32 buff, s32 count, Acmd *p);
f32  _doModFunc(ALDelay *d, s32 count);
extern s32 L_INC[];

/***********************************************************************
 * Reverb filter public interfaces
 ***********************************************************************/
Acmd *alFxPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset,
                     Acmd *p) 
{
    Acmd        *ptr = p;
    ALFx	*r = (ALFx *)filter;
    ALFilter    *source = r->filter.source;
    s16		i, buff1, buff2, input, output;
    s16		*in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
    ALDelay	*d, *pd;

#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif
#line 74
    assert(source);

    /*
     * pull channels going into this effect first
     */
    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);

    input  = AL_AUX_L_OUT;
    output = AL_AUX_R_OUT;
    buff1  = AL_TEMP_0;
    buff2  = AL_TEMP_1;
    
    aSetBuffer(ptr++, 0, 0, 0, outCount<<1);  /* set the buffer size */
    aMix(ptr++, 0, 0xda83, AL_AUX_L_OUT, input); /* .707L = L - .293L */
    aMix(ptr++, 0, 0x5a82, AL_AUX_R_OUT, input); /* mix the AuxL and AuxR into the AuxL */
    /* and write the mixed value to the delay line at r->input */
    ptr = _saveBuffer(r, r->input, input, outCount, ptr);

    aClearBuffer(ptr++, output, outCount<<1); /* clear the AL_AUX_R_OUT */

    for (i = 0; i < r->section_count; i++) {
	d  = &r->delay[i];  /* get the ALDelay structure */
	in_ptr  = &r->input[-d->input];
	out_ptr = &r->input[-d->output];
	
	if (in_ptr == prev_out_ptr) {
	    SWAP(buff1, buff2);
	} else {  /* load data at in_ptr into buff1 */
	    ptr = _loadBuffer(r, in_ptr, buff1, outCount, ptr);
	}
	ptr = _loadOutputBuffer(r, d, buff2, outCount, ptr);

	if (d->ffcoef) {
	    aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
	    if (!d->rs && !d->lp) {
		ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
	    }
	}
	
	if (d->fbcoef) {
	    aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
	    ptr = _saveBuffer(r, in_ptr, buff1, outCount, ptr);
	}

	if (d->lp)
	    ptr = _filterBuffer(d->lp, buff2, outCount, ptr);

	if (!d->rs)
	    ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
	
	if (d->gain)
	    aMix(ptr++, 0, (u16)d->gain, buff2, output);
	
	prev_out_ptr = &r->input[d->output];
    }

    /*
     * bump the master delay line input pointer
     * modulo the length
     */
    r->input += outCount;
    if (r->input > &r->base[r->length])
	r->input -= r->length;

    /*
     * output already in AL_AUX_R_OUT
     *      just copy to AL_AUX_L_OUT
     */
    aDMEMMove(ptr++, output, AL_AUX_L_OUT, outCount<<1);

#ifdef AUD_PROFILE
    PROFILE_AUD(reverb_num, reverb_cnt, reverb_max, reverb_min);
#endif
    return ptr;
}
s32 alFxParam(void *filter, s32 paramID, void *param);
/*
 * This routine gets called by alSynSetFXParam. No checking takes place to 
 * verify the validity of the paramID or the param value. input and output 
 * values must be 8 byte aligned, so round down any param passed. 
 */
s32 alFxParamHdl(void *filter, s32 paramID, void *param);
Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p);
/* 
 * This routine is for loading data from the delay line buff. If the
 * address of curr_ptr < r->base, it will force it to be within r->base
 * space, If the load goes past the end of r->base it will wrap around.
 * Cause count bytes of data at curr_ptr (within the delay line) to be
 * loaded into buff. (Buff is a dmem buffer)
 */
Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
/*
 * This routine is for writing data to the delay line buff. If the
 * address of curr_ptr < r->base, it will force it to be within r->base
 * space. If the write goes past the end of r->base, it will wrap around
 * Cause count bytes of data at buff to be written to delay line, curr_ptr.
 */
Acmd *_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_filterBuffer(ALLowPass *lp, s32 buff, s32 count, Acmd *p);
/*
 * Generate a triangle wave from -1 to 1, and find the current position
 * in the wave. (Rate of the wave is controlled by d->rsinc, which is chorus
 * rate) Multiply the current triangle wave value by d->rsgain, (chorus depth)
 * which is expressed in number of samples back from output pointer the chorus
 * should go at it's full chorus. In otherwords, this function returns a number
 * of samples the output pointer should modulate backwards.
 */
f32 _doModFunc(ALDelay *d, s32 count);



