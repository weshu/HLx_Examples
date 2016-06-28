#ifndef _H_FIR_H_
#define _H_FIR_H_

#define N	16

#define SAMPLES 1024


//#define SC_INCLUDE_FX
//#include <systemc.h>

#define DB_FIXED_POINT


#ifdef DB_FIXED_POINT
#include "ap_fixed.h"

typedef ap_fixed<18,2>	coef_t;
typedef ap_fixed<48,12>	out_data_t;
typedef ap_fixed<18,2>	inp_data_t;
typedef ap_fixed<48,12>	acc_t;

#else //FULLY ANSI C
typedef           int coef_t;
typedef long long int out_data_t;
typedef           int inp_data_t;
typedef long long int acc_t;

#endif

//template <typename OUT_T, typename INP_T, typename COE_T, int LEN>
//OUT_T fir_filter ( INP_T x, COE_T c[LEN] );

out_data_t fir_filter ( inp_data_t x, coef_t c[N] );

#endif // _H_FIR_H_
