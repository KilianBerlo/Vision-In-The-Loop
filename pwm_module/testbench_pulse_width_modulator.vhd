LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY testbench_pulse_width_modulator IS
END testbench_pulse_width_modulator;

ARCHITECTURE testbench_pulse_width_modulator_arch OF testbench_pulse_width_modulator IS

COMPONENT testset_pulse_width_modulator
	PORT (
				CLOCK_50: OUT std_logic;
				NOTRESET: OUT std_logic;
				PERIOD_CC: OUT std_logic_vector(31 DOWNTO 0);
				DUTY_CYCLE: OUT std_logic_vector(15 DOWNTO 0);
				DIRECTION_GIVEN: OUT std_logic_vector(1 DOWNTO 0);
				ENABLE: OUT std_logic
			 );
END COMPONENT;

COMPONENT pwm_module
  PORT (
				clk: IN std_logic;
				n_rst: IN std_logic;
				period_cc: IN std_logic_vector(31 DOWNTO 0); -- in clock cycles
				duty_cycle: IN std_logic_vector(15 DOWNTO 0); -- in clock cycles
				direction_given: IN std_logic_vector(1 DOWNTO 0);
				enable: IN std_logic;
				dir: OUT std_logic_vector(1 DOWNTO 0);
				pwm_out: OUT std_logic
       );
END COMPONENT;

SIGNAL clocksignal, notresetsignal: std_logic;
SIGNAL periodsignal: std_logic_vector(31 DOWNTO 0);
SIGNAL dcsignal: std_logic_vector(15 DOWNTO 0);
SIGNAL givendirsignal: std_logic_vector(1 DOWNTO 0);
SIGNAL enablesignal: std_logic;
SIGNAL dirsignal: std_logic_vector(1 DOWNTO 0);
SIGNAL pwmsignal: std_logic;

BEGIN
	test : testset_pulse_width_modulator
	PORT MAP (
	         CLOCK_50 => clocksignal,
	         NOTRESET => notresetsignal,
	         PERIOD_CC => periodsignal,
 	         DUTY_CYCLE => dcsignal,
 	         DIRECTION_GIVEN => givendirsignal,
 	         ENABLE => enablesignal
	         );

	pwm : pwm_module
	PORT MAP (
	         clk => clocksignal,
	         n_rst => notresetsignal,
	         period_cc => periodsignal,
	         duty_cycle => dcsignal,
	         direction_given => givendirsignal,
	         enable => enablesignal,
	         dir => dirsignal,
	         pwm_out => pwmsignal
	         );
END testbench_pulse_width_modulator_arch;

CONFIGURATION conf_testbench_pulse_width_modulator OF testbench_pulse_width_modulator IS
	FOR testbench_pulse_width_modulator_arch
		FOR test : testset_pulse_width_modulator USE ENTITY work.testset_pulse_width_modulator(testset_pulse_width_modulator_arch);
		END FOR;
		FOR pwm : pwm_module USE ENTITY work.pwm_module(behaviour);
		END FOR;
	END FOR;
END conf_testbench_pulse_width_modulator;
