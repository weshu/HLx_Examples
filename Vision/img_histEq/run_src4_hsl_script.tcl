############################################################
## Histogram Equalization from src4 code

## Copyright (C) 2015 Xilinx Inc. All rights reserved.

## daniele.bagni@xilinx.com

## 22 July 2015
############################################################


open_project hls_hist_src4_prj
set_top top_img_hist_equaliz1
add_files src4/img_hist1.cpp
add_files -tb src4/test_data
add_files -tb src4/ref_img_hist.cpp
add_files -tb src4/main_test.cpp
add_files -tb src4/ap_bmp.cpp

################## SOLUTION1: Baseline ####################

open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 160MHz -name default
csim_design -ldflags {-Wl,--stack=268435456} -clean 
# ONLY FOR WINDOWS
#csim_design -clean # ONLY FOR LINUX
csynth_design
#cosim_design -ldflags {-Wl,--stack=268435456} # ONLY FOR WINDOWS
#cosim_design 
#export_design -format ip_catalog


################## SOLUTION2: DATAFLOW ####################
open_solution "solution2"
set_part {xc7z020clg484-1}
create_clock -period 160MHz -name default
set_directive_dependence -variable hist -type inter -dependent false "top_img_hist_equaliz1"
#csim_design -ldflags {-Wl,--stack=268435456} -clean 
# ONLY FOR WINDOWS
#csim_design -clean #ONLY FOR LINUX
csynth_design
#cosim_design -ldflags {-Wl,--stack=268435456} # ONLY FOR WINDOWS
#cosim_design 
#export_design -format ip_catalog

