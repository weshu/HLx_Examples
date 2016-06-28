############################################################
## Histogram Equalization from src3 code

## Copyright (C) 2015 Xilinx Inc. All rights reserved.

## daniele.bagni@xilinx.com

## 22 July 2015
############################################################


open_project hls_hist_src3_prj
set_top top_img_hist_equaliz1
add_files src3/img_hist1.cpp
add_files -tb src3/ap_bmp.cpp
add_files -tb src3/main_test.cpp
add_files -tb src3/ref_img_hist.cpp
add_files -tb src3/test_data

################## SOLUTION1: Baseline ####################
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 160MHz -name default

csim_design -ldflags {-Wl,--stack=268435456} -clean 
# ONLY FOR WINDOWS
#csim_design -clean #ONLY FOR LINUX
csynth_design
#cosim_design -ldflags {-Wl,--stack=268435456} # ONLY FOR WINDOWS
#cosim_design 
#export_design -format ip_catalog


################## SOLUTION2: DATAFLOW ####################
open_solution "solution2"
set_part {xc7z020clg484-1}
create_clock -period 160MHz -name default
set_directive_dataflow "top_img_hist_equaliz1"
#csim_design -ldflags {-Wl,--stack=268435456} -clean 
# ONLY FOR WINDOWS
#csim_design -clean 
csynth_design
#cosim_design -ldflags {-Wl,--stack=268435456} # ONLY FOR WINDOWS
#cosim_design 
#export_design -format ip_catalog

################## SOLUTION3: DATAFLOW + CONFIG_DATAFLOW ####################
open_solution "solution3"
set_part {xc7z020clg484-1}
create_clock -period 160MHz -name default
config_dataflow -default_channel fifo -fifo_depth 2
set_directive_dataflow "top_img_hist_equaliz1"
#csim_design -ldflags {-Wl,--stack=268435456} -clean # ONLY FOR WINDOWS
#csim_design -clean #ONLY FOR LINUX
csynth_design
#cosim_design -ldflags {-Wl,--stack=268435456} # ONLY FOR WINDOWS
#cosim_design 
#export_design -format ip_catalog

################## SOLUTION4: as Solution3 + PIPELINE  all the innermost loops ####################
open_solution "solution4"
set_part {xc7z020clg484-1}
create_clock -period 160MHz -name default
config_dataflow -default_channel fifo -fifo_depth 2
set_directive_dataflow "top_img_hist_equaliz1"
set_directive_pipeline "compute_cdf/CDF_L2"
set_directive_pipeline "yuv2rgb/Lx"
set_directive_pipeline "rgb2yuv/Lx"
set_directive_pipeline "img_hist_equaliz1/Lx"
#csim_design -ldflags {-Wl,--stack=268435456} -clean # ONLY FOR WINDOWS
#csim_design -clean #ONLY FOR LINUX
csynth_design
#cosim_design -ldflags {-Wl,--stack=268435456} # ONLY FOR WINDOWS
#cosim_design 
#export_design -format ip_catalog

################## SOLUTION5: as Solution4 + DEPENDENCE FALSE  ####################
open_solution "solution5"
set_part {xc7z020clg484-1}
create_clock -period 160MHz -name default
config_dataflow -default_channel fifo -fifo_depth 2
set_directive_dataflow "top_img_hist_equaliz1"
set_directive_pipeline "compute_cdf/CDF_L2"
set_directive_pipeline "yuv2rgb/Lx"
set_directive_pipeline "rgb2yuv/Lx"
set_directive_pipeline "img_hist_equaliz1/Lx"
set_directive_pipeline "update_histogram/L0"
set_directive_dependence -variable hist -type intra -dependent false "img_hist_equaliz1"
#csim_design -ldflags {-Wl,--stack=268435456} -clean # ONLY FOR WINDOWS
#csim_design -clean #ONLY FOR LINUX
csynth_design
#cosim_design -ldflags {-Wl,--stack=268435456} # ONLY FOR WINDOWS
#cosim_design 
#export_design -format ip_catalog

