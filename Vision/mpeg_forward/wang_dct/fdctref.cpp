/* fdctref.c, forward discrete cosine transform, double precision           */

/* Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved. */

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
 * any and all warranties, whether express, implied, or statuary, including any
 * implied warranties or merchantability or of fitness for a particular
 * purpose.  In no event shall the copyright-holder be liable for any
 * incidental, punitive, or consequential damages of any kind whatsoever
 * arising from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs and user's
 * customers, employees, agents, transferees, successors, and assigns.
 *
 * The MPEG Software Simulation Group does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any third-party
 * patents.
 *
 * Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
 * are subject to royalty fees to patent holders.  Many of these patents are
 * general enough such that they are unavoidable regardless of implementation
 * design.
 *
 */

#include <math.h>


#define PI 3.14159265358979323846


/* private data */
static double c[8][8]; /* transform coefficients */

void init_fdct()
{
  int i, j;
  double s;

  for (i=0; i<8; i++)
  {
    s = (i==0) ? sqrt(0.125) : 0.5;

    for (j=0; j<8; j++)
      c[i][j] = s * cos((PI/8.0)*i*(j+0.5));
  }
}

void ref_fdct(short *inp_block, short *out_block)
{
  int i, j, k;
  double s;
  double tmp[64];

  for (i=0; i<8; i++)
    for (j=0; j<8; j++)
    {
      s = 0.0;

      for (k=0; k<8; k++)
        s += c[j][k] * inp_block[8*i+k];

      tmp[8*i+j] = s;
    }

  for (j=0; j<8; j++)
    for (i=0; i<8; i++)
    {
      s = 0.0;

      for (k=0; k<8; k++)
        s += c[i][k] * tmp[8*k+j];

      out_block[8*i+j] = (int)floor(s+0.499999);
      /*
       * reason for adding 0.499999 instead of 0.5:
       * s is quite often x.5 (at least for i and/or j = 0 or 4)
       * and setting the rounding threshold exactly to 0.5 leads to an
       * extremely high arithmetic implementation dependency of the result;
       * s being between x.5 and x.500001 (which is now incorrectly rounded
       * downwards instead of upwards) is assumed to occur less often
       * (if at all)
       */
    }
	return;
}



/**********************************************************/
/* inverse two dimensional DCT, Chen-Wang algorithm       */
/* (cf. IEEE ASSP-32, pp. 803-816, Aug. 1984)             */
/* 32-bit integer arithmetic (8 bit coefficients)         */
/* 11 mults, 29 adds per DCT                              */
/*                                      sE, 18.8.91       */
/**********************************************************/
/* coefficients extended to 12 bit for IEEE1180-1990      */
/* compliance                           sE,  2.1.94       */
/**********************************************************/

/* this code assumes >> to be a two's-complement arithmetic */
/* right shift: (-2)>>1 == -1 , (-3)>>1 == -2               */

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

/* private data */
static short iclip[1024]; /* clipping table */
static short *iclp;

/* row (horizontal) I-DCT
 *
 *           7                       pi         1
 * dst[k] = sum c[l] * src[l] * cos( -- * ( k + - ) * l )
 *          l=0                      8          2
 *
 * where: c[0]    = 128
 *        c[1..7] = 128*sqrt(2)
 **************************************************************************/

static void idctrow(short int *inp_blk, short int *out_blk)
{
  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  if (!((x1 = inp_blk[4]<<11) | (x2 = inp_blk[6]) | (x3 = inp_blk[2]) |
        (x4 = inp_blk[1]) | (x5 = inp_blk[7]) | (x6 = inp_blk[5]) | (x7 = inp_blk[3])))
  {
    out_blk[0]=out_blk[1]=out_blk[2]=out_blk[3]=out_blk[4]=out_blk[5]=out_blk[6]=out_blk[7]=inp_blk[0]<<3;
    return;
  }

  /* stage S0 */
  x1 = inp_blk[4]<<11;
  x2 = inp_blk[6];
  x3 = inp_blk[2];
  x4 = inp_blk[1];
  x5 = inp_blk[7];
  x6 = inp_blk[5];
  x7 = inp_blk[3];
  x0 = (inp_blk[0]<<11) + 128; /* for proper rounding in the last stage */

  /* stage S1 */
  x8 = W7*(x4+x5);
  x4 = x8 + (W1-W7)*x4;
  x5 = x8 - (W1+W7)*x5;
  x8 = W3*(x6+x7);
  x6 = x8 - (W3-W5)*x6;
  x7 = x8 - (W3+W5)*x7;
  
  /* stage S2 */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2);
  x2 = x1 - (W2+W6)*x2;
  x3 = x1 + (W2-W6)*x3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  
  /* stage S3 */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;
  
  /* stage S4 */
  out_blk[0] = (x7+x1)>>8;
  out_blk[1] = (x3+x2)>>8;
  out_blk[2] = (x0+x4)>>8;
  out_blk[3] = (x8+x6)>>8;
  out_blk[4] = (x8-x6)>>8;
  out_blk[5] = (x0-x4)>>8;
  out_blk[6] = (x3-x2)>>8;
  out_blk[7] = (x7-x1)>>8;
}


/* column (vertical) IDCT
 *
 *             7                         pi         1
 * dst[8*k] = sum c[l] * src[8*l] * cos( -- * ( k + - ) * l )
 *            l=0                        8          2
 *
 * where: c[0]    = 1/1024
 *        c[1..7] = (1/1024)*sqrt(2)
 **************************************************************/

static void idctcol(short int *inp_blk, short int *out_blk)
{
  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  /* stage S0 */
  x1 = inp_blk[8*4]<<8;
  x2 = inp_blk[8*6];
  x3 = inp_blk[8*2];
  x4 = inp_blk[8*1];
  x5 = inp_blk[8*7];
  x6 = inp_blk[8*5];
  x7 = inp_blk[8*3];
  x0 = (inp_blk[8*0]<<8) + 8192;

  /* stage S1 */
  x8 = W7*(x4+x5) + 4;
  x4 = (x8+(W1-W7)*x4)>>3;
  x5 = (x8-(W1+W7)*x5)>>3;
  x8 = W3*(x6+x7) + 4;
  x6 = (x8-(W3-W5)*x6)>>3;
  x7 = (x8-(W3+W5)*x7)>>3;
  
  /* stage S2 */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2) + 4;
  x2 = (x1-(W2+W6)*x2)>>3;
  x3 = (x1+(W2-W6)*x3)>>3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  
  /* stage S3 */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;
  
  /* stage S4 */
  /* iclip is the clipping Look-Up-Table computed in init_idct() */	
  out_blk[8*0] = iclp[(x7+x1)>>14]; 
  out_blk[8*1] = iclp[(x3+x2)>>14];
  out_blk[8*2] = iclp[(x0+x4)>>14];
  out_blk[8*3] = iclp[(x8+x6)>>14];
  out_blk[8*4] = iclp[(x8-x6)>>14];
  out_blk[8*5] = iclp[(x0-x4)>>14];
  out_blk[8*6] = iclp[(x3-x2)>>14];
  out_blk[8*7] = iclp[(x7-x1)>>14];
}

/* 
 * two dimensional inverse discrete cosine transform 
 *********************************************************/
void wang_idct(short int *inp_block, short int *out_block)
{
  int i;

  for (i=0; i<8; i++)
    idctrow(inp_block+8*i, out_block+8*i);

  for (i=0; i<8; i++)
    idctcol(out_block+i, out_block+i);
}

void init_idct(void)
{
  int i;

  iclp = iclip+512;
  for (i= -512; i<512; i++)
    iclp[i] = (i<-256) ? -256 : ((i>255) ? 255 : i);
}

/**********************************************************/