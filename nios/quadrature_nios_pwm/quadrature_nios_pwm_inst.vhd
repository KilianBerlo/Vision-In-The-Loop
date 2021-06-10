	component quadrature_nios_pwm is
		port (
			clk_clk                                 : in  std_logic                    := 'X';             -- clk
			motor_pwm_0_motor_control_direction_out : out std_logic_vector(1 downto 0);                    -- direction_out
			motor_pwm_0_motor_control_pwm_out       : out std_logic;                                       -- pwm_out
			motor_pwm_1_motor_control_direction_out : out std_logic_vector(1 downto 0);                    -- direction_out
			motor_pwm_1_motor_control_pwm_out       : out std_logic;                                       -- pwm_out
			pio_0_external_connection_export        : out std_logic_vector(7 downto 0);                    -- export
			quadrature_encoder_0_input_in           : in  std_logic_vector(1 downto 0) := (others => 'X'); -- in
			quadrature_encoder_1_input_in           : in  std_logic_vector(1 downto 0) := (others => 'X'); -- in
			reset_reset_n                           : in  std_logic                    := 'X';             -- reset_n
			uart_0_external_connection_rxd          : in  std_logic                    := 'X';             -- rxd
			uart_0_external_connection_txd          : out std_logic                                        -- txd
		);
	end component quadrature_nios_pwm;

	u0 : component quadrature_nios_pwm
		port map (
			clk_clk                                 => CONNECTED_TO_clk_clk,                                 --                        clk.clk
			motor_pwm_0_motor_control_direction_out => CONNECTED_TO_motor_pwm_0_motor_control_direction_out, --  motor_pwm_0_motor_control.direction_out
			motor_pwm_0_motor_control_pwm_out       => CONNECTED_TO_motor_pwm_0_motor_control_pwm_out,       --                           .pwm_out
			motor_pwm_1_motor_control_direction_out => CONNECTED_TO_motor_pwm_1_motor_control_direction_out, --  motor_pwm_1_motor_control.direction_out
			motor_pwm_1_motor_control_pwm_out       => CONNECTED_TO_motor_pwm_1_motor_control_pwm_out,       --                           .pwm_out
			pio_0_external_connection_export        => CONNECTED_TO_pio_0_external_connection_export,        --  pio_0_external_connection.export
			quadrature_encoder_0_input_in           => CONNECTED_TO_quadrature_encoder_0_input_in,           -- quadrature_encoder_0_input.in
			quadrature_encoder_1_input_in           => CONNECTED_TO_quadrature_encoder_1_input_in,           -- quadrature_encoder_1_input.in
			reset_reset_n                           => CONNECTED_TO_reset_reset_n,                           --                      reset.reset_n
			uart_0_external_connection_rxd          => CONNECTED_TO_uart_0_external_connection_rxd,          -- uart_0_external_connection.rxd
			uart_0_external_connection_txd          => CONNECTED_TO_uart_0_external_connection_txd           --                           .txd
		);

