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

#ifndef DDS_H_
#define DDS_H_

#include <iostream>
#include <fstream>
using namespace std;

#include "ap_fixed.h"
#include <math.h>


// phase accumulator 
typedef ap_fixed<32,1> incr_t;  // s.xxxx, +/- 1 = +/- pi = +fs/2 to -fs/2
typedef ap_fixed<32,1> acc_t;   // s.xxxx, +/- 1 = +/- pi = +fs/2 to -fs/2

// cos lut address, word size
const int NLUT     = 10;               // bitwidth for cos lut address, covers one quadrant
const int LUTSIZE  = 1024;             // 2^NLUT
typedef ap_uint<NLUT+2> lut_adr_t;     // covers 4 quadrant
typedef ap_uint<NLUT>   quad_adr_t;    // covers 1 quadrant

// rounding makes huge difference in the noise floor
typedef ap_fixed<18,1,AP_RND_CONV,AP_SAT_SYM> lut_word_t;
//typedef ap_fixed<18,1> lut_word_t;
//typedef double lut_word_t;


// fine lut address, word size
const int NFINE     = 9;               // bitwidth for fine lut address, covers one quadrant
const int FINESIZE  = 512;             // 2^NFINE
typedef ap_uint<NLUT+2> fine_adr_t;    // covers 4 quadrant
//typedef ap_fixed<18,1,AP_RND_INF,AP_SAT_SYM> fine_word_t;
//typedef ap_fixed<18,1> fine_word_t;
//typedef ap_fixed<18,-8> fine_word_t;
//typedef ap_fixed<18,-10> fine_word_t;
//typedef double fine_word_t;
typedef ap_fixed<18,-7> fine_word_t;

const double DELTA = M_PI/(2*LUTSIZE*FINESIZE); // fine lut resolution, range covers 0 to pi/(2*LUTSIZE)

// DDS output = f(cos lut, fine table)
//typedef ap_fixed<18,1,AP_RND_CONV,AP_SAT_SYM> dds_t;
typedef ap_fixed<16,1,AP_RND_CONV,AP_SAT_SYM> dds_t;

ap_uint<19> dither();
void init_cos_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE );
void read_cos_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE );
void read_sine_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE );
void init_fine_lut( fine_word_t fine_lut[FINESIZE], const int FINESIZE, const double DELTA );
void dds ( incr_t  incr,  dds_t*  cos_out,  dds_t* sin_out );

// added for SDSOC
typedef int   int32_tt;
typedef short int16_tt;

void dds_wrapper ( int32_tt  incr,  int16_tt*  cos_out,  int16_tt* sin_out );
void xxx ( incr_t incr, dds_t*  cos_out, dds_t* sin_out );

#endif

