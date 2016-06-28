open_project VHLS_proj
set_top crec
add_files crec.cpp
add_files -tb crec_tb.cpp
add_files -tb cin.txt
open_solution "sol1"
set_part {xc7a100tcsg324-3}
create_clock -period 10 -name default
set_clock_uncertainty 0
csim_design
#source "./VHLS_proj/sol3_dataflow_io/directives.tcl"
set_directive_allocation -limit 1 -type operation "mix" mul
set_directive_interface -mode ap_none -register "crec" dout_mix
set_directive_interface -mode ap_none -register "crec" ph_out
set_directive_interface -mode ap_none -register "crec" loop_integ
set_directive_dataflow "crec"
csynth_design
#cosim_design
#export_design -format sysgen
exit
