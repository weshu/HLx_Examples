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

// REMEMBER TO SCALE BY 8 THE OUTPUT RESULTS (>>3)

void new_dct_1d(dct_data_t src[N], dct_data_t dst[N], char off)
{
#pragma HLS PIPELINE
   unsigned char k, n;
   int tmp;
   const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
	#include "dct_coeff_table.h"
   };

DCT_Outer_Loop:
   for (k = 0; k < DCT_SIZE; k++) {
#pragma HLS PIPELINE
DCT_Inner_Loop:
      for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
#pragma HLS UNROLL factor=8
#pragma HLS PIPELINE
         int coeff = (int)dct_coeff_table[k][n];
         tmp += src[n] * coeff;
      }
      dst[k*DCT_SIZE+off] = DESCALE(tmp, CONST_BITS);
   }
}

void my_dct(dct_data_t in_block[N], dct_data_t out_block[N])
{
#pragma HLS INLINE

   dct_data_t row_outbuf[N];
   unsigned char i, j;

   // DCT rows
Row_DCT_Loop:
   for(i = 0; i < DCT_SIZE; i++) {
#pragma HLS PIPELINE
      new_dct_1d(in_block+DCT_SIZE*i, row_outbuf, i);
   }


   // DCT columns
Col_DCT_Loop:
   for (i = 0; i < DCT_SIZE; i++) {
#pragma HLS PIPELINE
      new_dct_1d(row_outbuf+DCT_SIZE*i, out_block, i);
   }


}




void top_fdct(dct_data_t in_block[N], dct_data_t out_block[N])
{
#ifdef DB_ORIGINAL
	my_dct(in_block, out_block);
#else
	wang_fdct(in_block, out_block);
#endif
}
