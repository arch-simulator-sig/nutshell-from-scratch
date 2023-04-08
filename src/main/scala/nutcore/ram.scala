package nutcore

import chisel3._
import chisel3.util._
import chisel3.experimental._


class ram_2r1w extends BlackBox with HasBlackBoxResource {
    val io = IO(new Bundle{
        val clk = Input(Clock())
        val imem_addr = Input(0.U(64.W))
        val imem_data = Output(0.U(64.W))

        val dmem_en = Input(Bool())
        val dmem_waddr = Input(0.U(64.W))
        val dmem_wdata = Input(0.U(64.W))
        val dmem_wmask = Input(0.U(64.W))
    })

    // InlineVerilog 方法 前面要加 | 写起来不方便，算了
    addResource("vsrc/ram_2r1w.v")
}