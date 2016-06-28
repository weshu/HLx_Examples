############################################################
## Z = SQRT( X^2 + Y^2)  

## Copyright (C) 2015 Xilinx Inc. All rights reserved.

## daniele.bagni@xilinx.com

## 22 July 2015
############################################################

#// to enable TOTALLY floating point sqrt reference function for SOLUTION 0
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_DOUBLE"
add_files float_sqrt.cpp    -cflags "-DSQRT_DOUBLE"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_DOUBLE"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_DOUBLE"
add_files -tb test_main.cpp -cflags "-DSQRT_DOUBLE"
open_solution "solution0"
set_part {xc7z020clg484-1}
create_clock -period 5 -name default
#source "./hls_isqrt_prj/solution0/directives.tcl"
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
#export_design -format ip_catalog
close_project

#// to enable TOTALLY floating point sqrt reference function for SOLUTION 1
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_REF"
add_files float_sqrt.cpp    -cflags "-DSQRT_REF"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_REF"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_REF"
add_files -tb test_main.cpp -cflags "-DSQRT_REF"
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 5 -name default
#source "./hls_isqrt_prj/solution0/directives.tcl"
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
#export_design -format ip_catalog
close_project


#// to enable floating point sqrt function while the rest is integer for SOLUTION 
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_FLOAT"
add_files float_sqrt.cpp    -cflags "-DSQRT_FLOAT"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_FLOAT"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_FLOAT"
add_files -tb test_main.cpp -cflags "-DSQRT_FLOAT"
open_solution "solution2"
set_part {xc7z020clg484-1}
create_clock -period 5 -name default
#source "./hls_isqrt_prj/solution0/directives.tcl"
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
#export_design -format ip_catalog
close_project

#//to enable full version of Cordic sqrt function for SOLUTIONS 3 and 4
open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_CORDIC"
add_files float_sqrt.cpp    -cflags "-DSQRT_CORDIC"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_CORDIC"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_CORDIC"
add_files -tb test_main.cpp -cflags "-DSQRT_CORDIC"
open_solution "solution3"
set_part {xc7z020clg484-1}
create_clock -period 5 -name default
#source "./hls_isqrt_prj/solution0/directives.tcl"
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
#export_design -format ip_catalog
close_project

open_project hls_isqrt_prj
set_top top_process_magnitude
add_files top_magnitude.cpp -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files float_sqrt.cpp    -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files cordic_sqrt.cpp   -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files cordic_isqrt.cpp  -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
add_files -tb test_main.cpp -cflags "-DSQRT_CORDIC -DSQRT_AP_INT"
open_solution "solution4"
set_part {xc7z020clg484-1}
create_clock -period 5 -name default
#source "./hls_isqrt_prj/solution0/directives.tcl"
set_directive_inline -off "func2_process_magnitude"
set_directive_inline -off "func1_process_magnitude"
set_directive_inline -off "func3_process_magnitude"
set_directive_inline -off "func4_process_magnitude"
set_directive_pipeline "func1_process_magnitude/L1"
set_directive_pipeline "func2_process_magnitude/L2"
set_directive_pipeline "func3_process_magnitude/L3"
set_directive_pipeline "func4_process_magnitude/L4"
csim_design -ldflags {{-Wl,--stack=268435456}} -clean
csynth_design
#cosim_design -ldflags {{-Wl,--stack=268435456}} -rtl systemc
#export_design -format ip_catalog
close_project