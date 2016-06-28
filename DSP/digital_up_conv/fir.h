#ifndef _FIR_H
#define _FIR_H

#include <fstream>
#include <iostream>
using namespace std;

//______________________________________________________________________________
// filter precision
// - precisions used by all the FIR class
//______________________________________________________________________________

#define HLS

#ifdef HLS
#include <ap_fixed.h>
static const int NFRAC = 14;
typedef ap_fixed<3+NFRAC,  3, AP_TRN, AP_WRAP> DATA_T;
typedef ap_fixed<4+NFRAC,  4, AP_TRN, AP_WRAP> DATA2_T;
typedef ap_fixed<2+NFRAC,  2, AP_RND_CONV, AP_WRAP> COEF_T;
typedef ap_fixed<5+NFRAC,  5, AP_RND_CONV, AP_WRAP> PROD_T; // rounding helps ~20db dc offset
typedef ap_fixed<10+NFRAC, 10, AP_TRN, AP_WRAP> ACC_T;
#else
typedef float DATA_T;
typedef float DATA2_T;
typedef float COEF_T;
typedef float PROD_T;
typedef float ACC_T;
#endif

//______________________________________________________________________________
// single rate, non symmetric fir class 
//
// - parameter: 
//    -l_WHOLE:  number of taps/coeff
//    -l_SAMPLE: number of data samples to process
//    -II_GOAL:  initiation interval goal
//______________________________________________________________________________

template<int l_WHOLE, int l_SAMPLE, int II_GOAL>
class nosym_class {

    // use assert to check the template parameter

    DATA_T sr[l_WHOLE];
    ACC_T  acc;
    COEF_T coeff[l_WHOLE];

public:

    //_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
    // MAC engine

    ACC_T MAC( DATA_T din, COEF_T coef, ACC_T  acc ) {

        PROD_T  prod   = din * coef;      
        ACC_T   sum    = prod + acc;      
        return sum;
    };


    //_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
    // filter

    void process ( DATA_T din, DATA_T* dout) {

    #pragma HLS INLINE
	  
    // using 'factor' instead of 'complete' uses BRAM instead of FF
    #pragma HLS array_reshape variable=sr  complete
    #pragma HLS array_reshape variable=coeff complete dim=0

    acc = 0;

    LOOP_MAC: 
    for (int i=0; i<l_WHOLE; i++) {
        acc = MAC(sr[i], coeff[i], acc);
    }

    LOOP_SR:  
    for (int i=l_WHOLE-1; i>0 ; i--) {
        sr[i] = sr[i-1];
    }

    sr[0] = din;
    *dout = acc;

    }

   //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
   // filter frame
   //

   void process_frame(DATA_T din[l_SAMPLE], DATA_T dout[l_SAMPLE])
   {
// not much difference inline
//#pragma HLS INLINE
       DATA_T tout;

   L_FRAME: for (int i=0; i<l_SAMPLE; i++ ) {
   #pragma HLS pipeline II=II_GOAL rewind
           process (din[i], &tout);
		   dout[i] = tout;
       }
   }


    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
    // initialize coeff
    //

    void init(const COEF_T cin[l_WHOLE]) {
    L_COEFF_INIT : 
        for (int i=0; i<l_WHOLE;i++) 
                coeff[i] = cin[i];
    }

    //constructor
    nosym_class(const COEF_T cin[l_WHOLE]) {
        init(cin);
    }

    //destructor
    ~nosym_class(void) {
    }

}; // nosym_class



//______________________________________________________________________________
// single rate, symmetric fir classs
//
// - parameter: 
//    -l_WHOLE:  number of taps
//    -l_SAMPLE: number of data sample
//    -II_GOAL:  initiation interval goal
//______________________________________________________________________________

template<int l_WHOLE, int l_SAMPLE, int II_GOAL>
class sym_class {

// use assert to check the template parameter


static const int odd    = l_WHOLE % 2;
static const int l_HALF = l_WHOLE/2;
static const int l_COEF = l_WHOLE/2 + odd;  // number of unique coefficients

DATA_T sr[l_WHOLE];
ACC_T  acc;
COEF_T coeff[l_COEF];

public:

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// MAC engine
ACC_T MAC_preadd( DATA_T din0, DATA_T din1, COEF_T coef, ACC_T acc ) {

    DATA2_T preadd = din0 + din1; 
    PROD_T  prod   = preadd * coef;      
    ACC_T   sum    = prod + acc;      

    return sum;
};


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// filter
void process ( DATA_T din, DATA_T* dout) {

#pragma HLS INLINE

// using 'factor' instead of 'complete' uses BRAM instead of FF
#pragma HLS array_reshape variable=sr  complete
#pragma HLS array_reshape variable=coeff complete dim=0

acc = 0;

LOOP_MAC: 
for (int i=0; i<l_HALF; i++) {
    acc = MAC_preadd (sr[i], sr[l_WHOLE-1-i], coeff[i], acc);
}

if (odd)
    acc = MAC_preadd (sr[l_HALF], 0, coeff[l_HALF], acc);

LOOP_SR:  for (int i=l_WHOLE-1; i>0 ; i--) {
            sr[i] = sr[i-1];
}

sr[0] = din;

*dout = acc;

}


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// filter frame
void process_frame(DATA_T din[l_SAMPLE], DATA_T dout[l_SAMPLE]) {

   DATA_T tout;

   L_FRAME: for (int i=0; i<l_SAMPLE; i++ ) {
   #pragma HLS pipeline II=II_GOAL rewind
           process (din[i], &tout);
		   dout[i] = tout;
       }

}


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// initialize coeff
void init(const COEF_T cin[l_COEF]) {
L_COEFF_INIT : for (int i=0; i<l_COEF;i++) 
                coeff[i] = cin[i];
}

//constructor
sym_class(const COEF_T cin[l_COEF]) {
    init(cin);
}

//destructor
~sym_class(void) {
}



}; // sym_class


//______________________________________________________________________________
// single rate, halfband fir classs
//
// - parameter: 
//    -l_WHOLE:  number of taps
//    -l_SAMPLE: number of data sample
//    -II_GOAL:  initiation interval goal
//______________________________________________________________________________

template<int l_WHOLE, int l_SAMPLE, int II_GOAL>
class hb_class {

// use assert to check the template parameter

static const int l_HALF = l_WHOLE/2;      
static const int l_COEF = (l_WHOLE+1)/4+1; // number of unique nonzero coeff

DATA_T sr[l_WHOLE];
ACC_T  acc;
COEF_T coeff[l_COEF];

public:
//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// MAC engine
ACC_T MAC_preadd( DATA_T din0, DATA_T din1, COEF_T coef, ACC_T acc ) {

    DATA2_T preadd = din0 + din1; 
    PROD_T  prod   = preadd * coef;      
    ACC_T   sum    = prod + acc;      

    return sum;
};

//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// filter
void process ( DATA_T din, DATA_T* dout) {

// critical, without it II is 4 times big
#pragma HLS INLINE

// using 'factor' instead of 'complete' uses BRAM instead of FF
#pragma HLS array_reshape variable=sr  complete
#pragma HLS array_reshape variable=coeff complete dim=0

acc = 0;
unsigned short k=0;

LOOP_MAC: 
for (int i=0; i<l_COEF-1; i++) {
    acc = MAC_preadd (sr[k], sr[l_WHOLE-1-k], coeff[i], acc);
    k+=2;
}

// center tap
acc = MAC_preadd (sr[l_HALF], 0, coeff[l_COEF-1], acc);

LOOP_SR:  
for (int i=l_WHOLE-1; i>0 ; i--) {
     sr[i] = sr[i-1];
}

sr[0] = din;
*dout = acc;

}


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// filter frame
void process_frame(DATA_T din[l_SAMPLE], DATA_T dout[l_SAMPLE]) {

   DATA_T tout;

   L_FRAME: for (int i=0; i<l_SAMPLE; i++ ) {
   #pragma HLS pipeline II=II_GOAL rewind
           process (din[i], &tout);
		   dout[i] = tout;
       }
}


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// initialize coeff
void init(const COEF_T cin[l_COEF]) {
L_COEFF_INIT : for (int i=0; i<l_COEF;i++) 
                coeff[i] = cin[i];
}

//constructor
hb_class(const COEF_T cin[l_COEF]) {
    init(cin);
}

//destructor
~hb_class(void) {
}


}; // hb_class



//______________________________________________________________________________
// interpolate by 2 FIR class
//
// - make sure the coeff is odd length, so both subfilter will be symmetric
// - parameter: 
//    -l_WHOLE:   number of taps of prototype filter (not the decomposed subfilter)
//    -l_INPUT:  number of data samples
//    -II_GOAL:   initiation interval goal
//
//______________________________________________________________________________

template<int l_WHOLE, int l_INPUT, int II_GOAL>
class interp2_class {

// use assert to check the template parameter

static const int INTERP_FACTOR = 2;
static const int L_SUB         = l_WHOLE/INTERP_FACTOR;  // 32
static const int ODD           = l_WHOLE % 2;            // 1
static const int l_TDL         = L_SUB + ODD;            // 33
static const int l_NONZERO     = L_SUB/2 + ODD;          // 17

DATA_T sr[l_TDL];
ACC_T  acc0, acc1;
COEF_T coeff[2][l_NONZERO];

public:

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// MAC engine
ACC_T MAC_preadd( DATA_T din0, DATA_T din1, COEF_T coef, ACC_T  acc ) {

    DATA2_T preadd = din0 + din1; 
    PROD_T  prod   = preadd * coef;      
    ACC_T   sum    = prod + acc;      

    return sum;
};


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void process ( DATA_T din, DATA_T* dout0, DATA_T* dout1) {

#pragma HLS INLINE

// using 'factor' instead of 'complete' uses BRAM instead of FF
#pragma HLS array_reshape variable=sr  complete
#pragma HLS array_reshape variable=coeff complete dim=0

acc0 = 0;
acc1 = 0;

LOOP_MAC: for (int i=0; i<l_NONZERO-ODD; i++) {

    // even number of taps, has one more than odd one
    acc0 = MAC_preadd (sr[i], sr[l_TDL-1-i], coeff[0][i], acc0);

    // odd number of taps
    acc1 = MAC_preadd (sr[i], sr[l_TDL-1-ODD-i], coeff[1][i], acc1);
}

// center tap
acc0 = MAC_preadd (sr[l_NONZERO-1], 0, coeff[0][l_NONZERO-1], acc0);

LOOP_SR:  for (int i=l_TDL-1; i>0 ; i--) {
            sr[i] = sr[i-1];
}

sr[0] = din;

*dout0 = acc0;
*dout1 = acc1;

}

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
void process_frame(DATA_T din[l_INPUT], DATA_T dout[2*l_INPUT]) {

    DATA_T dout0, dout1;
        
    L_process_frame: 
        for (int i=0; i<l_INPUT; i++ ) {
#pragma HLS pipeline II=II_GOAL rewind
            process (din[i], &dout0, &dout1);
            dout[2*i]   = dout0;
            dout[2*i+1] = dout1;
        }
}


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// initialize coefficient for polyphase decomposition 
// - test out even length
void init (const COEF_T coef_in[l_WHOLE] ) {

static const int nofphase = 2;

  bool scale = 1;
  COEF_T gain = scale ? 2:1;  // only for DUC

  for (int i=0;i<l_NONZERO-ODD;i++) 
      for (int k=0;k<2;k++) {

          coeff[k][i] = gain * coef_in[2*i+k];
          //cout <<"["<<k<<"]"<<"["<<i<<"] = " <<2*i+k <<endl;
      }

  // number of taps is one greater for odd phase filter
  if (ODD) {

      int k=0;
      int i=l_NONZERO-1;

      coeff[k][i] = gain * coef_in[2*i];
      //cout <<"["<<k<<"]"<<"["<<i<<"] = " <<2*i <<"<=== last " << endl;
  }

} 

    //constructor
    interp2_class(const COEF_T cin[l_WHOLE]) {
        init(cin);          
    }

    interp2_class(void) {
    }

    //destructor
    ~interp2_class(void) {
    }

}; // interp2_class



//______________________________________________________________________________
// interpolate by 2 halfband fir class
//
// - parameter: 
//    -l_WHOLE:   number of taps
//    -l_INPUT:  number of input data samples
//    -II_GOAL:   initiation interval goal
//
//
//    -l_TDL:     number of stages in tap delay line 
//    -l_NONZERO: number of unique coefficients
//______________________________________________________________________________

template<int l_WHOLE, int l_INPUT, int II_GOAL>
class interp2_hb_class {

// use assert to check the template parameter
//  (l_WHOLE+1) % 4 == 0?

static const int l_TDL     = (l_WHOLE+1)/2; // (23+1)/2 = 12
static const int l_NONZERO = 1+l_TDL/2;     // 1 + 12/2 = 7

DATA_T sr[l_TDL];
ACC_T  acc;
COEF_T coeff[l_NONZERO];

public:

//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// MAC engine
ACC_T MAC_preadd( DATA_T din0, DATA_T din1, COEF_T coef, ACC_T  acc ) {

    DATA2_T preadd = din0 + din1; 
    PROD_T  prod   = preadd * coef;      
    ACC_T   sum    = prod + acc;      

    return sum;
};


//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// filter
void process ( DATA_T din, DATA_T* dout0, DATA_T* dout1 ) {

#pragma HLS array_reshape variable=sr complete
#pragma HLS array_reshape variable=coeff complete dim=0

acc = 0;
LOOP_MAC: for (int i=0; i<l_NONZERO-1; i++) {
    acc = MAC_preadd (sr[i], sr[l_TDL-1-i], coeff[i], acc);

}

LOOP_SR:  for (int i=l_TDL-1; i>0 ; i--) {
            sr[i] = sr[i-1];
}

sr[0] = din;

*dout0 = acc;
*dout1 = coeff[l_NONZERO-1]*sr[l_NONZERO-1];

}

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// filter frame
void process_frame(DATA_T din[l_INPUT], DATA_T dout[2*l_INPUT]) {

    DATA_T dout0, dout1;
        
    L_process_frame: 
        for (int i=0; i<l_INPUT; i++ ) {
#pragma HLS pipeline II=II_GOAL rewind
            process (din[i], &dout0, &dout1);
            dout[2*i]   = dout0;
            dout[2*i+1] = dout1;
        }
}


//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// initialize coefficient for halfband filter
void init(const COEF_T cin[] ) {

  bool scale = 1;
  COEF_T gain = scale ? 2:1;

  for (int i=0; i<l_NONZERO-1; i++) {
      coeff[i]  = gain * cin[2*i];
  }

  int i    = l_NONZERO-1;
  coeff[i] = gain * cin[2*i-1];

  //for (int i=0; i<l_NONZERO; i++)
          //cout << coef[i] << endl;
  //cout<< "------" <<endl;
  
}; 

    //constructor
    interp2_hb_class(const COEF_T cin[l_WHOLE]) {
        init(cin);          
    }

    interp2_hb_class(void) {
    }

    //destructor
    ~interp2_hb_class(void) {
    }

}; // interp2_hb_class




//______________________________________________________________________________
// decimate by 2, symmetric fir classs
//
// - parameter: 
//    -l_WHOLE:   number of taps
//    -l_OUTPUT:  number of output data samples
//    -II_GOAL:   initiation interval goal
//______________________________________________________________________________

template<int l_WHOLE, int l_OUTPUT, int II_GOAL>
class decim2_class {

// use assert to check the template parameter

static const int odd    = l_WHOLE % 2;
static const int l_HALF = l_WHOLE/2;
static const int l_COEF = l_WHOLE/2 + odd;

DATA_T sr[l_WHOLE];
ACC_T  acc;
COEF_T coeff[l_COEF];

public:

//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// MAC engine
ACC_T MAC_preadd( DATA_T din0, DATA_T din1, COEF_T coef, ACC_T acc ) {

    DATA2_T preadd = din0 + din1; 
    PROD_T  prod   = preadd * coef;      
    ACC_T   sum    = prod + acc;      

    return sum;
};


//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// filter
void process ( DATA_T din0, DATA_T din1, DATA_T* dout) {

// using 'factor' instead of 'complete' uses BRAM instead of FF
#pragma HLS array_reshape variable=sr  complete
#pragma HLS array_reshape variable=coeff complete dim=0

acc = 0;

LOOP_MAC: 
for (int i=0; i<l_HALF; i++) {
    acc = MAC_preadd (sr[i], sr[l_WHOLE-1-i], coeff[i], acc);
}

if (odd) {
    acc = MAC_preadd (sr[l_HALF], 0, coeff[l_HALF], acc);
}

LOOP_SR:  
for (int i=l_WHOLE-1; i>1 ; i--)
            sr[i] = sr[i-2];

sr[1] = din0;
sr[0] = din1; // most recent sample

*dout = acc;

}

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
// filter frame
void process_frame(DATA_T din[2*l_OUTPUT], DATA_T dout[l_OUTPUT]) {

DATA_T din0, din1;
        
L_frame: for (int i=0; i<l_OUTPUT; i++ ) {
#pragma HLS pipeline II=II_GOAL rewind
            din0 = din[2*i];
            din1 = din[2*i+1];
            process (din0, din1, dout[i]);
        }
}

//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// initialize coefficient 
void init(const COEF_T cin[l_WHOLE] ) {

  for (int i=0; i<l_COEF-1; i++) {
      coeff[i]  = cin[i];
  }

}; 

    //constructor
    decim2_class(const COEF_T cin[l_WHOLE]) {
        init(cin);          
    }

    decim2_class(void) {
    }

    //destructor
    ~decim2_class(void) {
    }

}; // decim2_class




//______________________________________________________________________________
// decimate by 2, halfband fir classs
//
// - parameter: 
//    -l_WHOLE:   number of taps
//
//______________________________________________________________________________

template<int l_WHOLE>
class hb_decim2_class {

// use assert to check the template parameter

public:

static const int l_HALF = l_WHOLE/2;       
static const int l_COEF = (l_WHOLE+1)/4+1; // number of nonzero coeff

DATA_T sr[l_WHOLE];
ACC_T  acc;

//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// MAC engine

ACC_T MAC_preadd( DATA_T din0, DATA_T din1, COEF_T coef, ACC_T acc ) {

    DATA2_T preadd = din0 + din1; 
    PROD_T  prod   = preadd * coef;      
    ACC_T   sum    = prod + acc;      

    return sum;
};

//_  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
// filter

void process ( DATA_T din0, DATA_T din1, DATA_T* dout, const COEF_T cin[l_COEF]) {

// using 'factor' instead of 'complete' uses BRAM instead of FF
#pragma HLS array_reshape variable=sr  complete
#pragma HLS array_reshape variable=cin complete dim=0

acc = 0;
unsigned short k=0;

LOOP_MAC: 
for (int i=0; i<l_COEF-1; i++) {
    acc = MAC_preadd (sr[k], sr[l_WHOLE-1-k], cin[i], acc);
    k+=2;  // skipping zero coefficient
}

// center tap
acc = MAC_preadd (sr[l_HALF], 0, cin[l_COEF-1], acc);

#if 0
LOOP_SR:  
for (int i=l_HALF-1; i>0 ; i--) {
     sr[2*i]   = sr[2*i-1];
     sr[2*i-1] = sr[2*i-2];
}
#endif

LOOP_SR:  
for (int i=l_WHOLE-1; i>1 ; i--)
            sr[i] = sr[i-2];

sr[1] = din0;
sr[0] = din1; // most recent sample

*dout = acc;

}

}; // hb_decim2_class


#endif
