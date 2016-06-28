/*******************************************************************************
Vendor: Xilinx
Associated Filename: test_xapp_pid.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "xapp_pid.h"

#define N       256 
#define MAX_VAL  64

const float  C  = -0.7931; // derivation constant
const float  Gd =  6.0324; // Derivation gain
const float  Gp = 35.3675; // Proportional gain
const float  Gi =  0.5112; // Integration gain
const float  max_clip =  MAX_VAL-1; 
const float  min_clip = -MAX_VAL; 

int main(void)
{

	int i, ret_val;
	FILE *fp_e, *fp_i, *fp_d, *fp_p, *fp_u, *fp_u2, *fp_e2;
	float diff_E, diff_U, tot_diff_E, tot_diff_U;

	// PID input signals
	bool ResetN =1;
	data_type din[2], coeff[6], dout[2];

	// CLOSED LOOP SYSTEM STATES
	float yd_prev  = 0; float x1_prev  = 0; // derivation  states
	float yi_prev  = 0; float x2_prev  = 0; // integration states
	float y_z1 = 0; float y_z2 = 0; 
	float u_z1 = 0; float u_z2 = 0; 
	
	// closed loop system states
	float ref_out_prev = 0;
	float out_prev = 0;

	// PID AND PLANT SIGNALS
	float w[N]; 
	float u[N],  y[N], e[N];
	float ref_e[N], ref_u[N], ref_y[N], ref_yi[N], ref_yd[N];
	float ref_y_z1 = 0;    
	float ref_y_z2 = 0; 
	float ref_u_z1 = 0; 
	float ref_u_z2 = 0; 

#ifdef PID_DEBUG
	float yd[N], yi[N];
	data_type reg_yd = 0;
	data_type reg_yi = 0;
	float diff_P, diff_D, diff_I;
	float tot_diff_P = 0;
	float tot_diff_D = 0;
	float tot_diff_I = 0;
#endif

	tot_diff_E = 0;
	tot_diff_U = 0;

	// STEP FUNCTION
	w[0] = 0;  for (i=1; i<N; i++) w[i] = 1;


	// file I/O
	if ( ( fp_e = fopen ( (char *) "./test_data/e_res.txt", "w" ) ) == NULL )
	{
		fprintf (stderr, "Cannot open input file %s\n", (char *) "e_res.txt");
		exit (-1 );
	}
	if ( ( fp_i = fopen ( (char *) "./test_data/i_res.txt", "w" ) ) == NULL )
	{
		fprintf (stderr, "Cannot open input file %s\n", (char *) "i_res.txt");
		exit (-1 );
	}
	if ( ( fp_d = fopen ( (char *) "./test_data/d_res.txt", "w" ) ) == NULL )
	{
		fprintf (stderr, "Cannot open input file %s\n", (char *) "d_res.txt");
		exit (-1 );
	}
	if ( ( fp_p = fopen ( (char *) "./test_data/yp_res.txt", "w" ) ) == NULL )
	{
		fprintf (stderr, "Cannot open output file %s\n", (char *) "yp_res.txt");
		exit (-1 );
	}
	if ( ( fp_u = fopen ( (char *) "./test_data/u_res.txt", "w" ) ) == NULL )
	{
		fprintf (stderr, "Cannot open output file %s\n", (char *) "u_res.txt");
		exit (-1 );
	}
	if ( ( fp_u2 = fopen ( (char *) "./test_data/pid_u_res.txt", "w" ) ) == NULL )
	{
		fprintf (stderr, "Cannot open output file %s\n", (char *) "pid_u_res.txt");
		exit (-1 );
	}
	if ( ( fp_e2 = fopen ( (char *) "./test_data/pid_e_res.txt", "w" ) ) == NULL )
	{
		fprintf (stderr, "Cannot open input file %s\n", (char *) "pid_e_res.txt");
		exit (-1 );
	}

	//Gi = coeff[0]; Gd = coeff[1]; C = coeff[2]; Gp = coeff[3];
	//max_lim = coeff[4];//min_lim = coeff[6]; 

	coeff[0] = (data_type) Gi; 	     coeff[1] = (data_type) Gd;
	coeff[2] = (data_type) C;	     coeff[3] = (data_type) Gp;
	coeff[4] = (data_type) max_clip; coeff[5] = (data_type) min_clip;






#if defined(PID_FIXEDPOINT)
		fprintf(stdout, "RUNNING IN 25-bit FIXED POINT\n");
#elif defined(PID_DOUBLEPREC)
		fprintf(stdout, "RUNNING IN 64-bit FIXED POINT\n");
#else
		fprintf(stdout, "RUNNING IN 32-bit FLOATING POINT\n"); 
#endif	

#ifdef PID_DEBUG
		fprintf(stdout, "RUNNING IN DEBUG MODE\n"); 
#else
		fprintf(stdout, "RUNNING IN NORMAL MODE\n"); 
#endif	

	// TEST BENCH 
	for (i = 0; i<N; i++)
	{	
		// error between input and output signals
		ref_e[i] = w[i] - ref_out_prev; // PID IN CLOSED LOOP
		ref_e[i] = (ref_e[i] > max_clip) ? max_clip : ref_e[i];
	    ref_e[i] = (ref_e[i] < min_clip) ? min_clip : ref_e[i];

		float x1 = Gd*ref_e[i];
		float x2 = Gi*ref_e[i];

		// derivation stage
		// Yd(n-1) = -C * Yd(n-1) + X1(n) - X1(n-1)
		ref_yd[i] = -C*yd_prev + x1 - x1_prev;
        yd_prev = ref_yd[i];
        x1_prev = x1;

		// integration
		// Ti(n) = X2(n) + X2(n-1) 
		// Yi(n) = CLIP( Yi(n-1) + Ti(n) )    
		float ti = x2_prev + x2; 
		ti = ti + yi_prev;
		ti = (ti > max_clip) ? max_clip : ti;
	    ti = (ti < min_clip) ? min_clip : ti;
		ref_yi[i] = ti;
        yi_prev   = ti;
		x2_prev   = x2;

		// PID control output signal
		ref_u[i] = ref_e[i] * Gp + ref_yd[i] + ref_yi[i];
		ref_u[i] = (ref_u[i] > max_clip) ? max_clip : ref_u[i];
	    ref_u[i] = (ref_u[i] < min_clip) ? min_clip : ref_u[i];

		// plant
		ref_y[i] = 1.903*ref_y_z1 -0.9048*ref_y_z2 + 2.38e-005 *ref_u[i] + 4.76e-005 *ref_u_z1 + 2.38e-005*ref_u_z2;
        ref_y_z2 = ref_y_z1;
        ref_y_z1 = ref_y[i];
        ref_u_z2 = ref_u_z1;
        ref_u_z1 = ref_u[i];
        ref_out_prev = ref_y[i];

		fprintf(fp_u, "%f\n",  ref_u[i]); // C ref results of the ideal PID
		fprintf(fp_e, "%f\n",  ref_e[i]); // C ref results of the ideal PID


	    // EFFECTIVE Design Under Test
		din[0] = (data_type) w[i];
		din[1] = (data_type) out_prev;

#ifndef PID_DEBUG
		PID_Controller(ResetN, coeff, din, dout);
#else
		PID_Controller(ResetN, coeff, din, dout, &reg_yd, &reg_yi);
		yd[i] =  (float) reg_yd;
		yi[i] =  (float) reg_yi;
#endif //#ifndef PID_DEBUG

		u[i] = (float) dout[0];
		e[i] = (float) dout[1];

		// plant
		y[i] = 1.903*y_z1 -0.9048*y_z2 + 2.38e-005 *u[i] + 4.76e-005 *u_z1 + 2.38e-005*u_z2;
        y_z2 = y_z1; y_z1 = y[i];
        u_z2 = u_z1; u_z1 = u[i];
        out_prev = y[i];


#ifdef PID_DEBUG
		fprintf(fp_d, "%f\n",   (float) yd[i]);
		fprintf(fp_i, "%f\n",   (float) yi[i]);
		fprintf(fp_p, "%f\n",   (float) y[i]);
#endif
		fprintf(fp_u2, "%f\n",  (float) u[i]);
		fprintf(fp_e2, "%f\n",  (float) e[i]);


		// CHECK RESULTS

		// ERROR SIGNAL
		diff_E = (float) fabs( (float) e[i] - (float) ref_e[i]);
		tot_diff_E += diff_E;
		// PID OUTPUT 
		diff_U = (float) fabs( (float) u[i] - (float) ref_u[i]);
		tot_diff_U += diff_U;

#ifdef PID_DEBUG
		// INTEGRATOR OUTPUT
		diff_I = (float) fabs( (float) yi[i] - (float) ref_yi[i]);
		tot_diff_I += diff_I;
		// DERIVATOR OUTPUT
		diff_D = (float) fabs( (float) yd[i] - (float) ref_yd[i]);
		tot_diff_D += diff_D;
		// PLANT OUTPUT
		diff_P = (float) fabs( (float) y[i] - (float) ref_y[i]);
		tot_diff_P += diff_P;
#endif


	} //	for (i = 0; i<N; i++)



	fclose(fp_e); fclose(fp_d); fclose(fp_i); fclose(fp_u); fclose(fp_p); fclose(fp_u2); fclose(fp_e2);
	

#ifdef PID_DEBUG
	fprintf(stdout, "\n TEST RESULT D=%f \n", tot_diff_D);
	fprintf(stdout, "\n TEST RESULT I=%f \n", tot_diff_I);
	fprintf(stdout, "\n TEST RESULT P=%f \n", tot_diff_P);
#endif
	fprintf(stdout, "\n TEST RESULT U=%f \n", tot_diff_U);
	fprintf(stdout, "\n TEST RESULT E=%f \n", tot_diff_E);
	tot_diff_E += tot_diff_U; // total error during simulation
	fprintf(stdout, "\n TEST RESULT TOTAL ERROR =%f \n", tot_diff_E);


	if (tot_diff_E < PID_THRESHOLD) // MY THRESHOLD
	{
		ret_val = 0;
		fprintf(stdout, "\n TEST PASSED! \n");
	}
	else
	{
		ret_val = 1;
		fprintf(stdout, "\n TEST FAILED! \n");
	}

	return ret_val;


}
