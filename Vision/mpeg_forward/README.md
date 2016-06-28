
MPEG2 forward DCT 8x8  with Vivado HLS
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

The DCT concentrates most of the pixels energy distribution into a few frequency coefficients. 
DCT is a valuable tool for pictures compression, when associated with Quantization and VLC. The transform itself does not compress images and it is accurately reversible. DCT is a separable transform: 2-D F-DCT and I-DCT can be done as consecutive 1-D transforms
    
[Full Documentation]

## 2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS

* Xilinx Vivado 2015.1 Design Suite with High-Level Synthesis

## 3. DESIGN FILE HIERARCHY
```
	|   CONTRIBUTING.md
	|   LICENSE.md
	|   README.md
	|   wang_dct_db_v2.pdf
	|   
	\---wang_dct
			dct.h
			dct_coeff_table.h
			fdctref.cpp
			my_dct.cpp
			run_hls_dct_script.tcl
			test_wang_fdct.cpp
			wang_fdct.cpp
```

## 4. INSTALLATION AND OPERATING INSTRUCTIONS

1. In the Vivado HLS command line window:

```
	vivado_hls run_hls_dct_script.tcl
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
MPEG Software Simulation Group 


## 10. REVISION HISTORY

Date		|	Readme Version		|	Revision Description
------------|-----------------------|-------------------------
MAR2016		|	1.0					|	Initial Xilinx release




[Contributing]: CONTRIBUTING.md 
[3-Clause BSD License]: LICENSE.md
[Full Documentation]: wang_dct_db_v2.pdf
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 
[Vivado HLS User Guide]: http://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf