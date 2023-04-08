TOP = NutshellTop
BUILD_DIR = ./build
TOP_V = $(BUILD_DIR)/$(TOP).v
SCALA_FILE = $(shell find ./src/main/scala -name '*.scala')
TEST_FILE = $(shell find ./src/test/scala -name '*.scala')

SIMTOP = top.TopMain
IMAGE ?= ready-to-run/linux.bin

DATAWIDTH ?= 64
BOARD ?= sim  # sim  pynq  axu3cg
CORE  ?= inorder  # inorder  ooo  embedded


.DEFAULT_GOAL = verilog

help:
	mill chiselModule.runMain top.$(TOP) --help BOARD=$(BOARD) CORE=$(CORE)


verilog: $(TOP_V)

SIM_TOP = NutShellSimTop
SIM_TOP_V = $(BUILD_DIR)/$(SIM_TOP).v


$(SIM_TOP_V): $(SCALA_FILE) $(TEST_FILE)
	mkdir -p $(@D)
	mill chiselModule.test.runMain $(SIMTOP) -td $(@D) --output-file $(@F) BOARD=$(BOARD) CORE=$(CORE)

EMU_CSRC_DIR = $(abspath ./src/test/csrc)
EMU_VSRC_DIR = $(abspath ./src/test/vsrc)
EMU_CXXFILES = $(shell find $(EMU_CSRC_DIR) -name "*.cpp")
EMU_VFILES = $(shell find $(EMU_VSRC_DIR) -name "*.v" -or -name "*.sv")


# dump vcd: --debug --trace
# +define+RANDOMIZE_REG_INIT \
# +define+RANDOMIZE_MEM_INIT
VERILATOR_FLAGS = --top-module $(SIM_TOP) \
  +define+RV$(DATAWIDTH)=1 \
  +define+VERILATOR=1 \
  +define+PRINTF_COND=1 \
  --assert \
  --output-split 5000 \
  --output-split-cfuncs 5000 \
  -I$(abspath $(BUILD_DIR)) \
  --x-assign unique -O3 -CFLAGS "$(EMU_CXXFLAGS)" \
  -LDFLAGS "$(EMU_LDFLAGS)" \
  --trace

EMU_MK := $(BUILD_DIR)/emu-compile/V$(SIM_TOP).mk
EMU_DEPS := $(EMU_VFILES) $(EMU_CXXFILES)
EMU_HEADERS := $(shell find $(EMU_CSRC_DIR) -name "*.h")
EMU := $(BUILD_DIR)/emu


$(EMU_MK): $(SIM_TOP_V) | $(EMU_DEPS)
	@mkdir -p $(@D)
	verilator --cc --exe --trace $(VERILATOR_FLAGS) \
		-o $(abspath $(EMU)) -Mdir $(@D) $^ $(EMU_DEPS)

ifeq ($(USE_READY_TO_RUN_NEMU),true)
REF_SO := ./ready-to-run/riscv$(DATAWIDTH)-nemu-interpreter-so
else
REF_SO := $(NEMU_HOME)/build/riscv$(DATAWIDTH)-nemu-interpreter-so
$(REF_SO):
	$(MAKE) -C $(NEMU_HOME) ISA=riscv$(DATAWIDTH) SHARE=1
endif


$(EMU): $(EMU_MK) $(EMU_DEPS) $(EMU_HEADERS) $(REF_SO)
	CPPFLAGS=-DREF_SO=\\\"$(REF_SO)\\\" $(MAKE) VM_PARALLEL_BUILDS=1 -C $(dir $(EMU_MK)) -f $(abspath $(EMU_MK))

# SEED = -s $(shell seq 1 10000 | shuf | head -n 1)

#TODO: update log 
# log will only be printed when (LOG_BEGIN<=GTimer<=LOG_END) && (LOG_LEVEL < loglevel)
# use 'emu -h' to see more details
# LOG_BEGIN ?= 0
# LOG_END ?= 0
# LOG_LEVEL ?= ALL

# emu: $(EMU)
# 	@$(EMU) -i $(IMAGE) $(SEED) -b $(LOG_BEGIN) -e $(LOG_END) -v $(LOG_LEVEL)


clean:
	rm -rf $(BUILD_DIR)

.PHONY: verilog emu clean help $(REF_SO)