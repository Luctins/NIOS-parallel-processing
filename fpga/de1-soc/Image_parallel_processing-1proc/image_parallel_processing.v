

module image_parallel_processing(

      ///////// CLOCK2 /////////
      input              CLOCK2_50,

      ///////// CLOCK3 /////////
      input              CLOCK3_50,

      ///////// CLOCK4 /////////
      input              CLOCK4_50,

      ///////// CLOCK /////////
      input              CLOCK_50,

      ///////// DRAM /////////
      output      [12:0] DRAM_ADDR,

      output      [1:0]  DRAM_BA,
      output             DRAM_CAS_N,
      output             DRAM_CKE,
      output             DRAM_CLK,
      output             DRAM_CS_N,
      inout       [15:0] DRAM_DQ,
      output             DRAM_LDQM,
      output             DRAM_RAS_N,
      output             DRAM_UDQM,
      output             DRAM_WE_N,

      ///////// VGA /////////
      output      [7:0]  VGA_B,
      output             VGA_BLANK_N,
      output             VGA_CLK,
      output      [7:0]  VGA_G,
      output             VGA_HS,
      output      [7:0]  VGA_R,
      output             VGA_SYNC_N,
      output             VGA_VS
);

image_parallel_processing_qsys U0(
		.clk_clk(CLOCK_50),                        	//                     clk.clk
		.reset_reset_n(1'b1),                  		//                   reset.reset_n

		.clk_sdram_clk(DRAM_CLK),                  	//               clk_sdram.clk
	   .sdram_wire_addr(DRAM_ADDR),                	//              sdram_wire.addr
		.sdram_wire_ba(DRAM_BA),                  	//                        .ba
		.sdram_wire_cas_n(DRAM_CAS_N),               //                        .cas_n
		.sdram_wire_cke(DRAM_CKE),                 	//                        .cke
		.sdram_wire_cs_n(DRAM_CS_N),                	//                        .cs_n
		.sdram_wire_dq(DRAM_DQ),                  	//                        .dq
		.sdram_wire_dqm({DRAM_UDQM,DRAM_LDQM}),     	//                        .dqm
		.sdram_wire_ras_n(DRAM_RAS_N),              	//                        .ras_n
		.sdram_wire_we_n(DRAM_WE_N)                 	//                        .we_n
	);
endmodule
