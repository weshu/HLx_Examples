/***********************************************
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
***********************************************/

#ifndef _FIR_SUPER
#define _FIR_SUPER

#include <fstream>
#include <iostream>
using namespace std;

#include <ap_fixed.h>

static const int NFRAC = 15;
typedef ap_fixed<1+NFRAC,   1, AP_TRN, AP_WRAP> DATA_T;
typedef ap_fixed<1+NFRAC,   1, AP_TRN, AP_WRAP> COEF_T;
typedef ap_fixed<2+NFRAC+4, 2, AP_TRN, AP_WRAP> PROD_T;
typedef ap_fixed<6+NFRAC+4, 6, AP_TRN, AP_WRAP> ACC_T;

static const int NCOEF = 32;
static const int L = 4;

DATA_T fir0( DATA_T din[L], COEF_T cin[L][NCOEF] );
DATA_T fir1( DATA_T din[L], COEF_T cin[L][NCOEF] );
DATA_T fir2( DATA_T din[L], COEF_T cin[L][NCOEF] );
DATA_T fir3( DATA_T din[L], COEF_T cin[L][NCOEF] );

void init_coef( COEF_T cin[L][NCOEF] );
void fir_super( DATA_T din[L], DATA_T dout[L]);

#define JJH_MODS

//______________________________________________________________________________
// fir class
//
template<int ncoef, class data_t, class coef_t, class prod_t, class acc_t> 
class fir_nosym_obj {

#ifndef JJH_MODS
    coef_t c[ncoef];
#endif
    data_t shift_reg[ncoef];
    acc_t  acc;

public:
#ifndef JJH_MODS
// initialize coefficient
fir_nosym_obj( coef_t coef[ncoef] ) {

    for (int i=0; i<ncoef; i++)
        c[i] = coef[i];
};
#else
fir_nosym_obj() {}
#endif

// MAC
acc_t MAC( data_t din, coef_t coef, acc_t  acc ) {

#define ORIG
#ifdef ORIG // sol1 240 DSP 
    prod_t prod = din*coef;
    acc_t  sum  = prod + acc;
#else 
    //acc_t  sum  = din*coef + acc; // sol2, 240 DSP, more ff/lut
    //
    prod_t prod = din*coef;         // sol3
#pragma HLS resource variable=prod core=Mul2S
    acc_t  sum  = prod + acc;      

#endif

    return sum;
};

// filter
data_t process( data_t din, coef_t c[ncoef]) {
#ifndef JJH_MODS
#pragma HLS INLINE
#endif

#pragma HLS pipeline
#pragma HLS array_reshape variable=c complete dim=1
#pragma HLS array_reshape variable=shift_reg complete dim=1

    int i;
    acc = 0;

    loop_mac: 
        for (i=0; i<ncoef; i++) {
            acc = MAC(shift_reg[i], c[i], acc);
        }

    loop_sr: 
        for (i=ncoef-1; i>0 ; i--) {
            shift_reg[i] = shift_reg[i-1];
        }
        shift_reg[0] = din;

    return acc;

};

};

//______________________________________________________________________________
//  register
//
template<class T>
T Reg(T in) {
#pragma HLS PIPELINE
#pragma HLS INLINE off
#pragma HLS INTERFACE port=return register
return in;
};


#endif

