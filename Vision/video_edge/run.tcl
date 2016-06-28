open_project proj
set_top image_filter
add_files image_filter.cpp
add_files -tb image_filter_test.cpp
#add_files -tb small_image.bmp
#add_files -tb image_one.png
#add_files -tb image_two.png
#add_files -tb image_three.png
#add_files -tb image_four.png
#add_files -tb test_1080p.bmp
add_files -tb checkerboard.bmp
#add_files -tb golden_output.bmp
#
open_solution "sol1"
set_part {xc7z020clg484-1}
create_clock -period 6 -name default
#source "directives.tcl"
csim_design -clean
csynth_design
#cosim_design -trace_level all -tool modelsim
#cosim_design -trace_level port -tool modelsim
#export_design -format ip_catalog -vendor "NCR" -version "1.011" -display_name "PreprocessFrame"
exit

