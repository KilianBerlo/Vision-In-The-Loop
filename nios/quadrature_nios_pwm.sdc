#Update -period with clock period (in nanoseconds) of the clock driving the fpga
create_clock -period 20 [get_ports CLOCK_50]