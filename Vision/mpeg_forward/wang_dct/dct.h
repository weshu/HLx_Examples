#ifndef __DCT_H__
#define __DCT_H__

//#define DB_ORIGINAL

#define DW 16
#define N 1024/DW
#define NUM_TRANS 16

typedef signed short int dct_data_t;

#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)

#define DB_ABS(x) ( (x) < 0 ?  -(x) : (x) )




#ifdef BIT_ACCURATE

#include "ap_int.h"

typedef ap_uint<1>  uint1_t;
typedef ap_int<4>  int4_t;
typedef ap_int<11> int11_t;
typedef ap_int<12> int12_t;
typedef ap_int<13> int13_t;
typedef ap_int<14> int14_t;
typedef ap_int<16> int16_t;

typedef ap_int<17> int17_t;
typedef ap_int<18> int18_t;
typedef ap_int<19> int19_t;
typedef ap_int<20> int20_t;
typedef ap_int<25> int25_t;
typedef ap_int<26> int26_t;
typedef ap_int<32> int32_t;

typedef ap_int<33> int33_t;
typedef ap_int<35> int35_t;
typedef ap_int<48> int48_t;

#else 

typedef char      uint1_t;
typedef char      int4_t;
typedef short int int11_t;
typedef short int int12_t;
typedef short int int13_t;
typedef short int int14_t;
typedef short int int16_t;

typedef int      int17_t;
typedef int      int18_t;
typedef int      int19_t;
typedef int      int20_t;
typedef int      int25_t;
typedef int      int26_t;
typedef int      int32_t;

typedef long long int int33_t;
typedef long long int int35_t;
typedef long long int int48_t;

#endif // BIT_ACCURATE


extern void init_fdct();
extern void init_idct();
extern void ref_fdct(short *inp_block, short *out_block);
extern void wang_fdct(short int xx[N], short int yy[N]);
extern void my_dct(short int xx[N], short int yy[N]);
extern void top_fdct(short int xx[N], short int yy[N]);
extern void wang_idct(short int *inp_block, short int *out_block);

#endif // __DCT_H__ not defined
