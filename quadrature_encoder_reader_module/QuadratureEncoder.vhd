library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity QuadratureEncoder is 
	generic
	(
		-- Time after which a value is latched.
		debounce_cycles : integer := 2 -- 50 MHz clock has a period of 20ns.
	);
	
	port
	(
		clk	:  in std_logic;
		n_rst	:	in std_logic;
		gpio	:	in std_logic_vector(1 downto 0);
		count	:	out std_logic_vector(31 downto 0) := (others => '0')
	);
end QuadratureEncoder;

architecture behaviour of QuadratureEncoder is

	-- State 0 = "00", State 1 = "01", State 2 = "10", State 3 = "11"
	signal input_state: std_logic_vector(1 DOWNTO 0) := "00";
	-- Direction type
	type direction_type IS (clockwise, counterclockwise, idle);
	signal direction : direction_type := idle;
  
	begin
	
	-- Input debouncer.
	process (clk, n_rst, input_state)

		variable cycle_counter : integer range 1 to debounce_cycles - 1; -- minus one cycle for latching.
		variable input_latch : std_logic_vector(1 downto 0);	-- Variable which stores the input.
		
		begin
		
			if (n_rst = '0') then
			
				cycle_counter := 1;
			
			elsif rising_edge(clk) then
				
				if (gpio = input_latch) then
				
					if cycle_counter = debounce_cycles - 1 then
					
						input_state <= input_latch;
					
					else 
						
						cycle_counter := cycle_counter + 1;
					
					end if;
				
				else 
					-- New input, latch it and reset counter.
					cycle_counter := 1;
					input_latch := gpio;
					
				end if;
				
			end if;
	
	end process;
	
	-- Signal decoder.
	process (n_rst, input_state)
	
	
	variable tick_counter : signed(31 downto 0)   := (others => '0');
	variable prev_input_state: std_logic_vector(1 DOWNTO 0) := "00";
		
		begin
	
			if (n_rst = '0') then
			
				count <= (others => '0');
				tick_counter := (others => '0');
			
			elsif (input_state /= prev_input_state) then
				-- State has changed!
				
				if prev_input_state = "00" then 
					
					if input_state = "01" then 
						direction <= counterclockwise;
						prev_input_state := input_state;
					end if;
					
					if input_state = "10" then 
						direction <= clockwise;
						prev_input_state := input_state;
					end if;
				
				end if;
				
				
				if direction = clockwise then 
				
					if input_state = "11" and prev_input_state = "10" then 
						
						prev_input_state := input_state;
					
					elsif input_state = "01" and prev_input_state = "11" then
						
						prev_input_state := input_state;
					
					elsif input_state = "00" and prev_input_state = "01" then 
					
						-- Increment!
						tick_counter := tick_counter + 1;
						prev_input_state := input_state;
						direction <= idle;
					
					else 
					
						prev_input_state := input_state;
						direction <= idle;
				
					end if;
				
				end if;
				
				if direction = counterclockwise then
				
					if input_state = "11" and prev_input_state = "01" then 
						
						prev_input_state := input_state;
					
					elsif input_state = "10" and prev_input_state = "11" then
						
						prev_input_state := input_state;
					
					elsif input_state = "00" and prev_input_state = "10" then 
					
						-- Decrement!
						tick_counter := tick_counter - 1;
						prev_input_state := input_state;
						direction <= idle;
					
					else 
					
						prev_input_state := input_state;
						direction <= idle;
				
					end if;
				
				end if;
							
			end if;
			
			count <= std_logic_vector(tick_counter);
			
	end process;

end behaviour;