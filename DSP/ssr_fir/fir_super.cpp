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


#include "fir_super.h"
//______________________________________________________________________________
DATA_T fir0( DATA_T din[L], COEF_T cin[L][NCOEF] ) {

DATA_T d0,d1,d2,d3;
static DATA_T r0=0,r1=0,r2=0,r3=0;
DATA_T dout;

#ifndef JJH_MODS
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0(cin[0]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1(cin[3]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2(cin[2]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3(cin[1]); 

d0 = f0.process(din[0]);
d1 = f1.process(r1);
d2 = f2.process(r2);
d3 = f3.process(r3);
#else
//static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0(); 
//static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1(); 
//static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2(); 
//static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3(); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3;

d0 = f0.process(din[0], cin[0]);
d1 = f1.process(r1, cin[3]);
d2 = f2.process(r2, cin[2]);
d3 = f3.process(r3, cin[1]);
#endif

dout = d0 + d1 + d2 + d3;

//r0 = Reg(din[0]);
r1 = Reg(din[1]);
r2 = Reg(din[2]);
r3 = Reg(din[3]);

return dout;

}


//______________________________________________________________________________
DATA_T fir1( DATA_T din[L], COEF_T cin[L][NCOEF] ) {

DATA_T d0,d1,d2,d3;
static DATA_T r0=0,r1=0,r2=0,r3=0;
DATA_T dout;

#ifndef JJH_MODS
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0(cin[1]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1(cin[0]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2(cin[3]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3(cin[2]); 

d0 = f0.process(din[0]);
d1 = f1.process(din[1]);
d2 = f2.process(r2);
d3 = f3.process(r3);
#else
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3;

d0 = f0.process(din[0], cin[1]);
d1 = f1.process(din[1], cin[0]);
d2 = f2.process(r2, cin[3]);
d3 = f3.process(r3, cin[2]);
#endif

dout = d0 + d1 + d2 + d3;

//r0 = Reg(din[0]);
//r1 = Reg(din[1]);
r2 = Reg(din[2]);
r3 = Reg(din[3]);

return dout;

}


//______________________________________________________________________________
DATA_T fir2( DATA_T din[L], COEF_T cin[L][NCOEF] ) {

DATA_T d0,d1,d2,d3;
static DATA_T r0=0,r1=0,r2=0,r3=0;
DATA_T dout;

#ifndef JJH_MODS
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0(cin[2]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1(cin[1]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2(cin[0]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3(cin[3]); 

d0 = f0.process(din[0]);
d1 = f1.process(din[1]);
d2 = f2.process(din[2]);
d3 = f3.process(r3);
#else
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3;

d0 = f0.process(din[0], cin[2]);
d1 = f1.process(din[1], cin[1]);
d2 = f2.process(din[2], cin[0]);
d3 = f3.process(r3, cin[3]);
#endif

dout = d0 + d1 + d2 + d3;

//r0 = Reg(din[0]);
//r1 = Reg(din[1]);
//r2 = Reg(din[2]);
r3 = Reg(din[3]);

return dout;

}


//______________________________________________________________________________
DATA_T fir3( DATA_T din[L], COEF_T cin[L][NCOEF] ) {

DATA_T d0,d1,d2,d3;
static DATA_T r0=0,r1=0,r2=0,r3=0;
DATA_T dout;

#ifndef JJH_MODS
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0(cin[3]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1(cin[2]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2(cin[1]); 
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3(cin[0]); 

d0 = f0.process(din[0]);
d1 = f1.process(din[1]);
d2 = f2.process(din[2]);
d3 = f3.process(din[3]);
#else
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f0;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f1;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f2;
static fir_nosym_obj<NCOEF, DATA_T, COEF_T, PROD_T, ACC_T> f3;

d0 = f0.process(din[0], cin[3]);
d1 = f1.process(din[1], cin[2]);
d2 = f2.process(din[2], cin[1]);
d3 = f3.process(din[3], cin[0]);
#endif

dout = d0 + d1 + d2 + d3;

//r0 = Reg(din[0]);
//r1 = Reg(din[1]);
//r2 = Reg(din[2]);
//r3 = Reg(din[3]);

return dout;

}

//______________________________________________________________________________
// make sure it is implemented in ROM
//
void init_coef( COEF_T cin[L][NCOEF] ) {

  const COEF_T c_orig[L*NCOEF] = {
      #include "coef128.inc"
      //#include "ramp.inc"
  };

  int i, k = 0;

  // polyphase decomposition
  for (i=0; i<L*NCOEF; i+=L) {
      cin[i%L][k]   = c_orig[i];
      cin[i%L+1][k] = c_orig[i+1];
      cin[i%L+2][k] = c_orig[i+2];
      cin[i%L+3][k] = c_orig[i+3];
      k++;
  }

} 

//______________________________________________________________________________
void fir_super( DATA_T din[L], DATA_T dout[L] ) {
#pragma HLS INLINE region recursive
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_RESHAPE variable=din,dout complete
//static COEF_T cin[L][NCOEF];
COEF_T cin[L][NCOEF];
init_coef(cin);

    dout[0] = fir0( din, cin );
    dout[1] = fir1( din, cin );
    dout[2] = fir2( din, cin );
    dout[3] = fir3( din, cin );

}

