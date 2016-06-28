///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: srcMac.cpp
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file describes the behaviour of the multiply-accumulate chain.
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////
#include "multiSRC.h"


acc_t srcMac(bool toshift_i, data_t x_i, coef_t c_i[NumTap]){

	static data_t x_r[NumTap];
	static acc_t acc_r[NumTap];
	static bool toshift_r[NumTap];
	
	// shift register chain
	static ap_shift_reg<data_t, ChMux> shift_reg[NumTap];

	data_t x_mux;
	data_t x_reg;


	// last mac does not need shift in the data
	//mult_t mul_tmp = x_r[NumTap-2] * c_i[NumTap-1];
	//acc_t acc_out = mul_tmp + acc_r[NumTap-2];


	// loop for all the taps
	MULTMACLOOP: for(int i=NumTap-1; i>=1; i--){
	
		// all the MAC run in parallel

		// multiplier with registered output
		mult_t mul_temp = x_r[i] * c_i[i];

		// acc_o is expected to have one clock delay here
		acc_r[i] = mul_temp + acc_r[i-1];

		// read out data from shift register
		x_r[i] = shift_reg[i].read(ChMux-1);
		
		// mux to determine whether shift or not
		// if not shift, then write back the data read from shift register
		x_mux = toshift_r[i-1]? x_r[i-1] : x_r[i];
		
		// shift in and out
		shift_reg[i].shift(x_mux);
		toshift_r[i]=toshift_r[i-1];
	}
	

	// multiply for the first shift reg
	acc_r[0] = x_r[0] * c_i[0];

	// read out data for next
	x_r[0] = shift_reg[0].read(ChMux-1);


	// mux to determine whether shift or not
	// if not shift, then write back the data read from shift register
	toshift_r[0]=toshift_i;
	x_mux = toshift_r[0]? x_i : x_r[0];
	shift_reg[0].shift(x_mux);


	// return the last acc value
	return(acc_r[NumTap-1]);

}
