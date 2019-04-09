	proc_0 u0 (
		.clk_clk                         (<connected-to-clk_clk>),                         //               clk.clk
		.out_bridge_master_waitrequest   (<connected-to-out_bridge_master_waitrequest>),   // out_bridge_master.waitrequest
		.out_bridge_master_readdata      (<connected-to-out_bridge_master_readdata>),      //                  .readdata
		.out_bridge_master_readdatavalid (<connected-to-out_bridge_master_readdatavalid>), //                  .readdatavalid
		.out_bridge_master_burstcount    (<connected-to-out_bridge_master_burstcount>),    //                  .burstcount
		.out_bridge_master_writedata     (<connected-to-out_bridge_master_writedata>),     //                  .writedata
		.out_bridge_master_address       (<connected-to-out_bridge_master_address>),       //                  .address
		.out_bridge_master_write         (<connected-to-out_bridge_master_write>),         //                  .write
		.out_bridge_master_read          (<connected-to-out_bridge_master_read>),          //                  .read
		.out_bridge_master_byteenable    (<connected-to-out_bridge_master_byteenable>),    //                  .byteenable
		.out_bridge_master_debugaccess   (<connected-to-out_bridge_master_debugaccess>),   //                  .debugaccess
		.reset_reset_n                   (<connected-to-reset_reset_n>)                    //             reset.reset_n
	);

