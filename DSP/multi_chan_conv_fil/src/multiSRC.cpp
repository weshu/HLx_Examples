///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: multiSRC.cpp
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file is the top-level function of multi-channel SRC filter.
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////

#include  "multiSRC.h"

void multiSRC(bool vld_i, data_t x_i, rat_t rat_i, bool& vld_o, acc_t& y_o){

	// it runs every clock cycle
	bool         vld_v;
	bool         toshift_v;
	coef_phase_t addr_v;
	
	coef_t    coef_v[NumTap];

	// it goes into the control module first
	srcCtrl(vld_i, rat_i, vld_v, toshift_v, addr_v);
	
	// use phase_v to find the coefficients
	srcCoef(addr_v, coef_v);

	// call multi-mac
	//static data_t x_r;
	//static bool   tokeep_r;
	
	y_o=srcMac(toshift_v, x_i, coef_v);
	

	// extra logic can be added here, e.g., clipping and rounding


	// also output is_skip flag
	static ap_shift_reg<bool, ChMux+NumTap> vld_shftreg;
	vld_o = vld_shftreg.shift(vld_v, ChMux+NumTap-1);

}
