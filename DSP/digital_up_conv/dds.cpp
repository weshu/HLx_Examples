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

//______________________________________________________________________________ 
// dds.cpp:
// - barebone direct digital synthesizer
//
//
// a. paek, sep 2013
//______________________________________________________________________________ 


#include "duc_2ch.h"
//#include "dds.h"

//______________________________________________________________________________ 
void dds_frame ( incr_t   incr, 
                 dds_t    cos_out[L_OUTPUT],
                 dds_t    sin_out[L_OUTPUT] ) {

dds_t dds_cos;
dds_t dds_sin;

L1: for(int i=0;i<L_OUTPUT;i++) {
#pragma HLS pipeline rewind
    dds(incr, &dds_cos, &dds_sin);
	cos_out[i] = dds_cos;
	sin_out[i] = dds_sin;
}


}


//______________________________________________________________________________ 
void dds ( incr_t    incr, 
           dds_t*    cos_out,
           dds_t*    sin_out ) {

//#pragma HLS INTERFACE s_axilite port=incr bundle=DDS_BUS
//#pragma HLS INTERFACE s_axilite port=return bundle=DDS_BUS
//#pragma HLS INTERFACE axis port=cos_out
//#pragma HLS INTERFACE axis port=sin_out

// #pragma HLS pipeline

//static const lut_word_t cos_lut[LUTSIZE];
lut_word_t cos_lut[LUTSIZE];
init_cos_lut( cos_lut, LUTSIZE );

// fine table related
fine_word_t fine_lut[FINESIZE];
init_fine_lut( fine_lut, FINESIZE, DELTA );
fine_adr_t fine_adr;
fine_word_t fine_word;


lut_adr_t  full_adr;         // cover full quadrant
quad_adr_t lsb;              // cover 1/4 quadrant
quad_adr_t cos_adr, sin_adr; 

ap_uint<2>  msb;             // specify which quadrant
lut_word_t  cos_lut_word; 
lut_word_t  sin_lut_word; 


//________________ phase accumulator
static acc_t acc = 0;
acc += incr;


//________________ look up cos/sine table
full_adr = acc(31,20);
msb      = full_adr(11,10);
lsb      = full_adr(9,0);

    // right top
    if (msb==0) { 
       cos_adr      = lsb;
       cos_lut_word = cos_lut[cos_adr];

       if (lsb==0) sin_lut_word = 0;
       else { 
         sin_adr      = -lsb;
         sin_lut_word =  cos_lut[sin_adr];
       }

    // left top
    } else if (msb==1) {
       if (lsb==0) cos_lut_word = 0;
       else { 
         cos_adr      = -lsb;
         cos_lut_word = -cos_lut[cos_adr];
       }

       sin_adr      = lsb;
       sin_lut_word = cos_lut[sin_adr];

    // right bot
    } else if (msb==3) {
       if (lsb==0) cos_lut_word = 0;
       else { 
         cos_adr      = -lsb;
         cos_lut_word =  cos_lut[cos_adr];
       }
         sin_adr      =  lsb;
         sin_lut_word = -cos_lut[sin_adr];

    // left bot
    } else             { 
         cos_adr      =  lsb;
         cos_lut_word = -cos_lut[cos_adr];

       if (lsb==0) sin_lut_word = 0;
       else { 
         sin_adr      = -lsb;
         sin_lut_word = -cos_lut[sin_adr];
       }
    }

    //fp_dout << setw(10) << full_adr;
    //fp_dout << ", " << scientific << cos_lut_word;
    //fp_dout << ", " << scientific << sin_lut_word << endl;

    // adjustment w/ fine table
    fine_adr  = acc(19,11);
    fine_word = fine_lut[fine_adr];

    dds_t cos_dds, sin_dds;
    cos_dds = cos_lut_word - sin_lut_word * fine_word;
    sin_dds = sin_lut_word + cos_lut_word * fine_word;

    //fp_fine << setw(10) << full_adr;
    //fp_fine << ", " << scientific << cos_dds;
    //fp_fine << ", " << scientific << sin_dds << endl;

    *cos_out = cos_dds;
    *sin_out = sin_dds;

}




//______________________________________________________________________________ 
void init_cos_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE ){

double cos_double;
//ofstream fp_dout ("debug.txt");

// #define FULL
//________________________________
#ifdef MIDPOINT
// store single quadrant
  for (int i=0;i<LUTSIZE;i++) {
      //cos_double = cos(2*M_PI*(0.0+(double)i)/(4*LUTSIZE));
      cos_double = cos(2*M_PI*(0.5+(double)i)/(4*LUTSIZE));
      cos_lut[i] = cos_double;
      fp_dout << scientific << cos_double <<endl;
  }


#ifdef FULL
// store full quadrant
ofstream fp_ideal ("ideal.txt");
  for (int i=0;i<4*LUTSIZE;i++) {
      cos_double = cos(2*M_PI*(0.5+(double)i)/(4*LUTSIZE));
      fp_ideal << scientific << cos_double <<endl;
  }
#endif

//________________________________
// not the mid point
#else
// store single quadrant
  for (int i=0;i<LUTSIZE;i++) {
      cos_double = cos(2*M_PI*(0.0+(double)i)/(4*LUTSIZE));
      cos_lut[i] = cos_double;
      //fp_dout << scientific << cos_double <<endl;
  }

#ifdef FULL
// store full quadrant
ofstream fp_ideal ("ideal.txt");
  for (int i=0;i<4*LUTSIZE;i++) {
      cos_double = cos(2*M_PI*(0.0+(double)i)/(4*LUTSIZE));
      fp_ideal << scientific << cos_double <<endl;
  }
#endif


#endif


}


//______________________________________________________________________________ 
void read_cos_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE ) {

  lut_adr_t i;            // cover full quadrant
  quad_adr_t lsb,adr;     // cover 1/4 quadrant
  ap_uint<2>  msb;        // specify which quadrant
  lut_word_t  lut_word; 

  ofstream fp_dout ("fullcos.txt");

  for (int k=0;k<4*LUTSIZE;k++) {

    i    = k;
    msb  = i(11,10);
    lsb  = i(9,0);

    if (msb==0) {        // right top
       adr      = lsb;
       lut_word = cos_lut[adr];
    } else if (msb==1) {  // left top 
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word = -cos_lut[adr];
       }
    } else if (msb==3) {  // right bot
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word =  cos_lut[adr];
       }
    } else             {  // left bot 
         adr      =  lsb;
         lut_word = -cos_lut[adr];
    }

    fp_dout << scientific << lut_word << endl;

  }


}

//______________________________________________________________________________ 
void read_sine_lut( lut_word_t cos_lut[LUTSIZE], const int LUTSIZE ) {

  lut_adr_t i;            // cover full quadrant
  quad_adr_t lsb,adr;     // cover 1/4 quadrant
  ap_uint<2>  msb;        // specify which quadrant
  lut_word_t  lut_word; 

  ofstream fp_dout ("fullsine.txt");

  for (int k=0;k<4*LUTSIZE;k++) {

    i    = k;
    msb  = i(11,10);
    lsb  = i(9,0);

    if (msb==1) {         // left top
       adr      = lsb;
       lut_word = cos_lut[adr];
    } else if (msb==2) {  // left bot 
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word = -cos_lut[adr];
       }
    } else if (msb==0) {  // right top
       if (lsb==0) lut_word = 0;
       else { 
         adr      = -lsb;
         lut_word =  cos_lut[adr];
       }
    } else             {  // right bot 
         adr      =  lsb;
         lut_word = -cos_lut[adr];
    }

    fp_dout << scientific << lut_word << endl;

  }


}


//______________________________________________________________________________ 
//dithering
// z^19 + z^18 + z^14 + 1, from mathworks
// x^19 + x^6  + x^5 + x + 1, used here, not sure if it is optimum
//
ap_uint<19> dither() {

static ap_uint<1> sr[19] = {0,0,0,0,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1};
ap_uint<1> sin;
 
sin = sr[18] ^ sr[5] ^ sr[4] ^ sr[0];

LOOP_sr: 
for (int i=18; i>0; i--) {
    sr[i] = sr[i-1];
}

sr[0] = sin;

//for (int j=18; j>=0; j--)
    //cout<< setw(2) << sr[j];
//cout << endl;

ap_uint<19> whole;
LOOP_whole: 
    for (int i=0; i<19; i++) whole[i] = sr[i];
    //cout << ":   whole = " << setw(16) << whole << endl;
    //cout << setw(16) << whole << endl;

return whole;

}


//______________________________________________________________________________ 
void init_fine_lut( fine_word_t fine_lut[FINESIZE], 
                    const int FINESIZE, const double delta ) {

//double fine_double;
double sine_double;
//ofstream fp_dout ("fine.txt");

  for (int i=0;i<FINESIZE;i++) {
      //fine_double = cos(delta*(double)i);
      sine_double = sin(delta*(double)i);
      fine_lut[i] = sine_double;
      //fp_dout << scientific << fine_double <<", " << scientific << sine_double <<endl;
  }


}

