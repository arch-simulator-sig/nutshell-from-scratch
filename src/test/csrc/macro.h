#ifndef __MACRO_H__
#define __MACRO_H__

#include <stdint.h>
#include <assert.h>
#include <string.h>


typedef uint64_t rtlreg_t;
typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;

enum {
  DIFFTEST_THIS_PC = 32,
  DIFFTEST_MSTATUS,
  DIFFTEST_MCAUSE,
  DIFFTEST_MEPC,
  DIFFTEST_SSTATUS,
  DIFFTEST_SCAUSE,
  DIFFTEST_SEPC,
  DIFFTEST_NR_REG
};

enum {
  STATE_GOODTRAP = 0,
  STATE_BADTRAP,
  STATE_ABORT,
  STATE_RUNNING = -1
};

#endif