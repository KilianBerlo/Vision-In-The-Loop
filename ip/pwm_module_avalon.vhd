library ieee;
use ieee.std_logic_1164.all;

entity pwm_module_avalon is
	generic (
		DATA_WIDTH : natural := 32;		-- word size of the output register
		DUTY : natural := 16		-- word size of the output register
	);
	port (
		-- signals to connect to an Avalon clock source interface
		clk	: in std_logic;
		reset	: in std_logic;

		-- signals to connect to Avalon-MM slave interface 1
		slave_address_1	: in std_logic_vector(7 downto 0);
		slave_write_1	: in std_logic;
		slave_writedata_1 : in std_logic_vector(DATA_WIDTH-1 downto 0);

		-- signals to connect to Avalon-MM slave interface 2
		slave_address_2	: in std_logic_vector(7 downto 0);
		slave_write_2	: in std_logic;
		slave_writedata_2	: in std_logic_vector(DATA_WIDTH-1 downto 0);

		-- pwm_module outputs
		pwm_direction : out std_logic_vector(1 downto 0);
		pwm_out : out std_logic
	);

end entity;

architecture behaviour of pwm_module_avalon is

	signal mem1 : std_logic_vector(DATA_WIDTH - 1 downto 0) := (others => '0');
	signal mem2 : std_logic_vector(DATA_WIDTH - 1 downto 0) := (others => '0');

	component pwm_module
		port
		(
			clk : in std_logic;
			n_rst :	in std_logic;
			period_cc : in std_logic_vector(DATA_WIDTH-1 downto 0);
			duty_cycle : in std_logic_vector(DUTY-1 downto 0);
			direction_given : in std_logic_vector(1 downto 0);
			enable : in std_logic;
			dir : out std_logic_vector(1 downto 0);
			pwm_out : out std_logic
		);
	end component;

begin

	my_ip : pwm_module
	port map(
		clk => clk,
		n_rst => reset,
		period_cc => mem1,
		duty_cycle => mem2(DATA_WIDTH-1 downto DUTY),
		direction_given => mem2(DUTY-1 downto DUTY-2),
		enable => mem2(DUTY-3),
		dir => pwm_direction,
		pwm_out => pwm_out
	);
	
		-- Communication with the bus
	p_avalon : process(clk, reset)
	begin
		if (reset = '0') then
			mem1 <= (others => '0');
			mem2 <= (others => '0');
		elsif (rising_edge(clk)) then
			
			if (slave_write_1 = '1') then
				mem1 <= slave_writedata_1;
			end if;
			
			if (slave_write_2 = '1') then
				mem2 <= slave_writedata_2;
			end if;
			
		end if;
	end process;

end architecture;
