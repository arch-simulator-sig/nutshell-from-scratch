`define RAMWIDTH 64
import "DPI-C" function void ram_read_helper
(
    input   longint rIdx, 
    output  longint rdata,
    input   longint wIdx,
    input   longint wdata,
    input   longint wmask,
    input   bit     wen
);


module RAMHelper(
    input   clk,
    input  [`RAMWIDTH-1:0] rIdx,
    output [`RAMWIDTH-1:0] rdata,
    input  [`RAMWIDTH-1:0] wIdx,
    input  [`RAMWIDTH-1:0] wdata,
    input  [`RAMWIDTH-1:0] wmask,
    input   wen
);

always @(posedge clk) begin
    ram_helper(rIdx, rdata, wIdx, wdata, wmask);
end

endmodule
