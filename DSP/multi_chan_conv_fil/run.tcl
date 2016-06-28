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
### // /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
### // \   \  /  \
### //  \___\/\___\
### //
### //Command: vivado_hls run.tcl
### //Device:  7K325T-2
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
set Project     ProjMultiSRC
set Solution    SolutionX
set Device      "xc7k325tffg900-2"
set Flow        ""
set Clock       4
set DefaultFlag 1

#### query qor settings
if {[info exists env(AVE_LOAD_QOR_SETTING)] && $env(AVE_LOAD_QOR_SETTING) == 1} {
    catch {
        source [file join $env(AVE_ROOT) scripts QorSetting.tcl]
        QorSetting::loadSetting ;# this api will overwite the above variables
    }
}

#### main part

# Project settings
open_project $Project -reset

# Add the file for synthesis
add_files src/srcMac.cpp
add_files src/srcCtrl.cpp
add_files src/srcCoef.cpp
add_files src/multiSRC.cpp

# Add testbench files for co-simulation
add_files -tb  tb/tb_multiSRC.cpp
add_files -tb  tv/tv_multiSRC_in.txt
for { set i 0 } { $i < 8 } { incr i } { 
	add_files -tb  tv/tv_multiSRC_mout_$i.txt
}

# Set top module of the design
set_top multiSRC

# Solution settings
open_solution -reset $Solution

# Add library
if {$Device == ""} {
    # default
    add_library xilinx/virtex5/virtex5:xc5vsx50t:ff1136:-1
} else {
    set_part $Device
}

# Set the target clock period
create_clock -period $Clock
set_clock_uncertainty 0.4ns

###############
## Directives #
##############
config_rtl -form_dsp

# IO interface
set_directive_interface -mode ap_none multiSRC x_i
set_directive_interface -mode ap_none multiSRC vld_i
set_directive_interface -mode ap_none multiSRC rat_i
set_directive_interface -mode ap_none multiSRC vld_o
set_directive_interface -mode ap_none multiSRC y_o

# Loop pipeline
set_directive_pipeline -II 1  multiSRC

set_directive_dependence -variable x_r       -type inter -dependent false srcMac
set_directive_dependence -variable acc_r     -type inter -dependent false srcMac
set_directive_dependence -variable toshift_r -type inter -dependent false srcMac
set_directive_unroll -skip_exit_check   srcMac/MULTMACLOOP
set_directive_reset srcCtrl cnt_r
set_directive_reset srcCtrl cnt_w
set_directive_dependence -variable vec_phase  -type inter -dependent false srcCtrl
set_directive_dependence -variable vec_tokeep -type inter -dependent false srcCtrl
set_directive_dependence -variable vec_isskip -type inter -dependent false srcCtrl

# Function inline
set_directive_inline -region -recursive multiSRC

# Array related
set_directive_array_partition -dim 1 srcCoef coef_rom
set_directive_resource -core ROM_1P  srcCoef coef_rom 
set_directive_array_partition -type complete srcCoef addr
set_directive_array_partition -type complete srcMac c_i
set_directive_array_partition -type complete srcMac x_r
set_directive_array_partition -type complete srcMac acc_r
set_directive_array_partition -type complete srcMac toshift_r
set_directive_resource -core RAM_2P  srcCtrl vec_phase
set_directive_resource -core RAM_2P  srcCtrl vec_tokeep
set_directive_resource -core RAM_2P  srcCtrl vec_isskip


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

#### dump settings for ave coregen stage
if {[info exists env(AVE_LOAD_QOR_SETTING)] && $env(AVE_LOAD_QOR_SETTING) == 1} {
    catch {
        QorSetting::dumpSetting; # this api will dump coregen settings under solution
      }
}

exit
