library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity quadrature_encoder is 
	port
	(
		clk	:  in std_logic;
		n_rst	:	in std_logic;
		enc_in:	in std_logic_vector(1 downto 0);
		count	:	inout std_logic_vector(31 downto 0)
	);
end quadrature_encoder;

architecture behaviour of quadrature_encoder is

signal prev_input_state: std_logic_vector(1 DOWNTO 0) := "11";

begin
	
	-- Signal decoder.
	process (clk, n_rst, enc_in, prev_input_state)
	
	

	-- Direction type
	type direction_type is (clockwise, counterclockwise, idle);
	variable direction : direction_type := idle;
	
	

	variable tick_counter : signed(31 downto 0)   := (others => '0');
		
	begin
	
			if (n_rst = '0') then
			
				tick_counter := (others => '0');
				prev_input_state <= "11";
				direction := idle;
			
			-- Only enter this if statement if the input state has changed.
			elsif rising_edge(clk) and enc_in /= prev_input_state then
				
				if enc_in = "01" and prev_input_state = "00" then 
					
					tick_counter := tick_counter + 1;
				
				elsif enc_in = "10" and prev_input_state = "00" then 
					
					tick_counter := tick_counter - 1;
				
				end if;
				
				prev_input_state <= enc_in;
							
			end if;
			
			-- Route the counter to the output.
			count <= std_logic_vector(tick_counter);
			
	end process;

end behaviour;