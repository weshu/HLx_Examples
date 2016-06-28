/*******************************************************************************
Vendor: Xilinx 
Associated Filename: median.h
Purpose: Median algorithm header file for Vivado HLS
Revision History: Settember 24, 2013 - initial release

                                                
*******************************************************************************
© Copyright 2008 - 2012 Xilinx, Inc. All rights reserved. 

This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/



#ifndef _H_MEDIAN_H_
#define _H_MEDIAN_H_


//#include "ap_axi_sdata.h"
//#include "ap_interfaces.h"
//#include "ap_bmp.h"

/* ******************************************************************************* */
// CONSTANT PARAMETERS 

#define MAX_PATH      1000

#define MAX_HEIGHT    1080
#define MAX_WIDTH     1920

//#define KMED3

#if defined(KMED3)
#define KMED            3
#elif defined(KMED5)
#define KMED            5
#elif defined(KMED7)
#define KMED            7
#else
#error <YOU MUST DEFINE THE SIZE OF THE MEDIAN FILTER KERNEL>
#endif

#define KKMED      (KMED/2)
#define BLOCKSIZE     KMED


/* ******************************************************************************* */
// I/O Image Settings
#define INPUT_IMAGE_BASE      "./test_data/noisy_rgb2y_1080p"

#if (KMED==7)
//#error < KMED IS 7>
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_median7x7_1080p"
#elif (KMED==5)
//#error < KMED IS 5>
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_median5x5_1080p"
#elif (KMED==3)
//#error < KMED IS 3>
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_median3x3_1080p"
#endif

//#define REFER_IMAGE_BASE      "./test_data/golden_out_median_1080p"
//#define REFER_VAR_BASE        "./test_data/golden_out_variance_1080p"
//#define OUTPUT_VAR_BASE       "./test_data/c_out_variance_1080p"
//#define C_REFER_IMAGE_BASE    "./test_data/c_golden_out_median_1080p"

/* ******************************************************************************* */
// SMALL FUNCTIONS IN MACRO
#define ABSDIFF(x,y)	( (x)>(y) ? (x - y) : (y - x) )
#define ABS(x)          ( (x)> 0  ? (x)     : -(x)    )
#define MIN(x,y)        ( (x)>(y) ? (y)     :  (x)    )
#define MAX(x,y)        ( (x)>(y) ? (x)     :  (y)    )




/* ******************************************************************************* */
// DATA TYPES

//#define DB_FIXED_POINT

#ifndef DB_FIXED_POINT
typedef unsigned char      pix_t;
#else
#include <ap_int.h>
typedef ap_uint<11> pix_t;
#endif

//#define FLOAT_VAR
#ifndef FLOAT_VAR
typedef int var_t;
#else
typedef float var_t;
#endif
 
typedef struct ap_rgb{
    unsigned char B;
    unsigned char  G;
    unsigned char  R;
  } RGB_t;

typedef   signed short int  int16_t;
typedef unsigned short int uint16_t;

//typedef ap_axiu<32,1,1,1> AXI_PIXEL;
//typedef pix_t  AXI_PIXEL;


/* ******************************************************************************* */
// FUNCTION PROTOTYPES

int main_median(void);

pix_t median(pix_t window[KMED*KMED]); 

void top_median( pix_t   in_pix[MAX_HEIGHT][MAX_WIDTH],
						  pix_t  out_pix[MAX_HEIGHT][MAX_WIDTH],
						  short height, short width); //, FILE *fp);


void ref_median(pix_t in_pix[MAX_HEIGHT][MAX_WIDTH],
							  pix_t ref_pix[MAX_HEIGHT][MAX_WIDTH],
							  short height, short width); //, FILE *fp);

////extern void sw_naive_median( pix_t   in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t  out_pix[MAX_HEIGHT * MAX_WIDTH], short height, short width); //, FILE *fp);
//extern void hw_naive_median( pix_t   in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t  out_pix[MAX_HEIGHT * MAX_WIDTH], short int height, short int width); //, FILE *fp);
//
////void sw_linebuf_median(pix_t in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t ref_pix[MAX_HEIGHT * MAX_WIDTH], short height, short width); //, FILE *fp);
//extern void hw_linebuf_median(pix_t in_pix[MAX_HEIGHT * MAX_WIDTH], pix_t ref_pix[MAX_HEIGHT * MAX_WIDTH], short int height, short int width); //, FILE *fp);



#endif
