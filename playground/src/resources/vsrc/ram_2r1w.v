/***************************************************************************************
* Copyright (c) 2020-2021 Institute of Computing Technology, Chinese Academy of Sciences
* Copyright (c) 2020-2021 Peng Cheng Laboratory
*
* XiangShan is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

// ref: https://github.com/OSCPU/ysyx/issues/8
// ref: https://github.com/OSCPU/ysyx/issues/9
// ref: https://github.com/OpenXiangShan/difftest/blob/master/doc/usage.md

module ram_2r1w 
#
(
    parameter WIDTH = 64;
)
(
    input clk,

    input imem_en,
    input [WIDTH-1:0] imem_addr,
    input [WIDTH-1:0] imem_data,

    input dmem_en,
    input [WIDTH-1:0] dmem_addr,
    input [WIDTH-1:0] dmem_rdata,
    input [WIDTH-1:0] dmem_wddr,
    input [WIDTH-1:0] dmem_wmask,
    input dmem_wen
);
    
    wire [WIDH-1:0] imem_data_0 = ram_read_helper(imem_en, {3'b000, (imem_addr - 64'h0000_0000_8000_0000) >> 3});
    
    // addr[2] = 1 -> 高32位
    assign imem_data = {32'b0000_0000_0000_0000, (imem_addr[2] ? imem_data_0[63:32], imem_data_0[31:0])};

    assign dmem_data = ram_read_helper(dmem_en,  {3'b000, (dmem_addr - 64'h0000_0000_8000_0000) >> 3});

    always @(posedge clk) begin
        ram_write_helper({3'b000, (dmem_addr - 64'h0000_0000_8000_0000) >> 3}, dmem_wdata, dmem_wmask, dmem_en & dmem_wen);
    end

endmodule