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

ENTITY tb_esl_demonstrator IS
END tb_esl_demonstrator;
ARCHITECTURE tb_esl_demonstrator_arch OF tb_esl_demonstrator IS
-- constants                                                 
-- signals                                                   
SIGNAL CLOCK_50 : STD_LOGIC := '0';
SIGNAL GPIO_0 : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL GPIO_0_IN : STD_LOGIC_VECTOR(1 DOWNTO 0) := "00";
SIGNAL GPIO_1 : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL GPIO_1_IN : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL KEY : STD_LOGIC_VECTOR(1 DOWNTO 0) := "11";
SIGNAL LED : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL SW : STD_LOGIC_VECTOR(3 DOWNTO 0);
SIGNAL finished : boolean := false;

COMPONENT esl_demonstrator
	PORT (
	CLOCK_50 : IN STD_LOGIC;
	GPIO_0 : INOUT STD_LOGIC_VECTOR(31 DOWNTO 0);
	GPIO_0_IN : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
	GPIO_1 : INOUT STD_LOGIC_VECTOR(31 DOWNTO 0);
	GPIO_1_IN : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
	KEY : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
	LED : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	SW : IN STD_LOGIC_VECTOR(3 DOWNTO 0)
	);
END COMPONENT;

BEGIN
	i1 : esl_demonstrator
	PORT MAP (
-- list connections between master ports and signals
	CLOCK_50 => CLOCK_50,
	GPIO_0 => GPIO_0,
	GPIO_0_IN => GPIO_0_IN,
	GPIO_1 => GPIO_1,
	GPIO_1_IN => GPIO_1_IN,
	KEY => KEY,
	LED => LED,
	SW => SW
	);

CLOCK_50 <= NOT CLOCK_50 AFTER 10 ns WHEN NOT finished;

init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once  
  --KEY <= "00";
  --WAIT FOR 5 ns;
  --KEY <= "11";
WAIT;                                                       
END PROCESS init;   
                                        
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                               
BEGIN                                                         
        -- code executes for every event on sensitivity list  
  FOR i IN 0 TO 10 LOOP -- 0 to -11
    WAIT FOR 20 ns;   
    GPIO_0_IN <= "11";     
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "01";   
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "00";   
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "10";   
    WAIT FOR 20 ns;  
  END LOOP;     
  FOR i IN 0 TO 20 LOOP -- -11 to 10
    WAIT FOR 20 ns;   
    GPIO_0_IN <= "00";     
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "10";   
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "11";   
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "01";   
    WAIT FOR 20 ns;  
  END LOOP;  
  FOR i IN 0 TO 10 LOOP -- 10 back to 0
    WAIT FOR 20 ns;   
    GPIO_0_IN <= "00";     
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "01";   
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "11";   
    WAIT FOR 40 ns;  
    GPIO_0_IN <= "10";   
    WAIT FOR 20 ns;  
  END LOOP;          
END PROCESS always; 
                                         
END tb_esl_demonstrator_arch;