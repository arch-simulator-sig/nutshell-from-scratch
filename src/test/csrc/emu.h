#ifndef __EMU_H__
#define __EMU_H__

#include "macro.h"
#include <vector>
#include <unique_ptr.h>
#include "VNutShellSimTop.h"
#include "verilated.h"

#define VM_TRACE 0
#if VM_TRACE
#include <verilated_vcd_c.h>	// Trace file format header
#endif

class Emulator {
private:
    const char *img;
    std::unique_ptr<VNutShellSimTop> dut_ptr;
#if VM_TRACE
  VerilatedVcdC* tfp;
#endif

    // emu control
    uint32_t seed;
    uint64_t max_cycles, cycles;


public:

    void reset_ncycles(size_t cycles);
    void single_cycle();
    void execute_cycles(uint64_t n);
    void execute();
    void read_emu_regs(rtlreg_t *r);
    uint64_t get_cycles() const { return cycles; }
    uint64_t get_max_cycles() const { return max_cycles; }
    Emulator(int argc, const char *argv[]);
    ~Emulator() {};
};


#endif