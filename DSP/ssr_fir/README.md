
Super Sample Rate FIR (data rate > clock rate) Implementation using Vivado HLS
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

This project describes the implementation of the super sample rate (SSR) filter – where the sample data rate is greater than the clock rate using Vivado HLS tool.

The goal specification for this filter:
* The effective data rate = 1.6 Gsps (= 4 * clock rate) 
* 128 tap symmetric coefficients
* Clock rate = 400 MHz

## 2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS

Any Vivado HLS release from 2014.1 to 2016.1


## 3. DESIGN FILE HIERARCHY
```
    coef128.inc						
    comp.do
    CONTRIBUTING.md
    fir_super.cpp					: Design source code
    fir_super.h						: Design source code
    fir_super_test.cpp				: Testbench code
    LICENSE.md
    README.md
    run.tcl							
    ssr_fir_hls_v4.pdf				: Full Documentation  
```

## 4. INSTALLATION AND OPERATING INSTRUCTIONS

The procedure to build the HLS project is as follows:

TCL file to run HLS tool:
```
vivado_hls run.tcl
```
## 5. OTHER INFORMATION

For more information check here: [Full Documentation][]

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
[Full Documentation]: ssr_fir_hls_v4.pdf
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 