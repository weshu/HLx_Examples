///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: srcCoef.cpp
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file describes the behavior of the coefficient ROM and controller.
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////
#include "multiSRC.h"

#include "filt_coef_rom.h"


void srcCoef(coef_phase_t addr_i, coef_t c[NumTap]){

	static coef_phase_t addr[NumTap];
	static ap_shift_reg<coef_phase_t, ChMux> addr_dly;


	for(int i=0;i<NumTap;i++) c[i] = coef_rom[i][addr[i]];

	// output the coefficients
	for(int i=NumTap-1;i>=1; i--) addr[i]=addr[i-1];

	// total delay is ChMux+1
	// delayline is ChMux, plus one register delay
	addr[0]= addr_dly.shift(addr_i, ChMux-1);


}


