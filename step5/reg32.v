module reg32 (
    input clk, reset,
    input [31:0] D,
    input [3:0] byteenable,
    output reg [31:0] Q
);

    always @(posedge clk) begin
        if (reset) Q <= 32'b0;
        else if (byteenable == 4'd1) Q <= D;
    end

    
endmodule