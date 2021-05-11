library ieee;
use ieee.std_logic_1164.all;
library work;

entity esl_demonstrator is
	port (
	-- CLOCK
	CLOCK_50	: in std_logic;
	n_rst	:	in std_logic;
	
	-- LEDs are only available on the DE0 Nano board.
	LED		: out std_logic_vector(7 downto 0);
	KEY		: in std_logic_vector(1 downto 0)

	-- GPIO_0, GPIO_0 connect to GPIO Default
	--GPIO_0		: inout std_logic_vector(31 downto 0);
	--GPIO_0_IN	: in    std_logic_vector(1 downto 0)
	);
end entity;


architecture behavior of esl_demonstrator is
	signal enable : std_logic;
	
	component QuadratureEncoder is 
	port
	(
		clk	:  in std_logic;
		n_rst	:  in std_logic;
		gpio	:  in std_logic_vector(1 downto 0);
		count	:  out std_logic_vector(7 downto 0)
	);
	end component;
begin

	encoder : entity work.QuadratureEncoder
		port map
		(
			clk 		=>	CLOCK_50,
			n_rst 		=>	n_rst,
			gpio		=> 	KEY,
			count		=> 	LED
		);

	
--	pwm : entity work.PulseWidthModulator
--		port map
--		(
--			-- Map your pulse width modulator here to the I/O
--		);

end architecture;
