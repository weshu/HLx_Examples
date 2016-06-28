#ifndef DUC_2CH_H
#define DUC_2CH_H

#include <fstream>
#include <iostream>
#include "fir.h"
#include "dds.h"

using namespace std;

const int L_INPUT  = 200;
const int L_OUTPUT = 16*L_INPUT;


void dds_frame (incr_t  incr, 
                dds_t   cos_out[L_OUTPUT],
                dds_t   sin_out[L_OUTPUT] );

void mixer ( dds_t dds_cos[L_OUTPUT], 
             dds_t dds_sin[L_OUTPUT], 
	         DATA_T dout_i[L_OUTPUT], 
	         DATA_T dout_q[L_OUTPUT], 
			 DATA_T dout[L_OUTPUT]);

void duc_2ch ( DATA_T din_i[L_INPUT], 
	           DATA_T din_q[L_INPUT], 
			   DATA_T dout[L_OUTPUT],
               incr_t incr );

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// filter spec
// - putting these inside the duc class makes it not compile

// SRRC
static const int Lsrrc_WHOLE = 65;
const COEF_T cin_srrc[Lsrrc_WHOLE] = {
      #include "srrc_0db.inc"
};

// HB1
static const int Lhb1_WHOLE   = 23;
const COEF_T cin_hb1[Lhb1_WHOLE] = {
      #include "b1_fp2.inc"
};

// HB2
static const int Lhb2_WHOLE   = 11;
const COEF_T cin_hb2[Lhb2_WHOLE] = {
      #include "b2_fp2.inc"
};

// HB3
static const int Lhb3_WHOLE   = 7;
const COEF_T cin_hb3[Lhb3_WHOLE] = {
      #include "b3_fp2.inc"
};
  
static const int II_SRRC = 16;
static const int II_HB1  = 8;
static const int II_HB2  = 4;
static const int II_HB3  = 2;


//______________________________________________________________________________
// duc class
// - 4 stages, SRRC -> HB1-> HB2-> HB3 -> DAC
//______________________________________________________________________________

template<int l_INPUT>
class duc_class {

public:

#if 0
// constructor not compiling
interp2_class<Lsrrc_WHOLE,   l_INPUT,   II_SRRC> srrc(cin_srrc);
interp2_hb_class<Lhb1_WHOLE, 2*l_INPUT, II_HB1>  hb1(cin_hb1);
interp2_hb_class<Lhb2_WHOLE, 4*l_INPUT, II_HB2>  hb2(cin_hb2);
interp2_hb_class<Lhb3_WHOLE, 8*l_INPUT, II_HB3>  hb3(cin_hb3);
#endif

interp2_class<Lsrrc_WHOLE,   l_INPUT,   II_SRRC> srrc;
interp2_hb_class<Lhb1_WHOLE, 2*l_INPUT, II_HB1>  hb1;
interp2_hb_class<Lhb2_WHOLE, 4*l_INPUT, II_HB2>  hb2;
interp2_hb_class<Lhb3_WHOLE, 8*l_INPUT, II_HB3>  hb3;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void process(DATA_T din[l_INPUT], DATA_T dout[16*l_INPUT] ) {

#pragma HLS INLINE
#pragma HLS dataflow

    DATA_T srrc_dout[2*l_INPUT];
    DATA_T hb1_dout[4*l_INPUT];
    DATA_T hb2_dout[8*l_INPUT];
    //DATA_T hb3_dout[16*l_INPUT];
        
    srrc.process_frame(din, srrc_dout);
    hb1.process_frame(srrc_dout, hb1_dout);
    hb2.process_frame(hb1_dout, hb2_dout);
    hb3.process_frame(hb2_dout, dout);
}


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
#if 1
//constructor
duc_class( void ) { 
        srrc.init(cin_srrc);
        hb1.init(cin_hb1);
        hb2.init(cin_hb2);
        hb3.init(cin_hb3);
}

//destructor
~duc_class( void ) { }
#endif

};  // duc_class


//______________________________________________________________________________
// duc_2ch class
//______________________________________________________________________________
template<int l_INPUT>
class duc_2ch_class {

static const int l_OUTPUT = 16 * l_INPUT;

duc_class<l_INPUT> duc_i;
duc_class<l_INPUT> duc_q;

public:

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void process(DATA_T din_i[l_INPUT], DATA_T dout_i[l_OUTPUT],
             DATA_T din_q[l_INPUT], DATA_T dout_q[l_OUTPUT]) {

#pragma HLS INLINE
    duc_i.process(din_i, dout_i);
    duc_q.process(din_q, dout_q);
}

};  // duc_2ch_class



#endif
