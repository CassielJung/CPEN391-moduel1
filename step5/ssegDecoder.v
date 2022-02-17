module ssegDecoder (
    input [3:0] value,
    output [6:0] hex_out
);

    reg [6:0] temp;

    always @*
        case (value)
        4'h0: temp = 7'b0111111;
        4'h1: temp = 7'b0000110;
        4'h2: temp = 7'b1011011;
        4'h3: temp = 7'b1001111;
        4'h4: temp = 7'b1100110;
        4'h5: temp = 7'b1101101;
        4'h6: temp = 7'b1111101;
        4'h7: temp = 7'b0000111;
        4'h8: temp = 7'b1111111;
        4'h9: temp = 7'b1101111;
        4'hA: temp = 7'b1110111;
        4'hB: temp = 7'b1111100;
        4'hC: temp = 7'b0111001;
        4'hD: temp = 7'b1011110;
        4'hE: temp = 7'b1111001;
        4'hF: temp = 7'b1110001;
        endcase
	assign hex_out = ~temp;
    
endmodule