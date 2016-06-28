///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: multiSRC.h
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file defines the data types and number of filter types.
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////

#ifndef   _multiSRC_H_
#define   _multiSRC_H_

#include   <fstream>
#include   <iostream>
#include   <iomanip>
#include   <cstdlib>
#include   <cmath>

#include   "ap_int.h"
#include   "ap_shift_reg.h"

using  namespace  std;

// Number of Time Multiplexing Channels
#define ChMux  8  // 245.76/30.72 = 8

// Number of Filter Taps
#define NumTap 47

#define BIT_ACCURATE

#ifdef BIT_ACCURATE
typedef ap_int<18>	coef_t;
typedef ap_int<16>	data_t;
typedef ap_int<18>	add_t;
typedef ap_int<36>  mult_t;
typedef ap_int<48>	acc_t;  // max is ceil(log2(NumTap))+18+16=40 bit
typedef ap_uint<3>  rat_t;
typedef ap_uint<5>  coef_phase_t;
typedef ap_uint<3>  chid_t;

#else
typedef int	          coef_t;
typedef int	          data_t;
typedef int	          add_t;
typedef long long     mult_t;
typedef long long     acc_t;
typedef unsigned char rat_t;
typedef unsigned char coef_phase_t;

#endif


// define base address array

const coef_phase_t STEP_IN_VEC[] ={1, 3, 5, 5, 4, 8, 6};
const coef_phase_t STEP_OUT_VEC[]={1, 4, 8, 6, 3, 5, 5};

#define BA0  0
#define BA1  BA0+STEP_IN_VEC[0]
#define BA2  BA1+STEP_IN_VEC[1]
#define BA3  BA2+STEP_IN_VEC[2]
#define BA4  BA3+STEP_IN_VEC[3]
#define BA5  BA4+STEP_IN_VEC[4]
#define BA6  BA5+STEP_IN_VEC[5]

const coef_phase_t BA_VEC[]={BA0, BA1, BA2, BA3, BA4, BA5, BA6};


// main controller
void srcCtrl(bool vld_i, rat_t rat_i, bool& vld_o, bool& toshift_o, coef_phase_t &addr_o);

// srcCoef function generates coefficients for MAC
void srcCoef(coef_phase_t addr_i, coef_t c_o[NumTap]);

// generic multi-channel MAC unit
acc_t srcMac(bool toshift_i, data_t x_i, coef_t c_i[NumTap]);

// main function
void multiSRC(bool vld_i, data_t x_i, rat_t rat_i, bool& vld_o, acc_t& y_o);


#endif





