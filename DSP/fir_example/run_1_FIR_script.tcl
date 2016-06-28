############################################################
## FIR 1 channel, fractional fixed point precision.
##
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
##
## daniele.bagni@xilinx.com
## 22 July 2015
############################################################

open_project hls_fir1ch_prj
set_top fir_filter
add_files fir.cpp
add_files -tb fir_test.cpp
add_files -tb data

############ SOLUTION0 and its variants ###################

# baseline solution0: no directives at all
open_solution "solution0"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
csim_design -clean
csynth_design

# as solution0 plus PIPELINE the internal loop
open_solution "solution0_b"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_pipeline "fir_filter/Shift_Accum_Loop"
csynth_design

# as solution0_b plus complete PARTITION of the shift_reg ARRAY
open_solution "solution0_c"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline "fir_filter/Shift_Accum_Loop"
csynth_design

# as solution0_c plus UNROLL completely the internal loop
open_solution "solution0_d"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline "fir_filter/Shift_Accum_Loop"
set_directive_unroll -factor 16 "fir_filter/Shift_Accum_Loop"
csynth_design

# as solution0_d plus complete PARTITION of the c ARRAY 
open_solution "solution0_e"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline "fir_filter/Shift_Accum_Loop"
set_directive_unroll -factor 16 "fir_filter/Shift_Accum_Loop"
set_directive_array_partition -type complete -dim 1 "fir_filter" c
csynth_design

############ SOLUTION1 and its variants ###################

# solution1_II1: all arrays partitioned and PIPELINE at top level with II=1
open_solution "solution1_II1"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline -II 1 "fir_filter"
csynth_design
cosim_design
export_design -evaluate verilog -format ip_catalog

# as solution1_II1, but PIPELINE at top level with II=2 
#                   and ALLOCATION max 8 mult operations
open_solution "solution1_II2"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline -II 2 "fir_filter"
set_directive_allocation -limit 8 -type operation "fir_filter" mul
csynth_design

# as solution1_II1, but PIPELINE at top level with II=4 
#                   and ALLOCATION max 4 mult operations
open_solution "solution1_II4"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_interface -mode ap_none -register "fir_filter" x
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline -II 4 "fir_filter"
set_directive_allocation -limit 4 -type operation "fir_filter" mul
csynth_design

# as solution1_II1, but PIPELINE at top level with II=8 
#                   and ALLOCATION max 2 mult operations
open_solution "solution1_II8"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_interface -mode ap_none -register "fir_filter" x
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline -II 8 "fir_filter"
set_directive_allocation -limit 2 -type operation "fir_filter" mul
csynth_design

# as solution1_II1, but PIPELINE at top level with II=16
#                   and ALLOCATION max 1 mult operations
open_solution "solution1_II16"
set_part  {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_interface -mode ap_none -register "fir_filter" x
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_pipeline -II 16 "fir_filter"
set_directive_allocation -limit 1 -type operation "fir_filter" mul
csynth_design

############ SOLUTION2 and its variants ###################

# solution2_unroll2: all arrays partitioned and UNROLL the inner loop by 2
open_solution "solution2_unroll2"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_unroll -factor 2 "fir_filter/Shift_Accum_Loop"
csynth_design

# solution2_unroll4: all arrays partitioned and UNROLL the inner loop by 4
open_solution "solution2_unroll4"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_unroll -factor 4 "fir_filter/Shift_Accum_Loop"
csynth_design

# solution2_unroll8: all arrays partitioned and UNROLL the inner loop by 8
open_solution "solution2_unroll8"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_unroll -factor 8 "fir_filter/Shift_Accum_Loop"
csynth_design

# solution2_unroll16: all arrays partitioned and UNROLL the inner loop by 16
open_solution "solution2_unroll16"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_unroll -factor 16 "fir_filter/Shift_Accum_Loop"
csynth_design

# solution2_full_pipeline: as solution2_unroll16, plus PIPELINE at top level
open_solution "solution2_unroll16_full_pipelined"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" c
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_unroll -factor 16 "fir_filter/Shift_Accum_Loop"
set_directive_pipeline -II 1 "fir_filter"
csynth_design


############ SOLUTION3 and its variants ###################

#baseline of solution3
open_solution "solution3"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_array_partition -type complete -dim 1 "fir_filter" c
csynth_design

#as solution3 plus LATENCY=10 in the inner loop
open_solution "solution3_lat10"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_latency -min 10 -max 10 "fir_filter/Shift_Accum_Loop"
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_array_partition -type complete -dim 1 "fir_filter" c
csynth_design

#as solution3 plus LATENCY=20 in the inner loop
open_solution "solution3_lat20"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_directive_latency -min 20 -max 20 "fir_filter/Shift_Accum_Loop"
set_directive_array_partition -type complete -dim 1 "fir_filter" shift_reg
set_directive_array_partition -type complete -dim 1 "fir_filter" c
csynth_design


