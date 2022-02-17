module reg32_avalon_interface (
    input clk, reset, write, read, chipselect,
    input [3:0] byteenable,
    input [31:0] writedata,
    output [31:0] readdata,
    output [31:0] Q_export
);

    wire [31:0] to_reg;
    reg [31:0] from_reg;
    wire [3:0] local_byteenable;

    assign to_reg = writedata;
    assign local_byteenable = (chipselect & write) ? byteenable : 4'd0;
					  
	 always @(posedge clk) begin
        if (!reset) from_reg <= 32'b0;
        else if (local_byteenable[3]|local_byteenable[2]|local_byteenable[1]|local_byteenable[0]) from_reg <= to_reg;
    end

    assign readdata = from_reg;
    assign Q_export = from_reg;
    
endmodule