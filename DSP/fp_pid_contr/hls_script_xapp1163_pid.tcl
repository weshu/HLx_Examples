############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2014 Xilinx Inc. All rights reserved.
############################################################

open_project hls_pid_prj
set_top PID_Controller
add_files C/xapp_pid.cpp -cflags "-DPID_DEBUG -DPID_DOUBLEPREC"
add_files -tb C/test_data
add_files -tb C/test_xapp_pid.cpp -cflags "-DPID_DEBUG -DPID_DOUBLEPREC"
open_solution "solution_double"
set_part {xc7z010clg400-1}
create_clock -period 8 -name default
config_interface -clock_enable
set_directive_allocation -limit 1 -type core "PID_Controller" fAddSub
set_directive_allocation -limit 1 -type core "PID_Controller" fMul
set_directive_array_partition -type complete -dim 1 "PID_Controller" coeff
set_directive_array_partition -type complete -dim 1 "PID_Controller" din
set_directive_array_partition -type complete -dim 1 "PID_Controller" dout
set_directive_interface -mode ap_ctrl_hs "PID_Controller"
set_directive_interface -mode ap_none "PID_Controller" dout
set_directive_interface -mode ap_none "PID_Controller" test_yi
set_directive_interface -mode ap_none "PID_Controller" test_yd
csim_design -clean
csynth_design
cosim_design
#export_design -evaluate vhdl -format ip_catalog
close_project

open_project hls_pid_prj
set_top PID_Controller
add_files C/xapp_pid.cpp -cflags "-DPID_DEBUG -DPID_FIXEDPOINT"
add_files -tb C/test_data
add_files -tb C/test_xapp_pid.cpp -cflags "-DPID_DEBUG -DPID_FIXEDPOINT"
open_solution "solution_fixed"
set_part {xc7z020clg484-1}
create_clock -period 8 -name default
config_interface -clock_enable -m_axi_offset off
set_directive_allocation -limit 1 -type operation "PID_Controller" mul
set_directive_array_partition -type complete -dim 1 "PID_Controller" coeff
set_directive_array_partition -type complete -dim 1 "PID_Controller" din
set_directive_array_partition -type complete -dim 1 "PID_Controller" dout
set_directive_interface -mode ap_ctrl_hs "PID_Controller"
set_directive_interface -mode ap_none "PID_Controller" dout
set_directive_interface -mode ap_none "PID_Controller" test_yi
set_directive_interface -mode ap_none "PID_Controller" test_yd
csim_design -clean
csynth_design
cosim_design
#export_design -evaluate vhdl -format ip_catalog
close_project

open_project hls_pid_prj
set_top PID_Controller
add_files C/xapp_pid.cpp -cflags "-DPID_DEBUG"
add_files -tb C/test_data
add_files -tb C/test_xapp_pid.cpp -cflags "-DPID_DEBUG"
open_solution "solution_float"
set_part {xc7z020clg484-1}
create_clock -period 8 -name default
#source "directives3.tcl"
set_directive_allocation -limit 1 -type core "PID_Controller" fAddSub
set_directive_allocation -limit 1 -type core "PID_Controller" fMul
set_directive_array_partition -type complete -dim 1 "PID_Controller" coeff
set_directive_array_partition -type complete -dim 1 "PID_Controller" din
set_directive_array_partition -type complete -dim 1 "PID_Controller" dout
set_directive_interface -mode ap_ctrl_hs "PID_Controller"
set_directive_interface -mode ap_none "PID_Controller" dout
set_directive_interface -mode ap_none "PID_Controller" test_yi
set_directive_interface -mode ap_none "PID_Controller" test_yd
csim_design -clean
csynth_design
cosim_design
#export_design -evaluate vhdl -format ip_catalog
close_project










