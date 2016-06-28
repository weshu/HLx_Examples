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

#if 1
//______________________________________________________________________________
void duc_2ch ( DATA_T din_i[L_INPUT], 
	           DATA_T din_q[L_INPUT], 
			   DATA_T dout[L_OUTPUT],
               incr_t incr ) {

//#pragma HLS interface ap_fifo depth=L_INPUT  port=din_i
//#pragma HLS interface ap_fifo depth=L_INPUT  port=din_q
//#pragma HLS interface ap_fifo depth=L_OUTPUT port=dout_i
//#pragma HLS interface ap_fifo depth=L_OUTPUT port=dout_q

//#pragma HLS interface ap_ctrl_none port=return  // to avoid bubble

//#pragma HLS interface s_axilite port=incr
#pragma HLS interface ap_stable port=incr

#pragma HLS interface axis depth=300  port=din_i
#pragma HLS interface axis depth=300  port=din_q
#pragma HLS interface axis depth=4800 port=dout

#pragma HLS dataflow

static duc_2ch_class<L_INPUT> f0;

DATA_T dout_i[L_OUTPUT];
DATA_T dout_q[L_OUTPUT];
dds_t  dds_cos[L_OUTPUT];
dds_t  dds_sin[L_OUTPUT];

f0.process(din_i, dout_i, din_q, dout_q);
dds_frame(incr, dds_cos, dds_sin);
mixer(dds_cos, dds_sin, dout_i, dout_q, dout);

}

#endif


//______________________________________________________________________________
void mixer ( dds_t dds_cos[L_OUTPUT], 
             dds_t dds_sin[L_OUTPUT], 
	         DATA_T dout_i[L_OUTPUT], 
	         DATA_T dout_q[L_OUTPUT], 
			 DATA_T dout[L_OUTPUT]) {

static DATA_T dout_tmp;

L1: for(int i=0;i<L_OUTPUT;i++) {
#pragma HLS pipeline rewind
    dout[i]  = dout_tmp;
    dout_tmp = dout_i[i] * dds_cos[i] - dout_q[i] * dds_sin[i];
}

}
 


#if 0
//______________________________________________________________________________
// no DDS

void duc_2ch ( DATA_T din_i[L_INPUT], DATA_T dout_i[L_OUTPUT],
               DATA_T din_q[L_INPUT], DATA_T dout_q[L_OUTPUT] ) {

//#pragma HLS interface ap_fifo depth=L_INPUT  port=din_i
//#pragma HLS interface ap_fifo depth=L_INPUT  port=din_q
//#pragma HLS interface ap_fifo depth=L_OUTPUT port=dout_i
//#pragma HLS interface ap_fifo depth=L_OUTPUT port=dout_q

#pragma HLS interface ap_ctrl_none port=return

#pragma HLS interface axis depth=300  port=din_i
#pragma HLS interface axis depth=300  port=din_q
#pragma HLS interface axis depth=4800 port=dout_i
#pragma HLS interface axis depth=4800 port=dout_q

static duc_2ch_class<L_INPUT> f0;

f0.process(din_i, dout_i, din_q, dout_q);

}

#endif


