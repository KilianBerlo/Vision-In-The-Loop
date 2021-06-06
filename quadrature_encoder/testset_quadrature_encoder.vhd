LIBRARY ieee;                                               
USE ieee.std_logic_1164.all; 

ENTITY testset_quadrature_encoder IS
  PORT (
       CLOCK_50, NOTRESET: OUT std_logic;
       STATE: OUT std_logic_vector(1 DOWNTO 0)
       );
END testset_quadrature_encoder;

ARCHITECTURE testset_quadrature_encoder_arch OF testset_quadrature_encoder IS
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
    FOR i IN 0 TO 10 LOOP
      WAIT FOR 20 ns;   
      STATE <= "11";     
      WAIT FOR 40 ns;  
      STATE <= "01";   
      WAIT FOR 40 ns;  
      STATE <= "00";   
      WAIT FOR 40 ns;  
      STATE <= "10";   
      WAIT FOR 20 ns;  
    END LOOP;     
    FOR i IN 0 TO 20 LOOP
      WAIT FOR 20 ns;   
      STATE <= "00";     
      WAIT FOR 40 ns;  
      STATE <= "01";   
      WAIT FOR 40 ns;  
      STATE <= "11";   
      WAIT FOR 40 ns;  
      STATE <= "10";   
      WAIT FOR 20 ns;  
    END LOOP;  
    FOR i IN 0 TO 10 LOOP
      WAIT FOR 20 ns;   
      STATE <= "11";     
      WAIT FOR 40 ns;  
      STATE <= "01";   
      WAIT FOR 40 ns;  
      STATE <= "00";   
      WAIT FOR 40 ns;  
      STATE <= "10";   
      WAIT FOR 20 ns;  
    END LOOP;          
  END PROCESS always; 
                                         
END testset_quadrature_encoder_arch;