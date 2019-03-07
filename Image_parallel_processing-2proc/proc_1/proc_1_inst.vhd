	component proc_1 is
		port (
			clk_clk                         : in  std_logic                     := 'X';             -- clk
			out_bridge_master_waitrequest   : in  std_logic                     := 'X';             -- waitrequest
			out_bridge_master_readdata      : in  std_logic_vector(31 downto 0) := (others => 'X'); -- readdata
			out_bridge_master_readdatavalid : in  std_logic                     := 'X';             -- readdatavalid
			out_bridge_master_burstcount    : out std_logic_vector(0 downto 0);                     -- burstcount
			out_bridge_master_writedata     : out std_logic_vector(31 downto 0);                    -- writedata
			out_bridge_master_address       : out std_logic_vector(9 downto 0);                     -- address
			out_bridge_master_write         : out std_logic;                                        -- write
			out_bridge_master_read          : out std_logic;                                        -- read
			out_bridge_master_byteenable    : out std_logic_vector(3 downto 0);                     -- byteenable
			out_bridge_master_debugaccess   : out std_logic;                                        -- debugaccess
			reset_reset_n                   : in  std_logic                     := 'X'              -- reset_n
		);
	end component proc_1;

	u0 : component proc_1
		port map (
			clk_clk                         => CONNECTED_TO_clk_clk,                         --               clk.clk
			out_bridge_master_waitrequest   => CONNECTED_TO_out_bridge_master_waitrequest,   -- out_bridge_master.waitrequest
			out_bridge_master_readdata      => CONNECTED_TO_out_bridge_master_readdata,      --                  .readdata
			out_bridge_master_readdatavalid => CONNECTED_TO_out_bridge_master_readdatavalid, --                  .readdatavalid
			out_bridge_master_burstcount    => CONNECTED_TO_out_bridge_master_burstcount,    --                  .burstcount
			out_bridge_master_writedata     => CONNECTED_TO_out_bridge_master_writedata,     --                  .writedata
			out_bridge_master_address       => CONNECTED_TO_out_bridge_master_address,       --                  .address
			out_bridge_master_write         => CONNECTED_TO_out_bridge_master_write,         --                  .write
			out_bridge_master_read          => CONNECTED_TO_out_bridge_master_read,          --                  .read
			out_bridge_master_byteenable    => CONNECTED_TO_out_bridge_master_byteenable,    --                  .byteenable
			out_bridge_master_debugaccess   => CONNECTED_TO_out_bridge_master_debugaccess,   --                  .debugaccess
			reset_reset_n                   => CONNECTED_TO_reset_reset_n                    --             reset.reset_n
		);

