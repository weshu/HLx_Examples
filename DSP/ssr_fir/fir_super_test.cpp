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


int main() {

  // matlab: load out.dat; x=out';y=x(:);figure,plot(y,'.-');
  ofstream fp_result("out.dat", ofstream::out); 
  fp_result.precision(25); //fp_result.width(30);
  //ifstream fp_cin("coef.dat",ifstream::in);

  int i,j, k = 0;

  int nofsample = 800;
  int init = L*2;
  DATA_T din[L], dout[L];
  
  // impulse input
  for (i=0; i<nofsample; i+=4) {

      if (i==init) {
          din[0] = 0.999999;
          din[1] = 0;
          din[2] = 0;
          din[3] = 0;
      } else if (i==(init+1*L*(NCOEF+1) )) {
          din[0] = 0;
          din[1] = 0.555555;
          din[2] = 0;
          din[3] = 0;
      } else if (i==(init+2*L*(NCOEF+1) )) {
          din[0] = 0;
          din[1] = 0;
          din[2] = 0.222222;
          din[3] = 0;
      } else if (i==(init+3*L*(NCOEF+1) )) {
          din[0] = 0;
          din[1] = 0;
          din[2] = 0;
          din[3] = 0.111111;
      } else if (i==(init+4*L*(NCOEF+1) )) {
          din[0] = -0.99999;
          din[1] = 0;
          din[2] = 0;
          din[3] = 0;
      } else for (j=0;j<L;j++) {
              din[j] = 0; 
      }

      fir_super(din, dout);

      for (j=0;j<L;j++)
         //fp_result << setw(10) << dout[j] << endl;
         fp_result << setw(20) << dout[j] << ", ";

      fp_result << endl;
  }

  return 0;

}
