############################################################
## Forward WANG's DCT of an 8x8 block   

## Copyright (C) 2015 Xilinx Inc. All rights reserved.

## daniele.bagni@xilinx.com

## 22 July 2015
############################################################

open_project hls_wangdct_prj
set_top top_fdct
add_files wang_fdct.cpp
add_files my_dct.cpp
add_files -tb test_wang_fdct.cpp
add_files -tb fdctref.cpp

open_solution "solution_wang"
set_part {xc7k325tffg900-2}
create_clock -period 4 -name default
csim_design -clean
csynth_design
cosim_design
export_design -evaluate verilog -format ip_catalog
close_project


open_project hls_wangdct_prj
set_top top_fdct
add_files wang_fdct.cpp -cflags "-DDB_ORIGINAL"
add_files my_dct.cpp  -cflags "-DDB_ORIGINAL"
add_files -tb test_wang_fdct.cpp  -cflags "-DDB_ORIGINAL"
add_files -tb fdctref.cpp  -cflags "-DDB_ORIGINAL"
open_solution "solution_tutorial"
set_part {xc7k325tffg900-2}
create_clock -period 4 -name default
#source "./hls_wangdct_prj/solution_tutorial/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -evaluate verilog -format ip_catalog
close_project


