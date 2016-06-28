///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: tb_multiSRC.cpp
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file contains the testbench for the multi-channel SRC filter.
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////
#include "../src/multiSRC.h"

int main(void){


	acc_t  y_o;
	int chid;
	int dat;
	
	int outcnt[ChMux];
	int errcnt[ChMux];

	// input test vector
	FILE* fid=NULL;
	fid=fopen("tv_multiSRC_in.txt", "rt");
	if(NULL==fid){
		printf("Error Opening Input Test Vector!\n");
		return(1);
	}

	// output golden test vector
	FILE* ftv[ChMux];
	char basic_str[]="tv_multiSRC_mout";
	char fname[256];
	for(int i=0;i<ChMux;i++){
		sprintf(fname, "%s_%d.txt", basic_str, i);
		ftv[i]=NULL;
		ftv[i]=fopen(fname, "rt");
		if(NULL==ftv[i]){
			printf("\n--------\nError Opening Golden Output Test Vector: %s!\n--------", fname);
			return(1);
		}
	}
	
	FILE* fod=NULL;
	fod=fopen("cout.txt", "wt");
	if(NULL==fod){
			printf("Error Opening Output Test Vector!\n");
			return(1);
	}

	
	for(int i=0;i<ChMux;i++) outcnt[i]=errcnt[i]=0;

	chid = NumTap%ChMux;
	chid = (ChMux-chid)%ChMux;

	int postruncnt=0;
	int preruncnt=0;

	while(postruncnt<ChMux*NumTap){

		bool vld_i=false;
		data_t x_i=0;
		rat_t rat_i=0;

		if(!feof(fid)){
			// read one input
			fscanf(fid, "%d", &dat);
			vld_i=dat&1;
			fscanf(fid, "%d", &dat);
			x_i=dat;
			fscanf(fid, "%d", &dat);
			rat_i=dat;
		}else{
			postruncnt++;
		}
		
		// call DUT
		bool vld_o;
		multiSRC(vld_i, x_i, rat_i, vld_o, y_o);
		
		// check output
		if(vld_o){
			
			// increment output counter
			outcnt[chid]++;
			
			// read expected output
			long long int dat;
			fscanf(ftv[chid], "%lld", &dat);
			acc_t tv_val = dat;
			
			// check the DUT output
			if(y_o!=tv_val) errcnt[chid]++;
			
		}


		if(preruncnt<ChMux+NumTap) preruncnt++;
		else{
			int a[3];
			a[0] = y_o&0xFFFF;
			a[1] = (y_o>>16)&0xFFFF;
			a[2] = ((y_o>>32)&0x7FFF) | ((vld_o==1)? 0x8000 : 0);
			fprintf(fod, "%04X%04X%04X\n", a[2], a[1], a[0]);
		}


		// update channel id		
		chid = (chid+1)%ChMux;
	}
	
	fclose(fid);
	fclose(fod);
	
	// close all opened files
	int errall=0;
	for(int i=0;i<ChMux;i++){
		fclose(ftv[i]);
		errall+=errcnt[i];
		cout << "\t[" << i << "]\t Total " << outcnt[i] << " Output Samples \t" << errcnt[i] << " Errors." << endl;
	}

	return(errall);

}
