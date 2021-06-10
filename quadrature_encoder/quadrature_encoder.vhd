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

begin
	
	-- Signal decoder.
	process (clk, n_rst, enc_in)
		

	variable counter : signed(31 downto 0) := (others => '0');
	variable prev_input_state: std_logic_vector(1 DOWNTO 0) := enc_in;
		
	begin
	
			if (n_rst = '0') then
			
				counter := (others => '0');
				prev_input_state := enc_in;
			
			-- Only enter this if statement if the input state has changed.
			elsif rising_edge(clk) and enc_in /= prev_input_state then
			
				case prev_input_state is
				
					when "11" => 
					
						if enc_in = "10" then 
						
							counter := counter - 1;
						
						end if;
						
						if enc_in = "01" then 
						
							counter := counter + 1;
						
						end if;
					
					when "01" => 
					
						if enc_in = "11" then 
						
							counter := counter - 1;
						
						end if;
						
						if enc_in = "00" then 
						
							counter := counter + 1;
						
						end if;
					
					when "10" =>
					
						if enc_in = "00" then 
						
							counter := counter - 1;
						
						end if;
						
						if enc_in = "11" then 
						
							counter := counter + 1;
						
						end if;
					
					when "00" =>
					
						if enc_in = "01" then 
						
							counter := counter - 1;
						
						end if;
						
						if enc_in = "10" then 
						
							counter := counter + 1;
						
						end if;
					when others => 
						
						ASSERT false REPORT "previous state not known" SEVERITY note;
				end case;
			
				
				prev_input_state := enc_in;
							
			end if;
			
			-- Route the counter to the output.
			count <= std_logic_vector(counter);
			
	end process;

end behaviour;
