/* SDK sp/sprite.c */
/*
 * Copyright 1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 */
#include <assert.h>
#include <sp.h>
#include <PR/os_version.h>
// TODO: this comes from a header
#ident "$Revision: 1.17 $"

/*
 * spSetZ()
 *
 */
void 
spSetZ (Sprite *sp, s32 z);
/*
 * spMove()
 *
 *   Moves a sprite on the screen.
 */
void 
spMove (Sprite *sp, s32 x, s32 y);
extern s32 scissor_xmax;
extern s32 scissor_ymax;
extern s32 scissor_xmin;
extern s32 scissor_ymin;
/*
 * spScissor()
 *
 *   Sets the scissoring box to be used for subsequent Sprite Drawing
 */
void 
spScissor (s32 xmin, s32 xmax, s32 ymin, s32 ymax );
extern int *prev_bmbuf;

/*
 * drawbitmap()
 *
 *   Draws a bitmap from a sprite at a location on the screen.  If the
 *   "usevert" flag is set, then the routine will use vertices and triangles
 *   to draw the bitmap.  Otherwise, the texture rectangle instruction
 *   will be used.
 */

void 
drawbitmap (Gfx **glp, Sprite *s, Bitmap *b,
	    s32 x, s32 y, s32 xx, s32 yy, s32 fs, s32 ft, s32 sx, s32 sy)
{
    s32 rs, rt;
    s32 rxh, ryh;
    s32 rxl, ryl;
    Gfx *gl;
    s32 tex_width, tex_height;
    Gtexrect gt, *g;
    s32 s_clamp, t_clamp,
	s_mask,  t_mask, 
	s_lod,   t_lod;

    g = &gt;

    tex_width = b->width_img;
    tex_height= s->bmHreal;
    
#ifdef rmDEBUG
    rmonPrintf("\tdrawbitmap (buf= 0x%08x; x,y= %d,%d; w,h= %d,%d )\n",
	    b->buf, x, y, b->width, s->bmheight );
#endif
    gl = *glp;

    /* Scissoring */
    if( (x >= scissor_xmax) || (y >= scissor_ymax) ) {
#ifdef DEBUG_SCISSOR
	emPrintf("Sprite Scissoring: Upper Left corner (%d,%d) beyond range (%d,%d)\n",
		x, y, scissor_xmax, scissor_ymax );
#endif
	return;
    }

    if( (xx < scissor_xmin) || (yy < scissor_ymin) ) {
#ifdef DEBUG_SCISSOR
	emPrintf("Sprite Scissoring: Lower Right corner (%d,%d) not in range (%d,%d)\n",
		xx, yy, scissor_xmin, scissor_ymin );
#endif
	return;
    }


	if (x < scissor_xmin) {
	    rxh = scissor_xmin*4;
	    rs = (b->s<<5) + fs + (((scissor_xmin - x)*sx)>>5);
#ifdef DEBUG_SCISSOR
	emPrintf("Sprite Scissoring: Left Edge (%d) not in range (%d,%d)\n",
		x, scissor_xmin, scissor_xmax );
#endif
	} else {
	    rxh = x*4;
	    rs = (b->s<<5) + fs;
	};

	if (y < scissor_ymin) {
	    ryh = scissor_ymin*4;
	    rt = (b->t<<5) + ft + (((scissor_ymin-y)*sy)>>5);
#ifdef DEBUG_SCISSOR
	emPrintf("Sprite Scissoring: Top Edge (%d) not in range (%d,%d)\n",
		y, scissor_ymin, scissor_ymax );
#endif
	} else {
	    ryh = y*4;
	    rt = (b->t<<5) + ft;
	}

	if( xx >= scissor_xmax ) {
	    rxl = scissor_xmax*4;
#ifdef DEBUG_SCISSOR
	emPrintf("Sprite Scissoring: Right Edge (%d) not in range (%d,%d)\n",
		xx, scissor_xmin, scissor_xmax );
#endif
	} else {
	    rxl = xx*4;
	};

	if( yy >= scissor_ymax ) {
	    ryl = scissor_ymax*4;
#ifdef DEBUG_SCISSOR
	emPrintf("Sprite Scissoring: Bottom Edge (%d) not in range (%d,%d)\n",
		yy, scissor_ymin, scissor_ymax );
#endif
	} else {
	    ryl = yy*4;
	};

	s_clamp = G_TX_CLAMP;	t_clamp = G_TX_CLAMP;
	s_mask  = G_TX_NOMASK;	t_mask  = G_TX_NOMASK;
	s_lod   = G_TX_NOLOD;	t_lod   = G_TX_NOLOD;

#define LOAD_TEX

#ifdef LOAD_TEX
    if( b->buf != prev_bmbuf ) {
	switch(s->bmsiz) 
	  {
	  case G_IM_SIZ_4b:
		if (s->attr & SP_TEXSHUF) {
		  gDPLoadTextureBlock_4bS(gl++, b->buf, s->bmfmt,
								  tex_width, tex_height, 0, 
								  s_clamp, t_clamp,
								  s_mask,  t_mask, 
								  s_lod,   t_lod);
		} else {
		  gDPLoadTextureBlock_4b(gl++, b->buf, s->bmfmt,
								 tex_width, tex_height, 0, 
								 s_clamp, t_clamp,
								 s_mask,  t_mask, 
								 s_lod,   t_lod);
		};
	    break;

	  case G_IM_SIZ_8b:
	    if (s->attr & SP_TEXSHUF) {
		gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_8b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    } else {
		gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_8b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    };
	    break;
	  case G_IM_SIZ_16b:
	    if (s->bmfmt == G_IM_FMT_YUV ) {
		if (s->attr & SP_TEXSHUF) {
		    gDPLoadTextureBlockYuvS(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
					tex_width, tex_height, 0,
					   s_clamp, t_clamp,
					   s_mask,  t_mask, 
					   s_lod,   t_lod);
		} else {
		    if( b->LUToffset != 0 ) { /* Split Y and UV areas */
			unsigned char *uv, *addr;
			int j;
			s32 tmem, siz;

			gDPSetTextureImage( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 1, b->buf);
			gDPSetTile( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 0, 256, G_TX_LOADTILE, 0,
					t_clamp, t_mask, t_lod,
					s_clamp, s_mask, s_lod );
			gDPLoadSync( gl++ );
			gDPLoadBlock( gl++, G_TX_LOADTILE, 0,0, tex_width*tex_height -1,
				      CALC_DXT(tex_width,1) );
			gDPLoadSync( gl++ );

			uv = ((unsigned char *)b->buf) +
				((tex_width*tex_height)/2)*b->LUToffset;

			for( j=0; j<=(tex_height/2); j++ ) {
			    addr = uv + ((tex_height/2) -2 -j)*tex_width;
			    siz = 3*tex_width;
			    tmem = (tex_width/8)*(tex_height - 2 - 2*j);
			    if( tmem < 0 ) {
				addr += (-tmem)*8;
				siz -= (-tmem)*8;
				tmem = 0;
			    };
			    if( j == 0 )
				siz = 2*tex_width;

			    gDPSetTextureImage( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 1, addr);
			    gDPSetTile( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 0, tmem,
					    G_TX_LOADTILE, 0,
					    t_clamp, t_mask, t_lod,
					    s_clamp, s_mask, s_lod );
			    gDPLoadSync( gl++ );
			    gDPLoadBlock( gl++, G_TX_LOADTILE, 0,0, siz -1,
					  CALC_DXT(tex_width,1) );
			    gDPLoadSync( gl++ );
			};

			gDPSetTile( gl++, s->bmfmt, G_IM_SIZ_16b, (((tex_width) * 1)+7)>>3, 0,
				    G_TX_RENDERTILE, 0,
				    t_clamp, t_mask, t_lod,
				    s_clamp, s_mask, s_lod );
			gDPSetTileSize( gl++, G_TX_RENDERTILE, 0, 0,
					((tex_width)-1) << G_TEXTURE_IMAGE_FRAC,
					((tex_height)-1) << G_TEXTURE_IMAGE_FRAC );

		    } else {
			gDPLoadTextureBlockYuv(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
					    tex_width, tex_height, 0,
					       s_clamp, t_clamp,
					       s_mask,  t_mask, 
					       s_lod,   t_lod);
		    };
		};
	    } else {
		if (s->attr & SP_TEXSHUF) {
		    gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
					tex_width, tex_height, 0,
					   s_clamp, t_clamp,
					   s_mask,  t_mask, 
					   s_lod,   t_lod);
		} else {
		    gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
					tex_width, tex_height, 0,
					   s_clamp, t_clamp,
					   s_mask,  t_mask, 
					   s_lod,   t_lod);
		};
	    };
	    break;
	  case G_IM_SIZ_32b:
	    if (s->attr & SP_TEXSHUF) {
		gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_32b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    } else {
		gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_32b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    };
	    break;
       }
       prev_bmbuf = b->buf;
    }
#endif /* LOAD_TEX */


	g->cmd = G_TEXRECT;
	g->tile = 0;

	g->xh = rxh;
	g->xl = rxl;
	g->yh = ryh;
	g->yl = ryl;

	g->s = rs;
	g->t = rt;

	if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV) )
	    g->dsdx = sx * 4;
	else
	    g->dsdx = sx;

	g->dtdy = sy;

#ifdef rmDEBUG
    rmonPrintf("\trect (xh,l= %d,%d, yh,l= %d,%d, s,t= %d,%d, dsdx=%d, dtdy=%d )\n",
	    rxh,rxl,ryh,ryl,rs,rt,g->dsdx,sy );
#endif

    gSPTextureRectangle(gl++,g->xh,g->yh,g->xl,g->yl,g->tile,g->s,g->t,g->dsdx,g->dtdy);

    gDPPipeSync(gl++);

    *glp = gl;
}
extern u16 sp_attr;
void
spInit( Gfx **glistp );
/*
 * spDraw()
 *
 *   Draws all of the sprites that are associated with bitmaps and that
 *   are supposed to be drawn (spDraw()).  They are drawn from 0 to maxsp,
 *   so sprites with higher numbers will overwrite lower-numbered sprites.
 *
 */
Gfx *
spDraw ( Sprite   *s );
void
spFinish( Gfx **glistp );
