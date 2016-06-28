### ///////////////////////////////////////////////////////////////////////////////
### // Copyright (c) 2015 Xilinx, Inc.
### // All Rights Reserved
### ///////////////////////////////////////////////////////////////////////////////
### //   ____  ____
### //  /   /\/   /
### // /___/  \  /    Vendor: Xilinx
### // \   \   \/     Version: 1.0
### //  \   \         Application : Vivado HLS
### //  /   /         Filename: run.tcl
### // /___/   /\     Timestamp: Tue Aug 11 5:00:00 PST 2015
### // \   \  /  \
### //  \___\/\___\
### //
### //Command: vivado_hls run.tcl
### //Device:  XCKU035-2i
### //Design Name: multiSRC
### //Purpose:
### //    This file is the script to 
### //    1) do C++ functional simulation
### //    2) synthesize C++ into RTL 
### //    3) do functional verificaiton for the generated RTL
### //    4) synthesize the RTL with Vivado to check actual resource utilization
### //       and block-level timing performance
### //Reference:
### //    XAPP1236
### ///////////////////////////////////////////////////////////////////////////////

### default setting
set Project     ProjRSErasure
set Solution    SolutionX
set Device      "xcku040-ffva1156-2-e"
set Flow        ""
set Clock       3.3
set DefaultFlag 1

#### main part

# Project settings
open_project $Project -reset

# Add the file for synthesis
add_files src/rs_erasure.c
#add_files src/rs_roms.c

# Add testbench files for co-simulation
add_files -tb  tb/tb_rs_erasure.c
add_files -tb  tv/tv_rs_erasure_in.txt
add_files -tb  tv/tv_rs_erasure_mout.txt

# Set top module of the design
set_top rs_erasure

# Solution settings
open_solution -reset $Solution

# Add library
set_part $Device

# Set the target clock period
create_clock -period $Clock
set_clock_uncertainty 0.3ns

###############
## Directives #
##############

# IO interface
#set_directive_array_partition -dim 1 rs_erasure c
#set_directive_array_partition -dim 1 rs_erasure d
set_directive_array_partition -type complete rs_erasure c
set_directive_array_partition -type complete rs_erasure d
#set_directive_interface -mode ap_none rs_erasure c
set_directive_interface -mode ap_none rs_erasure d
#set_directive_interface -mode ap_none rs_erasure survival_pattern
set_directive_interface -mode ap_vld rs_erasure codeidx


# Loop pipeline
set_directive_pipeline -II 1  rs_erasure

# set resource mapping
#set_directive_array_partition -dim 2 rs_erasure rom_decmat
#set_directive_resource -core ROM_1P_BRAM rs_erasure rom_decmat 

#################
# C SIMULATION
#################
csim_design

#############
# SYNTHESIS #
#############
#config_schedule -verbose
csynth_design

#################
# CO-SIMULATION #
#################
#cosim_design -tool modelsim -rtl verilog -trace_level all
cosim_design -rtl verilog -trace_level all

##################
# IMPLEMENTATION #
##################
export_design -evaluate verilog -format ipxact


exit
