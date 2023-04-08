#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "macro.h"

#ifndef REF_SO
# error Please define REF_SO to the path of NEMU shared object file
#endif


void (*ref_difftest_memcpy_from_dut)(paddr_t dest, void *src, size_t n) = NULL;
void (*ref_difftest_getregs)(void *c) = NULL;
void (*ref_difftest_setregs)(const void *c) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;
void (*ref_isa_reg_display)(void) = NULL;


static bool is_skip_ref;
static bool is_skip_dut;


void difftest_skip_ref() {
    is_skip_ref = true;
}

// this is used to deal with instruction packing in QEMU.
// Sometimes letting QEMU step once will execute multiple instructions.
// We should skip checking until NEMU's pc catches up with QEMU'
void difftest_skip_dut() {
    if (is_skip_dut) return;


}


void init_difftest(rtlreg_t *reg) {
  void *handle;
  handle = dlopen(REF_SO, RTLD_LAZY | RTLD_DEEPBIND);
  assert(handle);

  ref_difftest_memcpy_from_dut = (void (*)(paddr_t, void *, size_t))dlsym(handle, "difftest_memcpy_from_dut");
  assert(ref_difftest_memcpy_from_dut);

  ref_difftest_getregs = (void (*)(void *))dlsym(handle, "difftest_getregs");
  assert(ref_difftest_getregs);

  ref_difftest_setregs = (void (*)(const void *))dlsym(handle, "difftest_setregs");
  assert(ref_difftest_setregs);

  ref_difftest_exec = (void (*)(uint64_t))dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = (void (*)(uint64_t))dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  ref_isa_reg_display = (void (*)(void))dlsym(handle, "isa_reg_display");
  assert(ref_isa_reg_display);

  void (*ref_difftest_init)(void) = (void (*)(void))dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  ref_difftest_init();
  void* get_img_start();
  long get_img_size();
  ref_difftest_memcpy_from_dut(0x80000000, get_img_start(), get_img_size());
  ref_difftest_setregs(reg);
}


static const char *reg_name[DIFFTEST_NR_REG] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
  "this_pc"
};


int difftest_step(rtlreg_t *reg_scala, uint32_t this_inst,
  int isMMIO, int isRVC, int isRVC2, uint64_t intrNO, int priviledgeMode, 
  int isMultiCommit
  ) {
    // Note:
    // reg_scala[DIFFTEST_THIS_PC] is the first PC commited by CPU-WB
    // ref_r[DIFFTEST_THIS_PC] is NEMU's next PC
    // To skip the compare of an instruction, replace NEMU reg value with CPU's regfile value,
    // then set NEMU's PC to next PC to be run   

    #define DEBUG_RETIRE_TRACE_SIZE 16

    rtlreg_t ref_r[DIFFTEST_NR_REG];
    rtlreg_t this_pc = reg_scala[DIFFTEST_THIS_PC];    

    // ref_difftest_getregs() will get the next pc,
    // therefore we must keep track this one    
    static rtlreg_t nemu_this_pc = 0x80000000;
    static rtlreg_t pc_retire_queue[DEBUG_RETIRE_TRACE_SIZE] = {0};
    static uint32_t inst_retire_queue[DEBUG_RETIRE_TRACE_SIZE] = {0};
    static uint32_t multi_commit_queue[DEBUG_RETIRE_TRACE_SIZE] = {0};
    static uint32_t skip_queue[DEBUG_RETIRE_TRACE_SIZE] = {0};
    static int pc_retire_pointer = 7;
    static int need_copy_pc = 0; //TODO:

    

}