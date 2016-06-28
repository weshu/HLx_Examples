/*******************************************************************************
Vendor: Xilinx 
Associated Filename: main_test.cpp
Purpose: Testbench file for image median 3x3 
Revision History: September 24, 2013 - initial release
                                                
*******************************************************************************
© Copyright 2008 - 2013 Xilinx, Inc. All rights reserved. 

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

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_defines.h"
#include "ap_bmp.h"

//#define MAX_PATH 1000


int main(void) 
{

  int  x,     y;
  int  width, height;
  char *tempbuf;
  char *tempbuf1;
  char *tempbuf2;
  char *tempbuf3;
  char *tempbuf4;

  FILE *fp;

  int check_results = 0;
  int diff_R, diff_G, diff_B, tot_err;
  int tot_diff_R=0;
  int tot_diff_B=0;
  int tot_diff_G=0;
  int tot_diff_H=0;
  int tot_diff_C=0;

  uint25 cdf[GRAY_LEVELS]  = {0};
  uint19 hist[GRAY_LEVELS] = {0};

  uint19 ref_hist[GRAY_LEVELS] = {
		  0
//		#include "histogram.h"
};

  uint25 ref_cdf[GRAY_LEVELS] = {
		  0
//		#include "cdf.h"
  };

  tempbuf  = (char *) malloc(MAX_PATH * sizeof(char));
  tempbuf1 = (char *) malloc(MAX_PATH * sizeof(char));
   
  // Arrays to store image data
  uint8 *R, *G, *B;
  
  //Arrays to send and receive data from the accelerator
  RGB_t *in_pix;
  RGB_t *out_pix;
  RGB_t *ref_pix;

  R = (uint8 *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(unsigned char));
  G = (uint8 *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(unsigned char));
  B = (uint8 *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(unsigned char));

   in_pix = (RGB_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(RGB_t));
  out_pix = (RGB_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(RGB_t));
  ref_pix = (RGB_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(RGB_t));

  //Get image data from disk
  sprintf(tempbuf,  "%s.bmp", INPUT_IMAGE_BASE);
  // Fill a frame with data
  int read_tmp = BMP_Read(tempbuf, MAX_HEIGHT, MAX_WIDTH, R, G, B);
  if(read_tmp != 0) {
    printf("%s Loading image failed\n", tempbuf);
    exit (1);
  }
  printf("Loaded image file %s of size %4d %4d\n", tempbuf, MAX_HEIGHT, MAX_WIDTH);

  // Copy Input Image to pixel data structure
  // Hardware accelerator works on video pixel streams
   for(y = 0; y < MAX_HEIGHT; y++){
      for(x = 0; x < MAX_WIDTH; x++){
      in_pix[y*MAX_WIDTH + x].R = G[y*MAX_WIDTH + x];
      in_pix[y*MAX_WIDTH + x].G = G[y*MAX_WIDTH + x];
      in_pix[y*MAX_WIDTH + x].B = G[y*MAX_WIDTH + x];
      }
   }

#ifdef __SYNTHESIS__
   width  = 1920;
   height = 1080;
#else
   width  = 1920;
   height = 1080;
#endif


   printf("Running the Design Under Test on image size of W=%4d H=%4d\n", width, height);
   wrapper_img_hist_equaliz1( width, height,     cdf,     hist, in_pix, out_pix);
       ref_histogram_equaliz3( width, height, ref_cdf, ref_hist, in_pix, ref_pix);
  

  // Copy Output video stream to Image data structure
  for(y =0; y < height; y++){
    for(x = 0; x < width; x++){

      R[y*MAX_WIDTH + x] = out_pix[y*MAX_WIDTH + x].G;
      G[y*MAX_WIDTH + x] = out_pix[y*MAX_WIDTH + x].G;
      B[y*MAX_WIDTH + x] = out_pix[y*MAX_WIDTH + x].G;
    }
  }
  //Write the output image back to disk
  sprintf(tempbuf1, "%s.bmp", OUTPUT_IMAGE_BASE);
  int write_tmp = BMP_Write(tempbuf1, MAX_HEIGHT, MAX_WIDTH, R, G, B);
  if(write_tmp != 0){ 
    printf("WriteBMP %s failed\n", tempbuf1);
    exit(1);
  }
 

  // Copy reference video stream to  Image data structure
  for(y =0; y < height; y++){
    for(x = 0; x < width; x++){

      R[y*MAX_WIDTH + x] = ref_pix[y*MAX_WIDTH + x].G;
      G[y*MAX_WIDTH + x] = ref_pix[y*MAX_WIDTH + x].G;
      B[y*MAX_WIDTH + x] = ref_pix[y*MAX_WIDTH + x].G;
    }
  }
  //Write the reference image back to disk
  sprintf(tempbuf1, "%s.bmp", REFER_IMAGE_BASE);
  write_tmp = BMP_Write(tempbuf1, MAX_HEIGHT, MAX_WIDTH, R, G, B);
  if(write_tmp != 0){
    printf("WriteBMP %s failed\n", tempbuf1);
    exit(1);
  }

  check_results = 0;
  for (y = 0;   y < height; y++)
  {
    for (x = 0; x < width; x++)
	{
 	  diff_R = out_pix[y*MAX_WIDTH + x].R - ref_pix[y*MAX_WIDTH + x].R;
	  diff_R = ABS(diff_R);
	  tot_diff_R = + diff_R;
 	  diff_G = out_pix[y*MAX_WIDTH + x].G - ref_pix[y*MAX_WIDTH + x].G;
	  diff_G = ABS(diff_G);
	  tot_diff_G = + diff_G;
 	  diff_B = out_pix[y*MAX_WIDTH + x].B - ref_pix[y*MAX_WIDTH + x].B;
	  diff_B = ABS(diff_B);
	  tot_diff_B = + diff_B;
    }
  }
  tot_err = tot_diff_R + tot_diff_G + tot_diff_B;

  printf("\ntotal error in images = %d\n", tot_err);
  if (tot_err<200) tot_err = 0;

  diff_G = 0;
  for (y = 0; y < GRAY_LEVELS-1; y++)
	{
	    diff_R = cdf[y] - ref_cdf[y];
		diff_R = ABS(diff_R);
		diff_G = diff_G + diff_R;
		//printf("got %7d expected %7d\n", cdf[y], ref_cdf[y]);
  }
  printf("CDF  tot diff= %d\n\n", diff_G);
  tot_err += diff_G;

  diff_G = 0;
  for (y = 0; y < GRAY_LEVELS-1; y++)
	{
	    diff_R = (int) hist[y] - (int) ref_hist[y];
		diff_R = ABS(diff_R);
		diff_G = diff_G + diff_R;
		//printf("got %7d expected %7d\n", hist[y], ref_hist[y]);
  }
  printf("HIST tot diff= %d\n\n", diff_G);






  free(R);
  free(G);
  free(B);
  free(tempbuf);
  free(tempbuf1);
  free(in_pix);
  free(out_pix);
  free(ref_pix);

 
  if (tot_err > 0)
  {
    printf("TEST FAILED!: error = %d\n", check_results);
    check_results = 1;
  }
  else 
  {
    printf("TEST SUCCESSFUL!\n");
	check_results = 0;
  }

  return check_results;

}
