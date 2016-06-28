open_project -reset proj
add_files dds.cpp
add_files -tb dds_test.cpp
set_top dds

#set hls_exec 2

#------------------------------- first solution
if {0} {
open_solution -reset sol1
set_part  {xc7k160tfbg484-1}
create_clock -period "400MHz"

csim_design
csynth_design
#cosim_design -rtl verilog -tool modelsim -trace_level port
}

#------------------------------- first solution
# 2-3 min for csim, csynth
# few min for cosim
# 1 min for export
#
open_solution -reset sol2_axi
set_part  {xc7k160tfbg484-1}
create_clock -period "400MHz"
source directives_axi.tcl

csim_design
csynth_design
#cosim_design -rtl verilog -tool modelsim -trace_level port
#export_design -format sysgen

exit

