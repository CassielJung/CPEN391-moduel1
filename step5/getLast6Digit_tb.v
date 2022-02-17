module getLast6Digit_tb ();
    reg clk;
    reg [31:0] value;
    wire [3:0] digit0, digit1, digit2, digit3, digit4, digit5;

    getLast6Digit DUT (
        .clk(clk),
        .value(value),
        .digit0(digit0),
        .digit1(digit1),
        .digit2(digit2),
        .digit3(digit3),
        .digit4(digit4),
        .digit5(digit5)
    );

    initial forever begin 
        clk = 1'b0;
        #1;
        clk = 1'b1;
        #1;
    end

    initial begin
        value = 32'd543210; #30;
        value = 32'd987654; #30;
        value = 32'd1234567890; #30;
    end
endmodule