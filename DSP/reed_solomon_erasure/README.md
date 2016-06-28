
Reed-Solomon Erasure Codec Design Using Vivado High-Level Synthesis
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

This application note focuses on the design of an erasure codec using the Xilinx® Vivado High-Level Synthesis (HLS) tool, which takes the source code in C programming language and generates highly efficient synthesizable Verilog or VHDL code for a Kintex® UltraScale™ FPGA.
When there is a need to change erasure code parameters, for example, the generator matrix definition or erasure code rate, only slight modification to the C header files is needed. The example Reed-Solomon erasure codec has a generic architecture from which other types of erasure codecs can be obtained by modifying the C source code.
    
[Full Documentation]

## 2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS

* Xilinx Vivado 2015.1 Design Suite with High-Level Synthesis

## 3. DESIGN FILE HIERARCHY
```
	|   CONTRIBUTING.md
	|   LICENSE.md
	|   README.md
	|   run.tcl
	|   
	+---src							: This folder contains C design files and header files.
	|       rs_erasure.c
	|       rs_erasure.h
	|       rs_roms.c
	|       
	+---tb							: This folder contains a C design file that serves as the test bench. 
	|       tb_rs_erasure.c
	|       
	\---tv							:  This folder contains the input and output golden test vectors for verification purposes.
			tv_rs_erasure_in.txt
			tv_rs_erasure_mout.txt
```

## 4. INSTALLATION AND OPERATING INSTRUCTIONS

1. Install the Xilinx Vivado tools, version 2015.3 or later.
2. Unzip the design files into a clean directory.
3. In the Vivado HLS command line window:
	a. `cd` to the root of the design directory.
	b. Type `vivado_hls run.tcl`
	c. Check that the synthesized design meets expectations.

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
This project is written by developers at [Xilinx](http://www.xilinx.com/) with other contributors listed below:

## 10. REVISION HISTORY

Date		|	Readme Version		|	Revision Description
------------|-----------------------|-------------------------
MAR2016		|	1.0					|	Initial Xilinx release




[Contributing]: CONTRIBUTING.md 
[3-Clause BSD License]: LICENSE.md
[Full Documentation]: http://www.xilinx.com/support/documentation/application_notes/xapp1273-reed-solomon-erasure.pdf
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 
[Vivado HLS User Guide]: http://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf