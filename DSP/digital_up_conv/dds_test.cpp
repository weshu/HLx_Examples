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

#include "dds.h"

int main(){

//lut_word_t cos_lut[LUTSIZE];
//fine_word_t fine_lut[FINESIZE];
//init_cos_lut( cos_lut, LUTSIZE );
//read_cos_lut( cos_lut, LUTSIZE );
//read_sine_lut( cos_lut, LUTSIZE );
//init_fine_lut( fine_lut, FINESIZE, DELTA );

#ifdef TEST_DITHER
ofstream fp_dit ("dither.txt");
ap_uint<19> dout;
for (int k=0;k<16*4096;k++) {
//for (int k=0;k<64;k++) {
  dout = dither();
  fp_dit << setw(10) << dout <<endl;
}
return 0;
#endif

ofstream fp_dout ("ddsout.txt");
ofstream fp_fine ("ddsfine.txt");

#define WRAP

#ifdef WRAP
int32_tt   incr;
int16_tt   cos_out;
int16_tt   sin_out;

//incr = 303.33333*pow(2,31-11);
incr = 45*pow(2,31-11);

for (int k=0;k<16*4096;k++) {
    dds_wrapper ( incr,  &cos_out, &sin_out );

    fp_fine << setw(10) << k;
    fp_fine << ", " << scientific << cos_out;
    fp_fine << ", " << scientific << sin_out << endl;

}
#else

incr_t  incr;
dds_t   cos_out;
dds_t   sin_out;

incr = 303.33333*pow(2,-11);

for (int k=0;k<16*4096;k++) {
    dds ( incr,  &cos_out, &sin_out );

    fp_fine << setw(10) << k;
    fp_fine << ", " << scientific << cos_out;
    fp_fine << ", " << scientific << sin_out << endl;

}
#endif

fp_dout.close();
fp_fine.close();

  
return 0;

}
