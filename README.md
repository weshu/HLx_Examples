# HLx_Examples
Open Source HLx Examples

## Acceleration 
- **memcached** - HLS implementation of Memcached pipeline.
  This HLS example gives the pipelined memcached implementation. The main pipeline stages of memcached include request parser, hash table, value store and response formatter. Both HLS and RTL testbenches are provided to allow users to simulate the design in HLS or modelsim.
  
## DSP 
- **digital_up_conv** - Digital Up Converter, FIR Filter.
  Digital Up Converters (DUC) and Digital Down Converters (DDC) are key components of RF systems in communications, sensing, and imaging.

- **direct_digital_syn** - Direct Digital Synthesizer (DDS) using Vivado HLS.
  This example shows the implementation of a DDS using Vivado HLS tool. The initial target device is K7 -1, but the design, described in C++, can be targeted to most of Xilinx FPGAs. 

- **fir_example** - FIR Filtering.
  Optimization of a FIR Operation.

- **fp_pid_contr** - Floating-Point PID Controller Design with Vivado HLS and System Generator for DSP
  Floating-point algorithms are widely used in industries from analysis to control applications. Traditionally, such algorithms have been implemented on microprocessors.
  
- **multi_chan_conv_fil** - Multi-channel Fractional Sample Rate Conversion Filter Design Using Vivado High-Level Synthesis
  This project focuses on the design of a multi-channel fractional sample rate conversion (SRC) filter using the Vivado High-Level Synthesis (HLS) tool, which takes the source code in C\++ programming language and generates highly efficient synthesizable Verilog or VHDL code for FPGA.

- **phase_rec_loop** - Implementing Carrier Phase Recovery Loop Using Vivado HLS 
  This application note describes the implementation of a carrier phase recovery loop algorithm for a single carrier QAM demodulator using Vivado High-Level Synthesis (HLS).
  
- **reed_solomon_erasure** - Reed-Solomon Erasure Codec Design Using Vivado High-Level Synthesis
  This application note focuses on the design of an erasure codec using the Xilinx® Vivado High-Level Synthesis (HLS) tool, which takes the source code in C programming language and generates highly efficient synthesizable Verilog or VHDL code for a Kintex® UltraScale™ FPGA.
  
- **ssr_fir** - Super Sample Rate FIR (data rate > clock rate) Implementation using Vivado HLS
  This project describes the implementation of the super sample rate (SSR) filter – where the sample data rate is greater than the clock rate using Vivado HLS tool.

## Math 
- **atan2_cordic** - Linear Algebra Library: Atan2() Example. 
Implementing the math function atan2 from the Linear Algebra Library 

- **fp_accum** - Floating Point Accumluation Operation and HLS Optimizations
Optimization of a Floating Point Accumluation Operation.

- **sqrt_cordic** - Linear Algebra Library: Sqrt() Example.
Implementing the math function sqrt from the Linear Algebra Library 

- **squared_difference_accumualate** - Squared Difference Accumulate Using Vivado High-Level Synthesis
This simple example shows how to use Vivado HLS to code a "Squared Difference Accumulate" function and ensure the new squaring MUX feature within the UltraScale DSP48E2

## Vision
- **img_histEq** - Image Histogram Equalization and HLS Optimizations.
  
- **mpeg_forward** - MPEG2 forward DCT 8x8  with Vivado HLS.
The DCT concentrates most of the pixels energy distribution into a few frequency coefficients. 
DCT is a valuable tool for pictures compression, when associated with Quantization and VLC.

- **video_edge** - Video Processing Design Using HLS.
Driver Assist Video Processing example using Vivado HLS and OpenCV Blocks.  

## OTHER INFORMATION

For more information check here: 

[Vivado HLS User Guide][]

## SUPPORT

For questions and to get help on this project or your own projects, visit the [Vivado HLS Forums][]. 

## License
The source for this project is licensed under the [3-Clause BSD License][]

## Contributing code
Please refer to and read the [Contributing][] document for guidelines on how to contribute code to this open source project. The code in the `/master` branch is considered to be stable, and all pull-requests should be made against the `/develop` branch.

[Contributing]: CONTRIBUTING.md 
[3-Clause BSD License]: LICENSE.md
[Vivado HLS Forums]: https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/bd-p/hls 
[Vivado HLS User Guide]: http://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug902-vivado-high-level-synthesis.pdf