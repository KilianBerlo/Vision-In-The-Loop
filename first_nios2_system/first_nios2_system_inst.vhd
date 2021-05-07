	component first_nios2_system is
		port (
			clk_clk                              : in  std_logic                    := 'X'; -- clk
			reset_reset_n                        : in  std_logic                    := 'X'; -- reset_n
			esl_bus_demo_0_user_interface_export : out std_logic_vector(7 downto 0)         -- export
		);
	end component first_nios2_system;

	u0 : component first_nios2_system
		port map (
			clk_clk                              => CONNECTED_TO_clk_clk,                              --                           clk.clk
			reset_reset_n                        => CONNECTED_TO_reset_reset_n,                        --                         reset.reset_n
			esl_bus_demo_0_user_interface_export => CONNECTED_TO_esl_bus_demo_0_user_interface_export  -- esl_bus_demo_0_user_interface.export
		);

