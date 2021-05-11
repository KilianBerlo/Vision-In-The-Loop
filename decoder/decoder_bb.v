
module decoder (
	clk_clk,
	esl_encoder_0_avalon_slave_0_readdata,
	esl_encoder_0_avalon_slave_0_writebyteenable_n,
	reset_reset_n);	

	input		clk_clk;
	output	[7:0]	esl_encoder_0_avalon_slave_0_readdata;
	input	[1:0]	esl_encoder_0_avalon_slave_0_writebyteenable_n;
	input		reset_reset_n;
endmodule
