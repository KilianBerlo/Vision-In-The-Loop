library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity pwm_module is
	generic (
		DATA_WIDTH : natural := 32;		-- word size of the output register
  	DUTY : natural := 16		-- word size of the output register
	);
	port
	(
		clk	:  in std_logic;
		n_rst	:	in std_logic;
		period_cc:	in std_logic_vector(DATA_WIDTH-1 downto 0); -- in clock cycles
		duty_cycle : in std_logic_vector(DUTY-1 downto 0); -- in clock cycles
		direction_given : in std_logic_vector(1 downto 0);
		enable : in std_logic;
		dir : out std_logic_vector(1 downto 0);
		pwm_out : out std_logic
	);
end pwm_module;

architecture behaviour of pwm_module is

begin
	-- PWM module.
	process (clk, n_rst)

  variable cc_count : unsigned(DATA_WIDTH-1 downto 0) := (others => '0');
  variable signed_period_cc : unsigned(DATA_WIDTH-1 downto 0);
  variable signed_duty_cycle : unsigned(DUTY-1 downto 0);

	begin

    if(n_rst = '0') then

      cc_count := (others => '0');
      signed_period_cc := (others => '0');
      signed_duty_cycle := (others => '0');
		pwm_out <= '0';

  	elsif(rising_edge(clk)) then

      signed_period_cc := unsigned(period_cc);--x"000186a0";
      signed_duty_cycle := unsigned(duty_cycle);--x"C350";

      if(enable = '1') then

        if(cc_count < signed_period_cc-1) then

          if(cc_count < signed_duty_cycle-1) then -- this if case is not needed if waiting one period before starting the pwm signal is okay
            pwm_out <= '1';

          elsif(cc_count >= signed_duty_cycle-1) then
            pwm_out <= '0';

          else
            ASSERT false  REPORT "No such operation exists" SEVERITY failure; -- not synthesized, just for debugging
          end if;
          
          cc_count := cc_count + 1;

        elsif(cc_count >= signed_period_cc-1) then
          cc_count := (others => '0');
          pwm_out <= '1';

        else
          ASSERT false  REPORT "No such operation exists" SEVERITY failure; -- not synthesized, just for debugging
        end if;

      else 
			
			pwm_out <= '0';
		
		end if;
      dir <= direction_given;

    end if;

  end process;

end behaviour;
