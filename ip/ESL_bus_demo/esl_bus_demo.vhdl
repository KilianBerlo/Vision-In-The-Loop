-------------------------------------------------------------------------------
-- 
-- ESL demo
-- Version: 1.0
-- Creator: Rene Moll
-- Date: 10th April 2012
--
-------------------------------------------------------------------------------
--
-- Straight forward initialization and mapping of an IP to the avalon bus.
-- The communication part is kept simple, since only register is of interest.
--
-- Communication signals use the prefix slave_
-- User signals use the prefix user_
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

entity esl_bus_demo is
	generic (
		DATA_WIDTH : natural := 32;	-- word size of each input and output register
		LED_WIDTH  : natural := 8	-- numbers of LEDs on the DE0-NANO
	);
	port (
		-- signals to connect to an Avalon clock source interface
		clk			: in  std_logic;
		reset			: in  std_logic;

		-- signals to connect to an Avalon-MM slave interface
		slave_address		: in  std_logic_vector(7 downto 0);
		slave_read		: in  std_logic;
		slave_write		: in  std_logic;
		slave_readdata		: out std_logic_vector(DATA_WIDTH-1 downto 0);
		slave_writedata		: in  std_logic_vector(DATA_WIDTH-1 downto 0);
		slave_byteenable	: in  std_logic_vector((DATA_WIDTH/8)-1 downto 0);

		-- signals to connect to custom user logic
		user_output		: out std_logic_vector(LED_WIDTH-1 downto 0)
	);
end entity;

architecture behavior of esl_bus_demo is
	-- Internal memory for the system and a subset for the IP
	signal enable : std_logic;
	signal mem        : std_logic_vector(31 downto 0);
	signal mem_masked : std_logic_vector(LED_WIDTH-1 downto 0);

	-- Definition of the counter
	component esl_bus_demo_example
	  generic (
		 DATA_WIDTH : natural := 8
	  );
	  port(
		 clk        : in  std_logic;
		 rst        : in  std_logic;
		 input	    : in  std_logic_vector(DATA_WIDTH-1 downto 0);
       cnt_enable : in std_logic;
		 output     : out std_logic_vector(DATA_WIDTH-1 downto 0)
	  );
	end component;
begin
	-- Initialization of the example
	my_ip : esl_bus_demo_example
	generic map(
		DATA_WIDTH => LED_WIDTH
	)
	port map(
		clk    => clk,
		rst    => reset,
		input  => mem_masked,
		cnt_enable => enable,
		output => user_output
	);

	-- Communication with the bus
	p_avalon : process(clk, reset)
	begin
		if (reset = '1') then
			mem <= (others => '0');
		elsif (rising_edge(clk)) then
			if (slave_read = '1') then
				slave_readdata <= mem;
			end if;
			
			if (slave_write = '1') then
				mem <= slave_writedata;
			end if;
		end if;
	end process;
	
	-- Only select the amount bits that the logic can handle
	mem_masked <= mem(LED_WIDTH-1 downto 0);
	enable <= mem(DATA_WIDTH-1);
end architecture;
