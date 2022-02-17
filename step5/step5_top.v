module step5_top (
    ////////////////// CLK ///////////////////
    input wire CLOCK_50,
    ////////////////// KEY ///////////////////
    input wire [3:0] KEY,
    ////////////////// SW ////////////////////
    input wire [9:0] SW,
    ////////////////// LEDR //////////////////
    output reg [9:0] LEDR,
    ////////////////// SSEG //////////////////
    output wire [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5
);

    wire [31:0] prime;

    // qsys instantiation
    step5 u0 (
        .clk_clk               (CLOCK_50),       //        clk.clk
        .ledr_export           (LEDR),           //        ledr.export
        .prime_number_readdata (prime),          //        prime_number.readdata
        .reset_reset_n         (KEY[0])          //        reset.reset_n
    );

    wire [3:0] digit0, digit1, digit2, digit3, digit4, digit5;
	 wire done;
    getLast6Digit getDigits(
        .clk(CLOCK_50),
        .value(prime[31:1]),
        .digit0(digit0),
        .digit1(digit1),
        .digit2(digit2),
        .digit3(digit3),
        .digit4(digit4),
        .digit5(digit5),
        .done(done)
    );
	 
	assign prime[0] = done;

    ssegDecoder hex0(.value(digit0), .hex_out(HEX0));
    ssegDecoder hex1(.value(digit1), .hex_out(HEX1));
    ssegDecoder hex2(.value(digit2), .hex_out(HEX2));
    ssegDecoder hex3(.value(digit3), .hex_out(HEX3));
    ssegDecoder hex4(.value(digit4), .hex_out(HEX4));
    ssegDecoder hex5(.value(digit5), .hex_out(HEX5));


endmodule
