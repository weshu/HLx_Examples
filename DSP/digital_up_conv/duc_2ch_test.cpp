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

#include "duc_2ch.h"
#include <math.h>

//______________________________________________________________________________
void duc_2ch_sw ( DATA_T din_i[L_INPUT], 
	              DATA_T din_q[L_INPUT], 
			      DATA_T dout[L_OUTPUT],
                  incr_t incr ) {

static duc_2ch_class<L_INPUT> f0;

DATA_T dout_i[L_OUTPUT];
DATA_T dout_q[L_OUTPUT];
dds_t  dds_cos[L_OUTPUT];
dds_t  dds_sin[L_OUTPUT];

f0.process(din_i, dout_i, din_q, dout_q);
dds_frame(incr, dds_cos, dds_sin);
mixer(dds_cos, dds_sin, dout_i, dout_q, dout);

}


//______________________________________________________________________________
int main() {

  ofstream fp_result("out.dat",ofstream::out); 
  fp_result.precision(25); 
  fp_result.width(30);

    double tmp;

    const int nofset = 10;

    DATA_T din_i[nofset][L_INPUT];
    DATA_T din_q[nofset][L_INPUT];
    DATA_T dout[nofset][L_OUTPUT];

    DATA_T swout[nofset][L_OUTPUT];

incr_t  incr;
//incr = 303.33333*pow(2,31-11);
incr = 303.33333*pow(2,-11);

    int jj = 0;
	float diff;
	int err_cnt = 0;

    for (int i=0; i<nofset; i++) {
        
        for (int k=0; k<L_INPUT; k++) {
            tmp = cos(2*M_PI*(0.5+(double)jj*33)/(1024));
            din_i[i][k] = tmp;

#if 0
            tmp = 0.3*cos(2*M_PI*(0.5+(double)jj*10)/(1024));
            din_q[i][k] = tmp;
#endif
            tmp = sin(2*M_PI*(0.5+(double)jj*33)/(1024));
            din_q[i][k] = tmp;

            jj++;
        }

        duc_2ch(din_i[i], din_q[i], dout[i], incr);
        duc_2ch_sw(din_i[i], din_q[i], swout[i], incr);

        //for (int k=0; k<L_OUTPUT; k++) 
            //fp_result << dout[i][k] << endl;

        // compare with expected
        for (int k=0; k<L_OUTPUT; k++)  {
		  diff = dout[i][k] - swout[i][k];
          fp_result << dout[i][k] <<", "<<swout[i][k] << endl;
		  if (diff != 0) err_cnt++;
	    }

    }

  if (diff==0) {
      cout <<"============== Test passed "<<endl;
      return 0;
  } else {
      cout <<"============== Test failed, error count =  "<< err_cnt << endl;
      return 1;
  }


}




#if 0
//______________________________________________________________________________
void duc_2ch_sw ( DATA_T din_i[L_INPUT], DATA_T dout_i[L_OUTPUT],
                  DATA_T din_q[L_INPUT], DATA_T dout_q[L_OUTPUT] ) {

static duc_2ch_class<L_INPUT> f0;

f0.process(din_i, dout_i, din_q, dout_q);

}



//______________________________________________________________________________
// no DDS

int main() {

  ofstream fp_result("out.dat",ofstream::out); 
  fp_result.precision(25); 
  fp_result.width(30);

    double tmp;

    const int nofset = 10;

    DATA_T din_i[nofset][L_INPUT], dout_i[nofset][L_OUTPUT];
    DATA_T din_q[nofset][L_INPUT], dout_q[nofset][L_OUTPUT];

    DATA_T swout_i[nofset][L_OUTPUT];
    DATA_T swout_q[nofset][L_OUTPUT];

    int jj = 0;
	float diff;
	int err_cnt = 0;

    for (int i=0; i<nofset; i++) {
        
        for (int k=0; k<L_INPUT; k++) {
            tmp = cos(2*M_PI*(0.5+(double)jj*33)/(1024));
            din_i[i][k] = tmp;

            tmp = 0.3*cos(2*M_PI*(0.5+(double)jj*10)/(1024));
            din_q[i][k] = tmp;
            jj++;
        }

        duc_2ch(din_i[i], dout_i[i], din_q[i], dout_q[i]);
        duc_2ch_sw(din_i[i], swout_i[i], din_q[i], swout_q[i]);

        for (int k=0; k<L_OUTPUT; k++) 
            fp_result << dout_i[i][k] <<", "<< dout_q[i][k] << endl;

        // compare with expected
        for (int k=0; k<L_OUTPUT; k++)  {
		  diff = dout_i[i][k] - swout_i[i][k];
		  if (diff != 0) err_cnt++;
		  diff = dout_q[i][k] - swout_q[i][k];
		  if (diff != 0) err_cnt++;
	    }

    }

  if (diff==0)
      cout <<"============== Test passed "<<endl;
  else
      cout <<"============== Test failed, error count =  "<<err_cnt<< endl;

  return err_cnt;

}

#endif
