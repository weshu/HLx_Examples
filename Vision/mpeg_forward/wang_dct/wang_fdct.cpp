/************************************************
Copyright (c) 2016, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software 
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************/

#include "dct.h"




void wang_dct_1d(short int xx[64], short int yy[64], int4_t i, uint1_t row_first)
{

  short int _x0,_x1,_x2,_x3,_x4,_x5,_x6,_x7,_x8;

  int17_t  i17_x1, i17_x2, i17_x3, i17_x4, i17_x5, i17_x7, i17_x8, i17_x0, i17_x6;
  int18_t  i18_x1, i18_x2, i18_x3, i18_x4, i18_x5, i18_x7, i18_x8, i18_x0, i18_x6;
  int19_t  i19_x1, i19_x2, i19_x3, i19_x4, i19_x5, i19_x7, i19_x8, i19_x0, i19_x6;
  int20_t  i20_x1, i20_x2, i20_x3, i20_x4, i20_x5, i20_x7, i20_x8, i20_x0, i20_x6;
  int33_t  i33_x1, i33_x2, i33_x3, i33_x4, i33_x5, i33_x7, i33_x8, i33_x0, i33_x6;

  int35_t  i35_tmp1_mult,  i35_tmp2_mult, i35_tmp1_reg, i35_tmp2_reg;
  int32_t  i32_tmp3_mult,  i32_tmp4_mult, i32_tmp5_mult, i32_tmp8_mult;
  int33_t  i33_tmp6_mult,  i33_tmp7_mult, i33_tmp9_mult, i33_tmp10_mult, i33_tmp11_mult;

  int17_t i17_const_46341 = 46341;
  int17_t i17_const_32768 = 32768;
  int18_t i18_const_1over2 = (row_first ==1) ? 128 : 65536; // 0.5 in different resolutions, for rounding

  int13_t i13_W1 = 2841; /* 2048*sqrt(2)*cos(1*pi/16) */    //12 bits +1 of sign
  int13_t i13_W2 = 2676; /* 2048*sqrt(2)*cos(2*pi/16) */    //12 bits +1 of sign
  int13_t i13_W3 = 2408; /* 2048*sqrt(2)*cos(3*pi/16) */    //12 bits +1 of sign
  int12_t i12_W5 = 1609; /* 2048*sqrt(2)*cos(5*pi/16) */    //11 bits +1 of sign
  int12_t i12_W6 = 1108; /* 2048*sqrt(2)*cos(6*pi/16) */    //11 bits +1 of sign
  int11_t i11_W7 =  565; /* 2048*sqrt(2)*cos(7*pi/16) */    //10 bits +1 of sign

  int14_t i14_W2_plus_W6  = i13_W2 + i12_W6;
  int14_t i14_W2_minus_W6 = i13_W2 - i12_W6;
  int14_t i14_W1_plus_W7  = i13_W1 + i11_W7;
  int14_t i14_W1_minus_W7 = i13_W1 - i11_W7;
  int14_t i14_W5_minus_W3 = i12_W5 - i13_W3;
  int14_t i14_W5_plus_W3  = i12_W5 + i13_W3;

  // 16 bits for the Vertical DCT // 9 bits (-255 to 255) for the Horizontal DCT
  _x0 = xx[0];
  _x1 = xx[1];
  _x2 = xx[2];
  _x3 = xx[3];
  _x4 = xx[4];
  _x5 = xx[5];
  _x6 = xx[6];
  _x7 = xx[7];

  // 17 bits Vert DCT // 10 bits (-510 to 510) Horiz DCT
  i17_x0 = _x0 + _x7;
  i17_x1 = _x1 + _x6;
  i17_x2 = _x2 + _x5;
  i17_x3 = _x3 + _x4;
  i17_x4 = _x0 - _x7;
  i17_x5 = _x3 - _x4;
  i17_x6 = _x1 - _x6;
  i17_x7 = _x2 - _x5;

  /* **************************  stage 1: it gains 1 bit ********************* */
  // 18 bits Vert DCT // 11 bits Horiz DCT

  i18_x8 = i17_x0 - i17_x3;
  i18_x0 = i17_x0 + i17_x3;
  i18_x3 = i17_x1 - i17_x2;
  i18_x1 = i17_x1 + i17_x2;

  i18_x6 = i17_x6 + i17_x7;
  i18_x7 = i17_x6 - i17_x7;

  /* Note that (46341>>16) = 1/sqrt(2) */
  i35_tmp1_mult =  i17_const_46341 * i18_x6;
  i35_tmp2_mult =  i17_const_46341 * i18_x7;

  i35_tmp1_reg = i35_tmp1_mult + i17_const_32768;
  i35_tmp2_reg = i35_tmp2_mult + i17_const_32768;

  // 18+16>>16 = 18bits Vert DCT // 11+16>>16 bits = 11 bits Horiz DCT
  i18_x2 = (i35_tmp1_reg>>16);
  i18_x7 = (i35_tmp2_reg>>16);


  /* **************************  stage 2: it gains 1 bit ********************* */
  // 19 bits Vert DCT  // 12 bits Horiz DCT

  i19_x6 = i18_x0 - i18_x1;
  i19_x0 = i18_x0 + i18_x1;

  i19_x3 = i18_x3 + i18_x8;
  i32_tmp3_mult = i12_W6 * i19_x3;
  i33_x1 = (int32_t) i32_tmp3_mult + i18_const_1over2;  // 19+12 =31 bits Vert DCT  // 12+11 + =24 bits Horiz DCT

  i32_tmp4_mult = i14_W2_plus_W6 * i18_x3;
  i32_tmp5_mult = i14_W2_minus_W6 *i18_x8;

  i33_x3 = i33_x1 - i32_tmp4_mult;
  i33_x8 = i33_x1 + i32_tmp5_mult;

  i19_x1 = i17_x4 - i18_x2;
  i19_x4 = i17_x4 + i18_x2;
  i19_x2 = i17_x5 - i18_x7;
  i19_x5 = i17_x5 + i18_x7;

 /* **************************  stage 3: it gains 1 bit ********************* */

  i20_x4 = i19_x4 + i19_x5;

  i33_tmp6_mult = i14_W1_minus_W7 * i19_x4;
  i33_tmp7_mult = i14_W1_plus_W7  * i19_x5;

  i32_tmp8_mult = i11_W7*i20_x4;

  i33_x7 = i32_tmp8_mult + i18_const_1over2;
  i33_x4 = i33_x7 + i33_tmp6_mult;
  i33_x5 = i33_x7 - i33_tmp7_mult;

  i20_x1 = (i19_x1 + i19_x2);
  i33_tmp9_mult = i13_W3*i20_x1;

  i33_tmp10_mult = i14_W5_minus_W3 * i19_x1;
  i33_tmp11_mult = i14_W5_plus_W3  * i19_x2;

  i33_x7 = i33_tmp9_mult + i18_const_1over2;
  i33_x1 = i33_x7 + i33_tmp10_mult;
  i33_x2 = i33_x7 - i33_tmp11_mult;


 /* **************************  stage 4: ********************* */

  if (row_first==1) // DCT on rows

  {
  _x0=i19_x0<<3;
  _x1=i33_x4>>8;
  _x2=i33_x8>>8;
  _x3=i33_x2>>8;
  _x4=i19_x6<<3;
  _x5=i33_x1>>8;
  _x6=i33_x3>>8;
  _x7=i33_x5>>8;

  }
  else // DCT on colums
  {
  _x0=(i19_x0+32)>>6;
  _x1=i33_x4>>17;			        // 14 bits
  _x2=i33_x8>>17;
  _x3=i33_x2>>17;
  _x4=(i19_x6+32)>>6;
  _x5=i33_x1>>17;
  _x6=i33_x3>>17;
  _x7=i33_x5>>17;
  }

  yy[i+0*8] = _x0;
  yy[i+1*8] = _x1;
  yy[i+2*8] = _x2;
  yy[i+3*8] = _x3;
  yy[i+4*8] = _x4;
  yy[i+5*8] = _x5;
  yy[i+6*8] = _x6;
  yy[i+7*8] = _x7;

}


//void read_data(short int inp_buf[N], short int out_buf[N])
//{
//#pragma HLS PIPELINE
//   int r;
//
//   RD_Loop: for (r = 0; r < N; r++)
//
//#pragma HLS UNROLL
//out_buf[r] = inp_buf[r];
//   return;
//}
//
//void write_data(short int inp_buf[N], short int out_buf[N])
//{
//#pragma HLS PIPELINE
//   int r;
//
//WR_Loop: for (r = 0; r < N; r++)
//
//#pragma HLS UNROLL
//out_buf[r] = inp_buf[r];
//   return;
//}

/* two dimensional inverse discrete cosine transform */

void wang_fdct(short int xx[N], short int yy[N])
{

#pragma HLS INLINE

int i;

short int loc_xx[N];
//short int loc_yy[N];

//read_data(xx, loc_xx);

  // horizontal DCT
L_hor: for (i=0; i<DCT_SIZE; i++)
	{
	//wang_dct_1d( loc_xx+i*DCT_SIZE, loc_yy, i, 1);
	#pragma HLS PIPELINE
	wang_dct_1d( xx+i*DCT_SIZE, loc_xx, i, 1);
	}
  //vertical DCT
 L_vert: for (i=0; i<DCT_SIZE; i++)
 	 {
	//wang_dct_1d( loc_yy+i*DCT_SIZE, loc_xx, i, 0);
	#pragma HLS PIPELINE
	 wang_dct_1d( loc_xx+i*DCT_SIZE, yy, i, 0);
 	 }

 //write_data(loc_xx, yy);



}





