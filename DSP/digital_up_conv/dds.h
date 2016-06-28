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

#endif

