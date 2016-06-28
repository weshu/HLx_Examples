
Digital Up Converter, FIR Filter
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

Digital Up Converters (DUC) and Digital Down Converters (DDC) are key components of RF systems in communications, sensing, and imaging. In terms of signal processing, narrowband systems are generally characterized by the fact that the bandwidth of the signal of interest is significantly less than the sampled bandwidth; that is, a narrow band of frequencies must be selected and filtered out from a much wider spectral window in which the signal might occur. This means that large sample rate changes (in the hundreds or even thousands) must be undertaken to efficiently process the signal for either transmission or reception. Examples of such systems include: communications systems, where several narrow channels must be recovered from a wider transmission band, or medical imaging systems, such as MRI, where the detected waveforms occur in a narrow range of frequencies at varying points within a wider spectrum, as well as many other systems that fall
within this grouping.
They are often used in conjunction with small Finite Impulse Response (FIR) filters to implement the filter chain of up- and down-converters in narrowband systems. While such converters have been implemented often for many years in both Application Specific Standard Products (ASSPs) and FPGAs, these have generally provided only single-channel solutions, or multiple instances thereof. The latest high-density FPGA families with advanced architectures, in combination with efficient IP cores and effective design tools, provide the capability to handle many channels simultaneously. This application note demonstrates how to implement such designs.

[Full Documentation]

## 2. SOFTWARE TOOLS AND SYSTEM REQUIREMENTS

Any Vivado HLS release from 2014.1 to 2016.1


## 3. DESIGN FILE HIERARCHY
```
    b1_fp2.inc
    b2_fp2.inc
    b3_fp2.inc
    CONTRIBUTING.md
    dds.cpp
    dds.h
    dds_test.cpp
    duc_2ch.cpp
    duc_2ch.h
    duc_2ch_test.cpp
    fir.h
    fir_hls_v7.pdf
    LICENSE.md
    README.md
    run.tcl
    srrc_0db.inc
    test_duc.slx
```

## 4. INSTALLATION AND OPERATING INSTRUCTIONS

The procedure to build the HLS project is as follows:

TCL file to run HLS tool:
```
	vivado_hls run.tcl
```
`duc_2ch.cpp`: contains the top level module called duc_2ch, which includes the following modules:
* `duc_2ch_class` (calling) -> duc_class -> various FIR classes
* `dds_frame` : generates an array of sine/cosine output. It calls dds module
* `mixer` : complex multiply duc_2ch output with dds_frame output 

## 5. OTHER INFORMATION

For more information check here: [Full Documentation][]
[Digital Down Converter][]
[Vivado HLS User Guide][]

## 6. SUPPORT

For questions and to get help on this project, or your own projects, visit the [Vivado HLS Forums][]. 

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
[Full Documentation]: fir_hls_v7.pdf
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 
[Digital Down Converter]: http://www.xilinx.com/support/documentation/application_notes/xapp1113.pdf
[Vivado HLS User Guide]: http://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf