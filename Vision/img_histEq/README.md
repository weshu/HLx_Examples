
Image Histogram Equalization and HLS Optimizations
======================================

This readme file contains these sections:

1. OVERVIEW
2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS
3. DESIGN FILE HIERARCHY
4. INSTALLATION AND OPERATING INSTRUCTIONS
5. OTHER INFORMATION (OPTIONAL)
6. SUPPORT
7. LICENSE
8. CONTRIBUTING
9. Acknowledgements
10. REVISION HISTORY

## 1. OVERVIEW

Step after step how the various directives affect performance
1. To help the compiler optimize an unbounded loop
	`Set_directive_loop_tripcount -min 640 -max 1920 func_name/Lx`
	`set_directive_loop_tripcount -min 480 -max 1080 func_name/Ly`

2. To make the 3 subroutines concurrent (improves latency and throughput)
	`set_directive_dataflow top_func_name`

3. To change the default interface of dataflow from ping-pong RAM to FIFO (all in all video data are streamed) buffering
	`config_dataflow -default_channel fifo -fifo_depth 2`

4. To pipeline the various routines (improves latency and throughput)
	`set_directive_pipeline`
    
5. To improve throughput in an array mapped as True dual-Port BRAM
	`set_directive_dependence false`
	`set_directive_resource -core RAM_T2P_BRAM array_name`
    
[Full Documentation]

## 2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS

Any Vivado HLS release from 2014.1 to 2016.1

## 3. DESIGN FILE HIERARCHY
```
    |   5_ImgHistEqualiz.pdf
    |   CONTRIBUTING.md
    |   LICENSE.md
    |   README.md
    |   run_src3_hsl_script.tcl
    |   run_src4_hsl_script.tcl
    |   
    +---src3
    |   |   ap_bmp.cpp
    |   |   ap_bmp.h
    |   |   cdf.h
    |   |   histogram.h
    |   |   img_hist1.cpp
    |   |   main_test.cpp
    |   |   median.h
    |   |   my_defines.h
    |   |   ref_img_hist.cpp
    |   |   
    |   \---test_data
    |           ref_cdf.txt
    |           ref_hist.txt
    |           test_1080p.bmp
    |           test_results.m
    |           
    \---src4
        |   ap_bmp.cpp
        |   ap_bmp.h
        |   cdf.h
        |   histogram.h
        |   img_hist1.cpp
        |   main_test.cpp
        |   my_defines.h
        |   ref_img_hist.cpp
        |   
        \---test_data
                ref_cdf.txt
                ref_hist.txt
                test_1080p.bmp
                test_results.m
```

## 4. INSTALLATION AND OPERATING INSTRUCTIONS

The procedure to build the HLS project is as follows:

Two versions of the project. 
Version 1: 
```
	vivado_hls run_src3_hsl_script.tcl
```
Version 2:
```
	vivado_hls run_src4_hsl_script.tcl
```

## 5. OTHER INFORMATION

For more information check here: 
[Full Documentation][]
[Vivado HLS User Guide][]

## 6. SUPPORT

For questions and to get help on this project or your own projects, visit the [Vivado HLS Forums][]. 

## 7. License
The source for this project is licensed under the [3-Clause BSD License][]

## 8. Contributing code
Please refer to and read the [Contributing][] document for guidelines on how to contribute code to this open source project. The code in the `/master` branch is considered to be stable, and all pull-requests should be made against the `/develop` branch.

## 9. Acknowledgements
The Library is written by developers at [Xilinx](http://www.xilinx.com/) with other contributors listed below:

## 10. REVISION HISTORY

Date		|	Readme Version		|	Revision Description
------------|-----------------------|-------------------------
JAN2014		|	1.0					|	Initial Xilinx release
24MAR2016	|	1.1					|	Verified for 2016.1



[Contributing]: CONTRIBUTING.md 
[3-Clause BSD License]: LICENSE.md
[Full Documentation]: 5_ImgHistEqualiz.pdf 
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 
[Vivado HLS User Guide]: http://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf