	proc_0 u0 (
		.clk_clk                  (<connected-to-clk_clk>),                  //        clk.clk
		.reset_reset_n            (<connected-to-reset_reset_n>),            //      reset.reset_n
		.out_bridge_waitrequest   (<connected-to-out_bridge_waitrequest>),   // out_bridge.waitrequest
		.out_bridge_readdata      (<connected-to-out_bridge_readdata>),      //           .readdata
		.out_bridge_readdatavalid (<connected-to-out_bridge_readdatavalid>), //           .readdatavalid
		.out_bridge_burstcount    (<connected-to-out_bridge_burstcount>),    //           .burstcount
		.out_bridge_writedata     (<connected-to-out_bridge_writedata>),     //           .writedata
		.out_bridge_address       (<connected-to-out_bridge_address>),       //           .address
		.out_bridge_write         (<connected-to-out_bridge_write>),         //           .write
		.out_bridge_read          (<connected-to-out_bridge_read>),          //           .read
		.out_bridge_byteenable    (<connected-to-out_bridge_byteenable>),    //           .byteenable
		.out_bridge_debugaccess   (<connected-to-out_bridge_debugaccess>)    //           .debugaccess
	);

