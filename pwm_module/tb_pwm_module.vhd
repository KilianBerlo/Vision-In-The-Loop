-- Copyright (C) 2020  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions
-- and other software and tools, and any partner logic
-- functions, and any output files from any of the foregoing
-- (including device programming or simulation files), and any
-- associated documentation or information are expressly subject
-- to the terms and conditions of the Intel Program License
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details, at
-- https://fpgasoftware.intel.com/eula.

-- Generated on "04/30/2021 16:48:15"

-- Vhdl Test Bench template for design  :  esl_demonstrator
--
-- Simulation tool : ModelSim-Altera (VHDL)
--

LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY tb_pwm_module IS
END tb_pwm_module;
ARCHITECTURE tb_pwm_module_arch OF tb_pwm_module IS
-- constants
-- signals
SIGNAL clk : STD_LOGIC := '0';
SIGNAL n_rst : STD_LOGIC := '1';
SIGNAL period_cc : std_logic_vector(31 downto 0);
SIGNAL duty_cycle : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL direction_given : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL enable : STD_LOGIC;
SIGNAL dir : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL pwm_out : STD_LOGIC;
SIGNAL finished : boolean := false;

COMPONENT pwm_module
	PORT (
		clk	:  in std_logic;
		n_rst	:	in std_logic;
		period_cc:	in std_logic_vector(31 downto 0); -- in clock cycles
    duty_cycle : in std_logic_vector(15 downto 0); -- in clock cycles
    direction_given : in std_logic_vector(1 downto 0);
    enable : in std_logic;
    dir : out std_logic_vector(1 downto 0);
    pwm_out : out std_logic
	);
END COMPONENT;

BEGIN
	i1 : pwm_module
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	n_rst => n_rst,
	period_cc => period_cc,
	duty_cycle => duty_cycle,
	direction_given => direction_given,
	enable => enable,
	dir => dir,
	pwm_out => pwm_out
	);

clk <= NOT clk AFTER 10 ns WHEN NOT finished;

init : PROCESS
-- variable declarations
BEGIN
        -- code that executes only once
  --KEY <= "00";
  --WAIT FOR 5 ns;
  --KEY <= "11";
	period_cc <= "00000000000000000000000000000101";
	duty_cycle <= "0000000000000011";
	direction_given <= "11";
WAIT;
END PROCESS init;

always : PROCESS
-- optional sensitivity list
-- (        )
-- variable declarations
BEGIN

  wait for 200 ns;
	enable <= '1';
	wait for 1000 ns;
	enable <= '0';
	wait for 100 ns;
	enable <= '1';
	wait;

END PROCESS always;

END tb_pwm_module_arch;
