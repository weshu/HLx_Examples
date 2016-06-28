/*******************************************************************************
 * File:     my_defines.h
 *

 ******************************************************************************/


#ifndef H_DB_DEFINES_H
#define H_DB_DEFINES_H


/* ************************************************************************* */
/* MAX IMAGE SIZE: */
/* ************************************************************************* */


#define MAX_WIDTH	1920
#define MAX_HEIGHT	1080

#define MAX_PATH 1000

#define GRAY_LEVELS 256

#define CLIP(x) (((x)>255) ? 255 : (((x)<0) ? 0 : (x)))
#define ABS(x)   ((x) < (-(x)) ? (-(x)) : (x))

/* ******************************************************************************* */
// I/O Image Settings
#define INPUT_IMAGE_BASE      "./test_data/test_1080p"
#define OUTPUT_IMAGE_BASE     "./test_data/c_out_1080p"
#define REFER_IMAGE_BASE      "./test_data/golden_out_1080p"


/* ************************************************************************* */
/* COMPILERS DIRECTIVES                                                      */
/* ************************************************************************* */

#ifdef DB_BIT_ACCURATE

#include "ap_int.h"

typedef ap_uint<8>          uint8;
typedef ap_uint<7>          uint7;
typedef ap_uint<1>          uint1;
typedef ap_uint<11>        uint11;
typedef ap_uint<16>        uint16;
typedef ap_uint<19>        uint19;
typedef ap_uint<25>        uint25;
typedef ap_uint<32>        uint32;
typedef ap_int<11>          int11;
typedef ap_int<9>            int9;

#else


typedef unsigned char       uint8;
typedef unsigned char       uint7;
typedef unsigned char       uint1;
typedef unsigned short     uint11;
typedef unsigned short     uint16;
typedef unsigned long int  uint19;
typedef unsigned long int  uint25;
typedef unsigned long int  uint32;
typedef   signed short      int11;
typedef   signed short       int9;

#endif

typedef struct ap_rgb{
    uint8  R;
    uint8  G;
    uint8  B;
  } RGB_t;


  void ref_histogram_equaliz3( uint11 width, uint11 height, uint25 out_cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
  					RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT]);

  void wrapper_img_hist_equaliz1( uint11 width, uint11 height, uint25 out_cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
  					RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT]);




#endif /* H_DB_DEFINES_H */
