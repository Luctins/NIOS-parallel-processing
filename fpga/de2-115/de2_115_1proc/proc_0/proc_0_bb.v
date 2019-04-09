
module proc_0 (
	clk_clk,
	reset_reset_n,
	out_bridge_waitrequest,
	out_bridge_readdata,
	out_bridge_readdatavalid,
	out_bridge_burstcount,
	out_bridge_writedata,
	out_bridge_address,
	out_bridge_write,
	out_bridge_read,
	out_bridge_byteenable,
	out_bridge_debugaccess);	

	input		clk_clk;
	input		reset_reset_n;
	input		out_bridge_waitrequest;
	input	[31:0]	out_bridge_readdata;
	input		out_bridge_readdatavalid;
	output	[0:0]	out_bridge_burstcount;
	output	[31:0]	out_bridge_writedata;
	output	[9:0]	out_bridge_address;
	output		out_bridge_write;
	output		out_bridge_read;
	output	[3:0]	out_bridge_byteenable;
	output		out_bridge_debugaccess;
endmodule
