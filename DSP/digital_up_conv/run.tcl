open_project -reset proj

add_files duc_2ch.cpp
add_files dds.cpp
add_files -tb duc_2ch_test.cpp

set_top duc_2ch
open_solution -reset sol1

set_part  {xc7k160tfbg484-1}
create_clock -period 3

#set_part {xc7z020clg484-1}
#create_clock -period 5

#---- crucial directive for
# 1. to use fifo instead of BRAM
# 2. synthesis runs forever if not used
# depth can affect the II
#
# config_dataflow -default_channel fifo -fifo_depth 2
config_dataflow -default_channel fifo -fifo_depth 16


# ISSUE: 
# - csim does not match
# - rtl cosim not match
#
# csim_design
csynth_design
#cosim_design -rtl verilog -trace_level port
#cosim_design -rtl verilog -tool modelsim -trace_level port
#export_design -evaluate verilog -format ip_catalog
#export_design -evaluate verilog -format sysgen
#export_design -format sysgen

exit

