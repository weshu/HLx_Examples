
Multi-channel Fractional Sample Rate Conversion Filter Design Using Vivado High-Level Synthesis
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

This project focuses on the design of a multi-channel fractional sample rate conversion (SRC) filter using the Vivado High-Level Synthesis (HLS) tool, which takes the source code in C\++ programming language and generates highly efficient synthesizable Verilog or VHDL code for FPGA. When there is a need to change filter parameters, e.g., the number of channels, the number of filter taps, or sample rate conversion ratios, only simple modification to the C\++ header file is needed. The example SRC filter has a generic architecture from which filters of other types can be easily obtained by modifying the C++ source code.

The design files are coded in C++ programming language that realizes an 8-channel fractional sample rate convresion filter where each channel can have independently configured sample rates and conversion ratios. Both interpolation and decimation are supported by a single filter.

The C++ compilation and synthesis scripts are included to produce verilog code with test bench by Vivado HLS.

The automatically generated verilog code can be synthesized with simple testing code on FPGA to validate the design on hardware. 
    
[Full Documentation]

## 2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS

* Xilinx Vivado 2015.1 Design Suite with High-Level Synthesis

## 3. DESIGN FILE HIERARCHY
```
    |   CONTRIBUTING.md
    |   LICENSE.md
    |   README.md
    |   run.tcl							: Execution Script
    |   
    +---boardtest						: This folder contains Vivado project files for onboard testing.
    |   |   boardtest.tcl
    |   |   
    |   \---src
    |           clk_wiz_0.v
    |           clk_wiz_0_clk_wiz.v
    |           multiSRC_onboard.v
    |           tb_multiSRC_onboard.v
    |           test_bench_onbaord.v
    |           tvin_rom.dat
    |           tvout_rom.dat
    |           zc706_ucf.xdc
    |
    +---src								: This folder contains C++ design files and header files.
    |       filt_coef_rom.h
    |       multiSRC.cpp
    |       multiSRC.h
    |       srcCoef.cpp
    |       srcCtrl.cpp
    |       srcMac.cpp
    |       
    +---tb								: This folder contains a C++ design file that serves as the test bench. 
    |       tb_multiSRC.cpp
    |       
    \---tv								: This folder contains the input and output golden test vectors for verification purpose.
            tv_multiSRC_in.txt
            tv_multiSRC_mout_0.txt
            tv_multiSRC_mout_1.txt
            tv_multiSRC_mout_2.txt
            tv_multiSRC_mout_3.txt
            tv_multiSRC_mout_4.txt
            tv_multiSRC_mout_5.txt
            tv_multiSRC_mout_6.txt
            tv_multiSRC_mout_7.txt
```

## 4. INSTALLATION AND OPERATING INSTRUCTIONS

1. Install the Xilinx Vivado 2015.1 or later tools.
2. Unzip the design files into a clean directory.
3. In Vivado HLS command line window
	a. cd to the root of design directory.
	b. type `vivado_hls run.tcl`
	c. check the synthesized design meet expectation.
4. In Vivado tcl command window
	a. cd to `boardtest` directory.
	b. type `source boardtest.tcl`.
	c. check the implementation result meet expectation.
5. Run the reference design on FPGA (zc706 required to run reference design)
	a. download the design onto ZC706 board.
	b. reset the design by pressing the middle pushbutton on ZC706.
	c. press the right pushbutton to run the test.
	d. The center LED (LED2) should be solid on.
	e. repeat c-d a number of time to confirm the test passes.

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
JUNE2015	|	1.0					|	Initial Xilinx release
24MAR2016	|	1.1					|	Verified for 2016.1



[Contributing]: CONTRIBUTING.md 
[3-Clause BSD License]: LICENSE.md
[Full Documentation]: http://www.xilinx.com/support/documentation/application_notes/xapp1236-multi-channel-src-using-hls.pdf
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 
[Vivado HLS User Guide]: http://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf