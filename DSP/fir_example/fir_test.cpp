#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fir.h"


int main (void)
{

  inp_data_t signal[SAMPLES];
  out_data_t output[SAMPLES], reference[SAMPLES];
  coef_t taps[N];

  FILE         *fp1, *fp2, *fp3;
  int i, ret_value;
  float val1, val2;
  float diff, tot_diff;
  int val3;

  tot_diff = 0;

  // LOAD FILTER COEFFICIENTS
  fp1=fopen("./data/fir_coeff.dat","r");
  for (i=0;i<N;i++)
  {
	  fscanf(fp1,"%f\n", &val1);
	  taps[i] = (coef_t) val1;
	  fprintf(stdout,"taps[%4d]=%10.5f\n", i, taps[i].to_double());
  }
  fclose(fp1);

  // LOAD INPUT DATA  AND REFERENCE RESULTS
  fp2=fopen("./data/input.dat","r");
  fp3=fopen("./data/ref_res.dat","r");

  for (i=0;i<SAMPLES;i++)
  {
    fscanf(fp2,"%f\n", &val1);
    signal[i]    = (inp_data_t) val1;
    fscanf(fp3,"%f\n", &val2);
    reference[i] = (out_data_t) val2;
	//if (i < 32) fprintf(stdout,"signal[%4d]=%10.5f \t reference[%4d]=%10.5f\n", i, signal[i].to_double(), i, reference[i].to_double() );
  }
  fclose(fp2);
  fclose(fp3);

// CALL DESIGN UNDER TEST
  for (i=0;i<SAMPLES;i++)
  {
	  output[i] = fir_filter(signal[i], taps);
  }

  // WRITE OUTPUT RESULTS
  fp1=fopen("./data/out_res.dat","w");
  for (i=0;i<SAMPLES;i++)
  {
	   fprintf(fp1,"%10.5f\n", output[i].to_double());
  }
  fclose(fp1);

  // CHECK RESULTS
  for (i=0;i<SAMPLES;i++)
  {
	   diff = output[i].to_double() - reference[i].to_double();
	   if (i<64) fprintf(stdout,"output[%4d]=%10.5f \t reference[%4d]=%10.5f\n", i, output[i].to_double(), i, reference[i].to_double() );
	   diff = fabs(diff);
	   tot_diff += diff;
  }
  fprintf(stdout, "TOTAL ERROR =%f\n",tot_diff);

  if (tot_diff < 1.0)
  {
	  fprintf(stdout, "\nTEST PASSED!\n");
	  ret_value =0;
  }
  else
  {
	  fprintf(stdout, "\nTEST FAILED!\n");
	  ret_value =1;
  }


  return ret_value;


}
