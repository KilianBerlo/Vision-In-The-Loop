	component quadrature_nios_pwm is
		port (
			clk_clk                                      : in std_logic                    := 'X';             -- clk
			reset_reset_n                                : in std_logic                    := 'X';             -- reset_n
			quadrature_encoder_0_input_writebyteenable_n : in std_logic_vector(1 downto 0) := (others => 'X')  -- writebyteenable_n
		);
	end component quadrature_nios_pwm;

	u0 : component quadrature_nios_pwm
		port map (
			clk_clk                                      => CONNECTED_TO_clk_clk,                                      --                        clk.clk
			reset_reset_n                                => CONNECTED_TO_reset_reset_n,                                --                      reset.reset_n
			quadrature_encoder_0_input_writebyteenable_n => CONNECTED_TO_quadrature_encoder_0_input_writebyteenable_n  -- quadrature_encoder_0_input.writebyteenable_n
		);

