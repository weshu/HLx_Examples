open_project -reset proj
add_files fir_super.cpp
add_files -tb fir_super_test.cpp
add_files -tb coef128.inc

set_top fir_super
open_solution -reset sol1

set_part  {xc7k160tfbg484-1}
create_clock -period 2.5

# Source x_hls.tcl to determine which steps to execute
set hls_exec 1

csim_design

# Set any optimization directives
# End of directives

if {$hls_exec == 1} {
	csynth_design
	
} elseif {$hls_exec == 2} {
	csynth_design
	cosim_design -rtl verilog -tool xsim

} elseif {$hls_exec == 3} { 
# takes about 2 mins
	csynth_design
	cosim_design
	export_design -evaluate verilog -format sysgen

} else {
	csynth_design
}

exit

