
module proc_toplevel (
	altpll_0_locked_conduit_export,
	clk_clk,
	clk_sdram_clk,
	reset_reset_n,
	sdram_controller_wire_addr,
	sdram_controller_wire_ba,
	sdram_controller_wire_cas_n,
	sdram_controller_wire_cke,
	sdram_controller_wire_cs_n,
	sdram_controller_wire_dq,
	sdram_controller_wire_dqm,
	sdram_controller_wire_ras_n,
	sdram_controller_wire_we_n);	

	output		altpll_0_locked_conduit_export;
	input		clk_clk;
	output		clk_sdram_clk;
	input		reset_reset_n;
	output	[12:0]	sdram_controller_wire_addr;
	output		sdram_controller_wire_ba;
	output		sdram_controller_wire_cas_n;
	output		sdram_controller_wire_cke;
	output	[1:0]	sdram_controller_wire_cs_n;
	inout	[31:0]	sdram_controller_wire_dq;
	output	[3:0]	sdram_controller_wire_dqm;
	output		sdram_controller_wire_ras_n;
	output		sdram_controller_wire_we_n;
endmodule
