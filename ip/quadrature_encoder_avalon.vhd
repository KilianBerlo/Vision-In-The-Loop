library ieee;
use ieee.std_logic_1164.all;

entity quadrature_encoder_avalon is
	generic (
		DATA_WIDTH : natural := 32		-- word size of the output register
	);
	port (
		-- signals to connect to an Avalon clock source interface
		clk			: in  std_logic;
		reset			: in  std_logic;

		-- signals to connect to an Avalon-MM slave interface
		slave_address		: in  std_logic_vector(7 downto 0);
		slave_read			: in  std_logic;
		slave_readdata		: out std_logic_vector(DATA_WIDTH-1 downto 0);
		
		quadrature_encoder_input :	in std_logic_vector(1 downto 0)
	);
	
end entity;

architecture behaviour of quadrature_encoder_avalon is

	component quadrature_encoder 
		port
		(
			clk		:  in std_logic;
			n_rst		:	in std_logic;
			enc_in	:	in std_logic_vector(1 downto 0);
			count		:	out std_logic_vector(DATA_WIDTH-1 downto 0)
		);
	end component;
	
begin 

	my_ip : quadrature_encoder
	port map(
		clk    	=> clk,
		n_rst    => reset,
		enc_in  	=> quadrature_encoder_input,
		count 	=> slave_readdata
	);
	
end architecture;