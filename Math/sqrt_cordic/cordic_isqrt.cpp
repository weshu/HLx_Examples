/************************************************
Copyright (c) 2016, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software 
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************/

#include "cordic_defines.h"



#define ITER    16


#ifdef SQRT_INT

unsigned short int cordic_isqrt (unsigned long int x)
{

#pragma	HLS INLINE
	unsigned char i;
	unsigned short int y;
	unsigned long int m;
	unsigned short int base = (1<<(ITER-1))&0xFFFF;
  
       y = 0 ;
 L0:      for (i = 1; i <= ITER; i++)
       {
#pragma	HLS PIPELINE
               y +=  base ;
               m = (unsigned long int) y * (unsigned long int) y;
               if  (m > x )
               {
                       y -= base ;  // base should not have been added, so we substract again
               }
               base = base >> 1 ;      // shift 1 digit to the right = divide by 2
        }
        return y ;
}



dout_t process_magnitude_int(dinp_t real_data, dinp_t imag_data) 
{
#pragma	HLS INLINE OFF
	typedef signed long int  acc_t;
	dinp_t mag_data;
	acc_t accu_plus, temp_datar, temp_datai;

	acc_t temp_long;

	temp_datar = (acc_t)real_data*(acc_t)real_data;
	temp_datai = (acc_t)imag_data*(acc_t)imag_data;
	accu_plus = temp_datar + temp_datai;

	mag_data = cordic_isqrt((unsigned long int) accu_plus); 

	return mag_data;
}

#endif
