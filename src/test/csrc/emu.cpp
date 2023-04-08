#include "emu.h"
#include 
#include <getopt.h>


void Emulator::reset_ncycles(size_t cycles) {

   for (int i=0; i<cycles; i++) {
      dut_ptr->reset = 1;
      dut_ptr->clock = 0;
      dut_ptr->eval();
      dut_ptr->clock = 1;
      dut_ptr->eval();
      dut_ptr->reset = 0;
   }
}


void Emulator::single_cycle() {
   dut_ptr->clock = 0;
   dut_ptr->eval();
   dut_ptr->clock = 1;
   dut_ptr->eval();
   cycles++;
}


void Emulator::execute_cycles(uint64_t n) {
   extern bool is_finshed();
   uint32_t lasttime = 0;
   uint64_t lastcommit = n;
   int hascommit = 0;
   const int struck_limit = 2000;

   while (!is_finshed() && n > 0) {
      single_cycle();
      n--;


   }
}


void Emulator::execute() {
   execute_cycles(max_cycles);
}


void Emulator::read_emu_regs(rtlreg_t *r) {
   
}