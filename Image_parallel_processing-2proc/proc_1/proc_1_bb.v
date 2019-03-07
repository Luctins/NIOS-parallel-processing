
module proc_1 (
	clk_clk,
	out_bridge_master_waitrequest,
	out_bridge_master_readdata,
	out_bridge_master_readdatavalid,
	out_bridge_master_burstcount,
	out_bridge_master_writedata,
	out_bridge_master_address,
	out_bridge_master_write,
	out_bridge_master_read,
	out_bridge_master_byteenable,
	out_bridge_master_debugaccess,
	reset_reset_n);	

	input		clk_clk;
	input		out_bridge_master_waitrequest;
	input	[31:0]	out_bridge_master_readdata;
	input		out_bridge_master_readdatavalid;
	output	[0:0]	out_bridge_master_burstcount;
	output	[31:0]	out_bridge_master_writedata;
	output	[9:0]	out_bridge_master_address;
	output		out_bridge_master_write;
	output		out_bridge_master_read;
	output	[3:0]	out_bridge_master_byteenable;
	output		out_bridge_master_debugaccess;
	input		reset_reset_n;
endmodule
