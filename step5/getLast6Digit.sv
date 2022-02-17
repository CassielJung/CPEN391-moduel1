module getLast6Digit (
    input clk,
    input [30:0] value,
    output done,
    output [3:0] digit0, digit1, digit2, digit3, digit4, digit5
);

    parameter idle = 4'd6;
    parameter getDigit0 = 4'd0;
    parameter getDigit1 = 4'd1;
    parameter getDigit2 = 4'd2;
    parameter getDigit3 = 4'd3;
    parameter getDigit4 = 4'd4;
    parameter getDigit5 = 4'd5;


    reg done_tmp = 0;
    reg [3:0] state = 4'd11;
    reg [3:0] digit0_tmp, digit1_tmp, digit2_tmp, digit3_tmp, digit4_tmp, digit5_tmp;
    reg [30:0] val;
    
    always_ff @(posedge clk) begin
        case (state)
            idle: begin
                val <= value;
                done_tmp <= 1'b0;
                state <= getDigit0;
            end
            getDigit0: begin
                digit0_tmp <= val % 10;
                state <= getDigit1;
                val <= val/10;
            end
            getDigit1: begin
                digit1_tmp <= val % 10;
                state <= getDigit2;
                val <= val/10;
            end
            getDigit2: begin
                digit2_tmp <= val % 10;
                state <= getDigit3;
                val <= val/10;
            end
            getDigit3: begin
                digit3_tmp <= val % 10;
                state <= getDigit4;
                val <= val/10;
            end
            getDigit4: begin
                digit4_tmp <= val % 10;
                state <= getDigit5;
                val <= val/10;
            end
            getDigit5: begin
                digit5_tmp <= val % 10;
                state <= idle;
                val <= val/10;
                done_tmp <= 1'b1;
            end
            default: begin state <= state; val <= val; end
        endcase
    end

    assign digit0 = digit0_tmp;
    assign digit1 = digit1_tmp;
    assign digit2 = digit2_tmp;
    assign digit3 = digit3_tmp;
    assign digit4 = digit4_tmp;
    assign digit5 = digit5_tmp;

    assign done = done_tmp;
    

endmodule
