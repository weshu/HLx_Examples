# ///////////////////////////////////////////////////////////////////////////////
# // Copyright (c) 2015 Xilinx, Inc.
# // All Rights Reserved
# ///////////////////////////////////////////////////////////////////////////////
# //   ____  ____
# //  /   /\/   /
# // /___/  \  /    Vendor: Xilinx
# // \   \   \/     Version: 1.0
# //  \   \         Application : Vivado HLS
# //  /   /         Filename: boardtest.tcl
# // /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
# // \   \  /  \
# //  \___\/\___\
# //
# //Command: N/A
# //Device:  7Z045
# //Design Name: multiSRC
# //Purpose:
# //    This file contains the script to synthesize Vivado HLS generated RTL with
# //    the testbench coded in verilog to run on 7Z045 on ZC706 EVB.
# //Reference:
# //    XAPP1236
# ///////////////////////////////////////////////////////////////////////////////
#
# STEP#1: define the output directory area.
#
set outputDir ./test_output 
file mkdir $outputDir

#create_project vivadoprj ./test_output -part xc7z045ffg900-2 -force

#
# STEP#2: setup design sources and constraints
#
#read_vhdl -library bftLib [ glob ./Sources/hdl/bftLib/*.vhdl ]
#read_vhdl ./Sources/hdl/bft.vhdl
read_verilog [ glob ./src/*.v ]
add_files [ glob ./src/*.dat ]
read_verilog [ glob ../ProjMultiSRC/SolutionX/syn/verilog/*.v ]
add_files [ glob ../ProjMultiSRC/SolutionX/syn/verilog/*.dat ]

read_xdc ./src/zc706_ucf.xdc
#
# STEP#3: run synthesis, write design checkpoint, report timing, 
# and utilization estimates
#
synth_design -top multiSRC_onboard -part xc7z045ffg900-2
write_checkpoint -force $outputDir/post_synth.dcp
report_timing_summary -file $outputDir/post_synth_timing_summary.rpt
report_utilization -file $outputDir/post_synth_util.rpt
# Run custom script to report critical timing paths
#reportCriticalPaths $outputDir/post_synth_critpath_report.csv
#
# STEP#4: run logic optimization, placement and physical logic optimization, 
# write design checkpoint, report utilization and timing estimates
#
opt_design
#reportCriticalPaths $outputDir/post_opt_critpath_report.csv
place_design
report_clock_utilization -file $outputDir/clock_util.rpt
#
# Optionally run optimization if there are timing violations after placement
if {[get_property SLACK [get_timing_paths -max_paths 1 -nworst 1 -setup]] < 0} {
 puts "Found setup timing violations => running physical optimization"
 phys_opt_design
}
write_checkpoint -force $outputDir/post_place.dcp
report_utilization -file $outputDir/post_place_util.rpt
report_timing_summary -file $outputDir/post_place_timing_summary.rpt
#
# STEP#5: run the router, write the post-route design checkpoint, report the routing
# status, report timing, power, and DRC, and finally save the Verilog netlist.
#
route_design
write_checkpoint -force $outputDir/post_route.dcp
report_route_status -file $outputDir/post_route_status.rpt
report_timing_summary -file $outputDir/post_route_timing_summary.rpt
report_power -file $outputDir/post_route_power.rpt
#report_power -file $outputDir/post_route_power.rpt
report_drc -file $outputDir/post_imp_drc.rpt
#write_verilog -force $outputDir/multiSRC_impl_netlist.v -mode timesim -sdf_anno true
#
# STEP#6: generate a bitstream
# 
write_bitstream -force $outputDir/multiSRC.bit
