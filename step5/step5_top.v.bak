module step4_top (
    ////////////////// CLK ///////////////////
    input wire CLOCK_50,
    ////////////////// KEY ///////////////////
    input wire [3:0] KEY,
    ////////////////// SW ////////////////////
    input wire [9:0] SW,
    ////////////////// LEDR //////////////////
    output reg [9:0] LEDR,
    ////////////////// SSEG //////////////////
    output reg [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5
);

    // qsys instantiation
    step4 qsys(
        .clk_clk(CLOCK_50),
        .reset_reset_n(KEY[0]),
        .hex0_export(HEX0),
        .hex1_export(HEX1),
        .hex2_export(HEX2),
        .hex3_export(HEX3),
        .hex4_export(HEX4),
        .hex5_export(HEX5),
        .ledr_export(LEDR),
        .sw_export(SW),
    );


endmodule
