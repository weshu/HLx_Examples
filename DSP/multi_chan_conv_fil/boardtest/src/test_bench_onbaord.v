///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015 Xilinx, Inc.
// All Rights Reserved
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: 1.0
//  \   \         Application : Vivado HLS
//  /   /         Filename: test_bench_onboard.v
// /___/   /\     Timestamp: Tue May 12 5:00:00 PST 2015
// \   \  /  \
//  \___\/\___\
//
//Command: N/A
//Device:  7K325T-2
//Design Name: multiSRC
//Purpose:
//    This file contains the test bench to test the multiSRC filter
//Reference:
//    XAPP1236
///////////////////////////////////////////////////////////////////////////////

module test_bench_onboard
(
	input clk,
	input rst,
	input test_start,
	output test_done,
	output test_pass,
	// interface to DUT
	input ap_ready,
	input ap_idle,
	input ap_done,
	input        dut_vld_y,
	input [47:0] dut_y,
	output reg   dut_rst,
	output reg   dut_start,
	output reg   dut_vld_x,
	output reg [15:0] dut_x,
	output reg [2:0]  dut_rat
);


reg [14:0] cnt;

reg start_r1;
reg start_r2;
reg start_r3;

reg tv_compare1;
reg	tv_compare2;
reg tv_compare3;

// interface to input test vector RAM
reg [19:0] tvin_rdata;
reg [12:0] tvin_raddr;
reg [47:0] tvout_rdata; 
reg [12:0] tvout_raddr;

reg tv_valid;
reg tv_failure;

reg tvin_valid;
reg tvout_valid;

wire start_s;

reg test_pass_s;
reg tv_valid_p;

reg isfirst;


// clock domain conversion for start
always @(posedge clk) begin
	start_r1  <= test_start;
	start_r2  <= start_r1;
	start_r3  <= start_r2;
end

assign start_s = (start_r3 & (!start_r2));
reg test_done_s;
assign test_done = test_done_s;

//-----------------------------------------
// main counter as the timing generation
//-----------------------------------------
always @(posedge clk, posedge rst) begin

	// main counter
	if(rst) begin
		cnt <= 0;
	end else begin
		cnt <= (start_s==1'b1) ? 1      :  
			   (cnt==0)  ?       0      : cnt+1;			   
	end

end 


//-----------------------------------------------------
// dut interface and test vector rom address control
//-----------------------------------------------------
always @(posedge clk, posedge rst) begin

	// main counter
	if(rst) begin
		dut_rst    <= 1'b1;
		dut_start  <= 1'b0;
		dut_vld_x  <= 1'b0;
		dut_x      <= 0;
		dut_rat    <= 0;
		
		tvin_valid  <= 1'b0;
		tvout_valid <= 1'b0;
		
		tv_valid   <= 1'b0;
		tv_valid_p <= 1'b0;
		tv_failure <= 1'b0;
		
		test_done_s <= 1'b1;
		isfirst <= 1'b1;
		
	end else begin
		
		dut_rst    <= (isfirst && (cnt<32))   ? 1'b1 : 1'b0;
		dut_start  <= (isfirst && (cnt<64))   ? 1'b0 : 1'b1;
		
		dut_vld_x  <= (cnt<128)  ? 1'b0 : 
					  (cnt<1024) ? 1'b1 : tvin_rdata[19];
		dut_x      <= (cnt<1024) ? 0    : tvin_rdata[15:0];
		dut_rat    <= (cnt<1024) ? 0    : tvin_rdata[18:16];
		
		
		tvin_valid  <= (cnt<1022)      ? 1'b0 : 
		               (cnt>1022+8191) ? 1'b0 : 1'b1;
	
		tvout_valid <= (cnt<(1022+62))      ? 1'b0 : 
		               (cnt>(1022+62+8191)) ? 1'b0 : 1'b1;
					   
	
		tv_valid_p  <= tvout_valid;
		tv_valid    <= tv_valid_p;
		tv_failure  <= (tv_valid==1'b1) && (tv_compare3==1'b1) && ( (tv_compare1==1'b0) || (tv_compare2==1'b0) );

		test_done_s <= (start_s==1'b1) ? 1'b0 :
					   (cnt==8192+1024+256) ? 1'b1 : test_done_s;
					   
		isfirst <= (cnt==8192+1024+256) ? 1'b0 : isfirst;
		
    end

end 



//-----------------------------------------------------
// ROM Handling
//-----------------------------------------------------
reg [19:0] tvin_rom[8191:0];
reg [47:0] tvout_rom[8191:0];

initial begin
    $readmemh("./tvin_rom.dat", tvin_rom);
	$readmemh("./tvout_rom.dat", tvout_rom);
end

always @(posedge clk) begin

	tvin_raddr  <= (tvin_valid==1'b0)?  0 : tvin_raddr+1;
	tvout_raddr <= (tvout_valid==1'b0)? 0 : tvout_raddr+1;

	tvin_rdata  <= tvin_rom[tvin_raddr];
	tvout_rdata <= tvout_rom[tvout_raddr];
	
	tv_compare1  <= (dut_vld_y==1'b0) || (tvout_rdata[46:24]==dut_y[46:24]);
	tv_compare2  <= (dut_vld_y==1'b0) || (tvout_rdata[23:0]==dut_y[23:0]);
	tv_compare3  <= (tvout_rdata[47]==dut_vld_y);
	
end 
//-----------------------------------------------------


assign test_pass = test_pass_s;

always @(posedge clk, posedge rst) begin

	if(rst) begin
		test_pass_s <= 1'b1;
	end else begin
		test_pass_s <= (start_s==1'b1)?    1'b1 :
					 (tv_failure==1'b1)? 1'b0 : test_pass;
	end
		
end 

endmodule
