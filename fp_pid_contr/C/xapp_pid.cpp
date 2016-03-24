/*******************************************************************************
Vendor: Xilinx
Associated Filename: xapp_pid.cpp
Purpose: HLS Floating Point PID Controller
Revision History: 23 March, 2016

*******************************************************************************
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

*******************************************************************************/ 

#include "xapp_pid.h"

#ifndef PID_DEBUG
void PID_Controller(bool InitN, data_type coeff[6], data_type din[2], data_type dout[2])
#else
void PID_Controller(bool InitN, data_type coeff[6], data_type din[2], data_type dout[2], data_type *test_yd, data_type *test_yi)
#endif //#ifndef PID_DEBUG

{
#pragma HLS ARRAY_PARTITION variable=coeff complete dim=1
#pragma HLS INLINE off

	// previous PID states: Yd(n-1), X1(n-1), X2(n-1), Yi(n-1)
	static data_type   prev_x1, prev_x2, prev_yd, prev_yi;
	// current local states
	data_type w, e, y, x1, x2, yd, yi;
	// local variables
	data_type max_lim, min_lim, Gi, Gd, C, Gp, tmp;
	data_type pid_mult, pid_addsub, pid_mult2, pid_addsub2;
	
	// get PID input coefficients
	Gi = coeff[0]; 	Gd = coeff[1];
	C  = coeff[2];  Gp = coeff[3];
	max_lim = coeff[4]; min_lim = coeff[5];

	// get PID input signals
	w = din[0]; // effective input signal
	y = din[1]; // closed loop signal

	if (InitN==0)
	{
	  prev_yi = 0; // reset Integrator stage
	  prev_x2 = 0; 
	  prev_x1 = 0; // reset Derivative stage
	  prev_yd = 0;
	}

	// compute error signal E = W - Y
	pid_addsub = w - y;
	e          = (pid_addsub > max_lim) ? max_lim : pid_addsub;
	e          = (pid_addsub < min_lim) ? min_lim : e;

	pid_mult  = Gd * e;
	pid_mult2 = Gi * e;
	x1  = pid_mult;  // input signal of the derivative stage
	x2  = pid_mult2; // input signal of the integration stage

	// Derivation stage
	// Yd(n) = -C*Yd(n-1)+X1(n)-X1(n-1) = X1 - (prev_X1 + C*prev_Yd)
	pid_mult   = C * prev_yd;
	pid_addsub2= x1 - prev_x1; 
	pid_addsub = pid_addsub2 -pid_mult;
	yd         = pid_addsub;

	// Integrator  stage
	// Ti = X2(n) + X2(n-1) 
	// Yi(n) = CLIP( Yi(n-1) + Ti )    
	pid_addsub = prev_x2 + x2;
	pid_addsub2= prev_yi + pid_addsub;
	yi         = (pid_addsub2 > max_lim) ? max_lim : pid_addsub2;
	yi         = (pid_addsub2 < min_lim) ? min_lim : yi;

#ifdef PID_DEBUG
	*test_yd   = yd;
	*test_yi   = yi;
#endif

	// output signal U(n)
	pid_mult   = Gp * e;
    pid_addsub = yi + yd;
	pid_addsub2= pid_addsub + pid_mult;
	tmp        = (pid_addsub2 > max_lim) ? max_lim : pid_addsub2;
	tmp        = (pid_addsub2 < min_lim) ? min_lim : tmp;
    dout[0]    = tmp; // PID output
	dout[1]    = e;   // error reported as output

	// update internal PID states for the next iteration
	prev_x1 = x1; prev_x2 = x2;
	prev_yd = yd; prev_yi = yi;

	return;
}
