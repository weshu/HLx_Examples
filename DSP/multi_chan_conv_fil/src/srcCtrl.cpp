///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: srcCtrl.cpp
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file describes the behavior of the controller that 
//    1) generates the coefficient ROM address 
//    2) determines whether the data in the MAC chain should be shifted or not
//    3) determines whether the MAC chain computed data is a valid output
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////
#include "multiSRC.h"


void srcCtrl(bool vld_i, rat_t rat_i, bool& vld_o, bool& toshift_o, coef_phase_t &addr_o){

	// time multiplex channels
	static coef_phase_t vec_phase[ChMux];
	static coef_phase_t vec_tokeep[ChMux];
	static coef_phase_t vec_isskip[ChMux];
	
	static chid_t cnt_r=0;

	// read out the info from the shift register as output
	coef_phase_t phase = vec_phase[cnt_r];

	// whether to shift the register
	bool tokeep = vec_tokeep[cnt_r];

	// isskip flag for the valid input
	bool isskip = vec_isskip[cnt_r];

	if(cnt_r==ChMux-1) cnt_r=0;
	else cnt_r++;


	// rat_i=0, bypass : addr=0, 
	// rat_i=1, 3/4    : addr=1, 2, 3
	// rat_i=2, 5/8    : addr=4, 5, 6, 7, 8
	// rat_i=3, 5/6    : addr=9, 10, 11, 12, 13

	coef_phase_t base_addr = BA_VEC[rat_i];

	// output the signals
	toshift_o = vld_i & (!tokeep);    // shift when a valid input is available and it is time to shift
	vld_o     = (vld_i & (!isskip)) | tokeep; // 
	addr_o    = phase+base_addr;

	
	// mux for the step_input and step_output
	coef_phase_t step_in  = STEP_IN_VEC[rat_i];
	coef_phase_t step_out = STEP_OUT_VEC[rat_i];


							
	// when there is a valid output out,
	// phase need to increase by step_out
	// and if the new phase is not larger than step_in
	// then it should not shift
	if (vld_o){
		phase+=step_out;
		if(phase<step_in){ // it means that no new data is required for next output
			tokeep=true;
			isskip=false;
		}else{ // new data is needed
			tokeep=false;
			isskip=( (phase>>1) >=step_in);
			phase-=step_in;
		}
	}else if (vld_i){ // a new input is available, but it is to be skipped
		tokeep=false;
		isskip=( (phase>>1) >=step_in);
		phase-=step_in;
	}
	

	static chid_t cnt_w=0;

	// save nxt values into the shift register
	vec_phase[cnt_w]=phase;
	vec_tokeep[cnt_w]=tokeep;
	vec_isskip[cnt_w]=isskip;

	if(cnt_w==ChMux-1) cnt_w=0;
	else cnt_w++;

}

