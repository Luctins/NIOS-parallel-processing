	component proc_0 is
		port (
			clk_clk                  : in  std_logic                     := 'X';             -- clk
			reset_reset_n            : in  std_logic                     := 'X';             -- reset_n
			out_bridge_waitrequest   : in  std_logic                     := 'X';             -- waitrequest
			out_bridge_readdata      : in  std_logic_vector(31 downto 0) := (others => 'X'); -- readdata
			out_bridge_readdatavalid : in  std_logic                     := 'X';             -- readdatavalid
			out_bridge_burstcount    : out std_logic_vector(0 downto 0);                     -- burstcount
			out_bridge_writedata     : out std_logic_vector(31 downto 0);                    -- writedata
			out_bridge_address       : out std_logic_vector(9 downto 0);                     -- address
			out_bridge_write         : out std_logic;                                        -- write
			out_bridge_read          : out std_logic;                                        -- read
			out_bridge_byteenable    : out std_logic_vector(3 downto 0);                     -- byteenable
			out_bridge_debugaccess   : out std_logic                                         -- debugaccess
		);
	end component proc_0;

	u0 : component proc_0
		port map (
			clk_clk                  => CONNECTED_TO_clk_clk,                  --        clk.clk
			reset_reset_n            => CONNECTED_TO_reset_reset_n,            --      reset.reset_n
			out_bridge_waitrequest   => CONNECTED_TO_out_bridge_waitrequest,   -- out_bridge.waitrequest
			out_bridge_readdata      => CONNECTED_TO_out_bridge_readdata,      --           .readdata
			out_bridge_readdatavalid => CONNECTED_TO_out_bridge_readdatavalid, --           .readdatavalid
			out_bridge_burstcount    => CONNECTED_TO_out_bridge_burstcount,    --           .burstcount
			out_bridge_writedata     => CONNECTED_TO_out_bridge_writedata,     --           .writedata
			out_bridge_address       => CONNECTED_TO_out_bridge_address,       --           .address
			out_bridge_write         => CONNECTED_TO_out_bridge_write,         --           .write
			out_bridge_read          => CONNECTED_TO_out_bridge_read,          --           .read
			out_bridge_byteenable    => CONNECTED_TO_out_bridge_byteenable,    --           .byteenable
			out_bridge_debugaccess   => CONNECTED_TO_out_bridge_debugaccess    --           .debugaccess
		);

