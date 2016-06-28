#cadman add -t xilinx -v 2015.1 -p vivado_gsd
cd ../hls
vivado_hls -f run_hls.flashModel.tcl
vivado_hls -f run_hls.memcachedPipeline.tcl
vivado_hls -f run_hls.readConverter.tcl
vivado_hls -f run_hls.writeConverter.tcl
vivado_hls -f run_hls.dramModel.tcl  
vivado_hls -f run_hls.dummyPCIeJoint.tcl
cd ../scripts
