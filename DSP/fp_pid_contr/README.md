

Floating-Point PID Controller Design with Vivado HLS and System Generator for DSP
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

Floating-point algorithms are widely used in industries from analysis to control applications. Traditionally, such algorithms have been implemented on microprocessors. The primary reason for using microprocessors has been the ease with which floating-point algorithms can be captured, validated, and debugged in C/C++ code, therefore avoiding the complexity and skills required to implement them in hardware. However, implementing these algorithms on optimized and dedicated hardware provides lower cost, higher performance, and power benefits over a standard, or even optimized microprocessor.

This application note presents a new design flow enabled by the Xilinx Vivado Design Suite, which allows floating-point algorithms to be quickly specified in C/C++ code, optimized for performance, and implemented on Xilinx FPGA devices. This flow delivers on the cost, performance, and power benefits that have so far eluded designers who rely on traditional microprocessors for implementing floating-point algorithms.

Starting from the standard industrial application of a Proportional-Integral-Derivative (PID) control, as reported in "Vivado HLS Eases Design of Floating-Point PID Controller", we will review and explain the following aspects of implementing floating-point algorithms in an FPGA:
1. Explain benefits and opportunities created by implementing floating-point designs in an
FPGA device.
2. Review an industry standard application, showing how the algorithm can be implemented
in C/C++ code.
3. Show how the C/C++ design can transformed into a high-performance hardware
implemented using Vivado high-level synthesis (HLS) and how the design can be further
optimized.

The Proportional-Integral-Derivative (PID) systems are used throughout industries, from
factory control systems to car braking systems, robot controls, and medical devices. The
implementation of a PID core very often requires the accuracy provided by floating-point
calculations.

[Full Documentation][]

## 2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS

Any Vivado HLS release from 2014.1 to 2015.4.


## 3. DESIGN FILE HIERARCHY
```
    |   CONTRIBUTING.md
    |   hls_script_xapp1163_pid.tcl
    |   LICENSE.md
    |   LICENSE.txt
    |   readme.md
    |   
    \---C
        |   test_xapp_pid.cpp
        |   xapp_pid.cpp
        |   xapp_pid.h
        |   
        \---test_data
                d_ref.txt
                e_ref.txt
                i_ref.txt
                u_ref.txt
                yp_ref.txt
```

## 4. INSTALLATION AND OPERATING INSTRUCTIONS 

The procedure to build the HLS project is as follows:

1. Launch the TCL script `hls_script_xapp1163_pid.tcl` from the Vivado HLS 2015.4 Command Prompt
2. Results of the 32-bit floating point design in the folder `hls_pid_prj/solution_float`
3. Results of the 64-bit floating point design in the folder `hls_pid_prj/solution_double`
4. Results of the 25-bit fractional fixed point design in the folder `hls_pid_prj/solution_fixed`

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
13JAN2013	|	1.0					|	Initial Xilinx release.
23MAR2016	|	1.1					|	Re-written for GIT HUB



[Contributing]: CONTRIBUTING.md 
[3-Clause BSD License]: LICENSE.md
[Vivado HLS User Guide]: http://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf
[Full Documentation]: http://www.xilinx.com/support/documentation/application_notes/xapp1163.pdf
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 