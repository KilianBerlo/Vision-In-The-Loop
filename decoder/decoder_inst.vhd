	component decoder is
		port (
			clk_clk                                        : in  std_logic                    := 'X';             -- clk
			esl_encoder_0_avalon_slave_0_readdata          : out std_logic_vector(7 downto 0);                    -- readdata
			esl_encoder_0_avalon_slave_0_writebyteenable_n : in  std_logic_vector(1 downto 0) := (others => 'X'); -- writebyteenable_n
			reset_reset_n                                  : in  std_logic                    := 'X'              -- reset_n
		);
	end component decoder;

	u0 : component decoder
		port map (
			clk_clk                                        => CONNECTED_TO_clk_clk,                                        --                          clk.clk
			esl_encoder_0_avalon_slave_0_readdata          => CONNECTED_TO_esl_encoder_0_avalon_slave_0_readdata,          -- esl_encoder_0_avalon_slave_0.readdata
			esl_encoder_0_avalon_slave_0_writebyteenable_n => CONNECTED_TO_esl_encoder_0_avalon_slave_0_writebyteenable_n, --                             .writebyteenable_n
			reset_reset_n                                  => CONNECTED_TO_reset_reset_n                                   --                        reset.reset_n
		);

