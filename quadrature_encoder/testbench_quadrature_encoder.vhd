LIBRARY ieee;                                               
USE ieee.std_logic_1164.all; 

ENTITY testbench_quadrature_encoder IS
END testbench_quadrature_encoder;

ARCHITECTURE testbench_quadrature_encoder_arch OF testbench_quadrature_encoder IS

COMPONENT testset_quadrature_encoder
  PORT (
       CLOCK_50, NOTRESET: OUT std_logic;
       STATE: OUT std_logic_vector(1 DOWNTO 0)
       );
END COMPONENT;

COMPONENT quadrature_encoder
  PORT (
       clk: IN std_logic;
       n_rst: IN std_logic;
       enc_in: IN std_logic_vector(1 DOWNTO 0);
       count: INOUT std_logic_vector(31 DOWNTO 0)
       );
END COMPONENT;

SIGNAL clocksignal, notresetsignal: std_logic;
SIGNAL statesignal: std_logic_vector(1 DOWNTO 0);
SIGNAL countsignal: std_logic_vector(31 DOWNTO 0);

BEGIN
	test : testset_quadrature_encoder
	PORT MAP (
	         CLOCK_50 => clocksignal,
	         NOTRESET => notresetsignal,
	         STATE => statesignal
	         );

	qe : quadrature_encoder
	PORT MAP (
	         clk => clocksignal,
	         n_rst => notresetsignal,
	         enc_in => statesignal,
	         count => countsignal
	         );                           
END testbench_quadrature_encoder_arch;

CONFIGURATION conf_testbench_quadrature_encoder OF testbench_quadrature_encoder IS
	FOR testbench_quadrature_encoder_arch
		FOR test : testset_quadrature_encoder USE ENTITY work.testset_quadrature_encoder(testset_quadrature_encoder_arch);
		END FOR;
		FOR qe : quadrature_encoder USE ENTITY work.quadrature_encoder(behaviour);
		END FOR;
	END FOR;
END conf_testbench_quadrature_encoder;