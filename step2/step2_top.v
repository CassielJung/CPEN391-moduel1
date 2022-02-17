module step2_top (
	input wire CLOCK_50,
	input wire [3:0] KEY
);

	part1 qsys(.clk_clk(CLOCK_50), .reset_reset_n(KEY[0]));

endmodule
