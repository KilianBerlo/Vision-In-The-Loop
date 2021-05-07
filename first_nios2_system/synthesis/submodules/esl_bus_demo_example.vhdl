-------------------------------------------------------------------------------
-- 
-- ESL demo
-- Version: 1.0
-- Creator: Rene Moll
-- Date: 10th April 2012
--
-------------------------------------------------------------------------------
--
-- Countdown demo IP
-- A given value is counted down to zero with a frequency of 1 Hz
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity esl_bus_demo_example is
	generic (
		DATA_WIDTH : natural := 8
	);
	port(
		clk        : in  std_logic;
		rst        : in  std_logic;
		input      : in  std_logic_vector(DATA_WIDTH-1 downto 0);
		cnt_enable : in  std_logic;
		output     : out std_logic_vector(DATA_WIDTH-1 downto 0)
	);
end esl_bus_demo_example;

architecture behaviour of esl_bus_demo_example is
	constant residual		: natural := 17108864;
	signal   counter		: std_logic_vector(25 downto 0);
	signal   counter_overflow	: std_logic;
	signal   count_down_input	: std_logic_vector(DATA_WIDTH-1 downto 0);
	signal   count_down		: std_logic_vector(DATA_WIDTH-1 downto 0);
begin
	--
	-- Create a counter to generate the desired frequency from the main clock source
	--
	p_count : process(rst, clk)
	begin
		if (rst = '1') then
			counter <= std_logic_vector(to_unsigned(residual, counter'length));
			counter_overflow <= '0';
		elsif (rising_edge(clk)) then
			if (cnt_enable = '1') then
				if (unsigned(counter) = 0) then
					counter_overflow <= '1';
					counter <= std_logic_vector(to_unsigned(residual, counter'length));
				else
					counter <= std_logic_vector(unsigned(counter) + 1);
					counter_overflow <= '0';
				end if;
			end if;
		end if;
	end process;

	--
	-- Count down logic
	--
	p_count_down : process(rst, clk)
	begin
		if (rst = '1') then
			count_down       <= (others => '1');
			count_down_input <= (others => '0');
		elsif (rising_edge(clk)) then
			if (cnt_enable = '1') then
				-- Check for new input
				if (input /= count_down_input) then
					count_down_input <= input;
					count_down       <= input;
				end if;
			
				-- Check if the count down has reached zero
				if (unsigned(count_down) = 0) then
					count_down <= count_down_input;
				end if;
			
				-- Enable count down only if the counter is reset.
				if (counter_overflow = '1') then
					count_down <= std_logic_vector(unsigned(count_down) - 1);
				end if;
			end if;
		end if;
	end process;
	
	-- Route the counter directly to the output
	output <= count_down;
end architecture;
