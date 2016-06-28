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

/* ************ starting point of  module qfloatdct.c *************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "dct.h"

/* the input_block array contains the values obtained by
   subtracting 128 to the following 8x8 array of pixels */

static dct_data_t original_block[8][8] = {
  {168,    161,    161,    150,    154,    168,    164,    154},
  {171,    154,    161,    150,    157,    171,    150,    164},
  {171,    168,    147,    164,    164,    161,    143,    154},
  {164,    171,    154,    161,    157,    157,    147,    132},
  {161,    161,    157,    154,    143,    161,    154,    132},
  {164,    161,    161,    154,    150,    157,    154,    140},
  {161,    168,    157,    154,    161,    140,    140,    132},
  {154,    161,    157,    150,    140,    132,    136,    128}
 };


static dct_data_t input_block[8*8] = {
   40,33,33,22,26,40,36,26,
   43,26,33,22,29,43,22,36,
   43,40,19,36,36,33,15,26,
   36,43,26,33,29,29,19,4 ,
   33,33,29,26,15,33,26,4 ,
   36,33,33,26,22,29,26,12,
   33,40,29,26,33,12,12,4 ,
   26,33,29,22,12,4 ,8 ,0
};

/*
   The following matrix is defined as: [C(mn)] = k(m)/2 cos [m(2n+1)pi/16]
   with pi=3.14...
   and k(m) = 1         if (m !=0)
   and k(m) = 1/sqrt(2) if m==0
*/

static const double C_matrix[8][8] = {
  {0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553},
  {0.490393, 0.415735, 0.277785, 0.097545,-0.097545,-0.277785,-0.415735,-0.490393},
  {0.461940, 0.191342,-0.191342,-0.461940,-0.461940,-0.191342, 0.191342, 0.461940},
  {0.415735,-0.097545,-0.490393,-0.277785, 0.277785, 0.490393, 0.097545,-0.415735},
  {0.353553,-0.353553,-0.353553, 0.353553, 0.353553,-0.353553,-0.353553, 0.353554},
  {0.277785,-0.490393, 0.097545, 0.415735,-0.415735,-0.097545, 0.490393,-0.277785},
  {0.191342,-0.461940, 0.461940,-0.191342,-0.191342, 0.461940,-0.461940, 0.191342},
  {0.097545,-0.277785, 0.415735,-0.490393, 0.490393,-0.415735, 0.277785,-0.097545}
};





/****************************************************/

int main(void) 
{
  int j,i;
  dct_data_t block[8*8];
  dct_data_t tmp_block[8*8];

  int diff      =  0;
  int tot_diff  =  0;
  int threshold = 64;
  int ret_value =  0;

  init_fdct(); // needed by REF  FDCT 
  init_idct(); // needed by WANG IDCT 


  printf("\nOriginal data (-128) in the 8x8 Block\n");
  for(j=0;j<8;j++) {
    printf("\n");
    for(i=0;i<8;i++) {
      printf("%4d\t",    input_block[j*8+i]);
    }

  }
  printf("\n");

  // reference DCT
  ref_fdct(input_block, block);
  printf("\nREF FDCT output data\n");
  for(j=0;j<8;j++) {
    printf("\n");
    for(i=0;i<8;i++) {
      printf("%4d\t", block[j*8+i]);
    }
  }
  printf("\n");


  // Design Under Test DCT
  top_fdct(input_block, tmp_block);

  //restore range
#ifdef DB_ORIGINAL
  for(j=0;j<8;j++) {
    for(i=0;i<8;i++) {
    	tmp_block[j*8+i] = tmp_block[j*8+i]>>3;
    }
  }
#endif

  printf("\n DUT  FDCT output data\n");
  for(j=0;j<8;j++) {
    printf("\n");
    for(i=0;i<8;i++) {
      printf("%4d\t",tmp_block[j*8+i]);
    }
  }
  printf("\n");

  printf("\n Difference between REF and DUT FDCT data\n");
  for(j=0;j<8;j++) {
    printf("\n");
    for(i=0;i<8;i++) {
	  diff = tmp_block[j*8+i]-block[j*8+i];
	  diff = DB_ABS(diff);
	  tot_diff = tot_diff + diff;
      printf("%4d\t",diff);
	  }
  }
  printf("\n");

 
  wang_idct(tmp_block, block);
  printf("\n WANG IDCT output data\n");
  for(j=0;j<8;j++) {
    printf("\n");
    for(i=0;i<8;i++) {
      printf("%4d\t",block[j*8+i]);
    }
  }
  printf("\n");


  printf("\n Difference between Original (-128) and Reconstructed data\n");
  for(j=0;j<8;j++) {
    printf("\n");
    for(i=0;i<8;i++) {
	  diff = input_block[j*8+i]-block[j*8+i];
	  diff = DB_ABS(diff);
	  tot_diff = tot_diff + diff;
      printf("%4d\t",diff);
	  }
  }
  printf("\n");

  


   if(tot_diff <= threshold ){
      printf("    *** *** *** *** \n");
      printf("    Results are good \n");
      printf("    *** *** *** *** \n");
      ret_value = 0;
   } else {
      printf("    *** *** *** *** \n");
      printf("    BAD!! error= %d \n", tot_diff);
      printf("    *** *** *** *** \n");
      ret_value = 1;
   }

   return ret_value;



}
	
	/* ***************** ending point of  module qfloatdct.c ************************* */
