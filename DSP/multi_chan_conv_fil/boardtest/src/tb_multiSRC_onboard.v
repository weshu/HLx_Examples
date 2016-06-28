///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: tb_multiSRC_onboard.v
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file is the functional verification testbench for the multiSRC 
//    onboard test.
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////
`timescale 1 ns/1 ps

module tb_multiSRC_onboard;


parameter CLKCYCLE = 4;



reg test_start;
wire test_done;
wire test_pass;


reg clk;
reg rst;

integer i;

wire ap_start;
wire ap_ready;
wire ap_idle;
wire ap_done;
wire dut_rst;
wire vld_x;
wire vld_y;
wire [15:0] x;
wire [2:0]  rat;
wire [47:0] y;



//-----------------------------------
// generate clock and reset
//-----------------------------------
initial begin
	clk=1'b0;
	rst=1'b1;
	#14;
	@(posedge clk);
	@(posedge clk);
	@(posedge clk);
	rst=1'b0;

end


//---- clock gen -------------------
always 
    #(CLKCYCLE/2) clk = ~clk;
//----------------------------------



initial begin

	test_start=1'b0;
	#30;
	test_start=1'b1;
	#100;
	test_start=1'b0;
	
	for(i=0;i< (2048+8192);i=i+1) begin
		@(posedge clk);
	end
	
	test_start=1'b1;
	#100;
	test_start=1'b0;
  
end





test_bench_onboard TB
(
	.clk(clk),
	.rst(rst),
	.test_start(test_start),
	.test_done(test_done),
	.test_pass(test_pass),
	.ap_ready(ap_ready),
	.ap_idle(ap_idle),
	.ap_done(ap_done),
	.dut_vld_y(vld_y),
	.dut_y(y),
	.dut_rst(dut_rst),
	.dut_start(ap_start),
	.dut_vld_x(vld_x),
	.dut_x(x),
	.dut_rat(rat)
);


multiSRC DUT(
        .ap_clk(clk),
        .ap_rst(dut_rst),
        .ap_start(ap_start),
        .ap_done(ap_done),
        .ap_idle(ap_idle),
        .ap_ready(ap_ready),
        .vld_i(vld_x),
        .x_i_V(x),
        .rat_i_V(rat),
        .vld_o(vld_y),
        .y_o_V(y)
);

	
endmodule
