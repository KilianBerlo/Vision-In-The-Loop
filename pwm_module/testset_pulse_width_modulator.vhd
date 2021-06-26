LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY testset_pulse_width_modulator IS
  PORT (
       CLOCK_50: OUT std_logic;
       NOTRESET: OUT std_logic;
       PERIOD_CC: OUT std_logic_vector(31 DOWNTO 0);
       DUTY_CYCLE: OUT std_logic_vector(15 DOWNTO 0);
       DIRECTION_GIVEN: OUT std_logic_vector(1 DOWNTO 0);
       ENABLE: OUT std_logic
       );
END testset_pulse_width_modulator;

ARCHITECTURE testset_pulse_width_modulator_arch OF testset_pulse_width_modulator IS
BEGIN
  clock : PROCESS
  BEGIN
    CLOCK_50 <= '0';
    WAIT FOR 10 ns;
    CLOCK_50 <= '1';
    WAIT FOR 10 ns;
  END PROCESS clock;

  always : PROCESS
  BEGIN
    PERIOD_CC <= "00000000000000000000000000000101";
    DUTY_CYCLE <= "0000000000000011";
    DIRECTION_GIVEN <= "11";
    WAIT FOR 20 ns;
    ENABLE <= '1';
    WAIT FOR 500 ns;
    PERIOD_CC <= "00000000000000000000000000000101";
    DUTY_CYCLE <= "0000000000000000";
    DIRECTION_GIVEN <= "01";  
    WAIT FOR 500 ns;
    PERIOD_CC <= "00000000000000000000000000000011";
    DUTY_CYCLE <= "0000000000000001";
    DIRECTION_GIVEN <= "00";  
    WAIT FOR 500 ns;
    PERIOD_CC <= "00000000000000000000000000000011";
    DUTY_CYCLE <= "0000000000000011";
    DIRECTION_GIVEN <= "10";
    WAIT;
  END PROCESS always;

END testset_pulse_width_modulator_arch;
