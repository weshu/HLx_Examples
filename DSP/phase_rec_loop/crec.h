#ifndef CREC_H_
#define CREC_H_

#include <iostream>
#include <fstream>
using namespace std;

#include "ap_fixed.h"

#include <math.h>

//#define N	10

typedef ap_fixed<16,3>	data_t;  // sxx.xx.....

typedef struct cdata_t {
  data_t i,q;
} cdata_t; 

typedef struct error_t {
  ap_fixed<16,2,AP_TRN,AP_SAT> i,q;
} error_t;

typedef ap_ufixed<2,2> qam_t;

typedef struct ctl_crec_t {
  qam_t          qam;
  // ap_int does not work w/ shifting!
  //ap_int<3>     lf_p;         // 2^0,-1,-2,-3,-4,-5,-6,-7
  //ap_int<3>     lf_i;         // 2^-9,-10,-11,-12,-13,-14,-15,-16
  //ap_int<3>     lf_out_gain;  // 2^0,-1,-2,-3,-4,-5,-6,-7
  char     lf_p;         // 2^0,-1,-2,-3,-4,-5,-6,-7
  char     lf_i;         // 2^-9,-10,-11,-12,-13,-14,-15,-16
  char     lf_out_gain;  // 2^0,-1,-2,-3,-4,-5,-6,-7
  bool     reg_clr; 
  ap_fixed<28,2,AP_TRN,AP_SAT> reg_init;
  //ap_ufixed<8,8> lf_init;
} ctl_crec_t;

typedef ap_fixed<14,2,AP_TRN,AP_SAT> phase_t;   // phase in radian

typedef struct cphase_t {  // phase in x, y
    ap_fixed<12,1> i,q;
} cphase_t;

typedef ap_fixed<28,2> loop_int_t;
typedef ap_fixed<28,2> loop_out_t;

typedef struct hd_t {
  ap_fixed<5,1> i,q;
} hd_t;

#define Q 1024

typedef ap_fixed<16,1,AP_RND_INF,AP_SAT_SYM> cos_t;
typedef ap_fixed<16,1> lut_out_t;

/*
typedef struct cos_t {
  ap_fixed<16,1,AP_RND_INF,AP_SAT_SYM> i,q;
  //ap_fixed<16,1,AP_RND,AP_SAT> i,q;
  //ap_fixed<16,1> i,q;
} cos_t;

typedef struct lut_out_t {
  ap_fixed<16,1> i,q;
} lut_out_t;
*/


#ifdef xx
void crec (
  cdata_t din,
  cdata_t *crec_out,
  cdata_t *err,
  ctl_crec_t control
);
#endif


// apply phase correction
void mix(cdata_t din, cdata_t *dout_mix, cphase_t ph_xy);

// slicer
void slicer (qam_t qam, cdata_t dout_mix, hd_t *hd_out, cdata_t *sd_out, error_t *err);

// determine phase offset
void compute_phase (hd_t hd_out, cdata_t sd_out, phase_t *ph_est);

// loop filter
void loop_filter(phase_t din, loop_out_t *loop_out, loop_int_t *loop_integ, ctl_crec_t control);

// VCO
void vco (loop_out_t loop_out, ap_fixed<16,1> *tphase, cphase_t *ph_offset );

// phase recovery
void phase_recovery (
  hd_t       hd_out, 
  cdata_t    sd_out, 
  loop_int_t *loop_integ, 
  loop_out_t *loop_out, 
  cphase_t   *ph_offset,
  ctl_crec_t control );

// crec
void crec (
    cdata_t     din, 
    cdata_t    *dout,
    cphase_t    ph_in,
    cphase_t   *ph_out,
    loop_int_t *loop_integ, 
    ctl_crec_t  control );


#endif

