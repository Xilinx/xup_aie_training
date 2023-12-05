# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

# settings
PLATFORM := xilinx_vck5000_gen4x8_qdma_2_202220_1
TARGET := hw

ECHO := @echo

XPFM = $(shell platforminfo -p $(PLATFORM) --json="file")
XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))
MAX_CYCLES := 10000

# OUTPUT PRODUCTS 
BUILD_DIR := $(if $(filter x86sim,$(TARGET)),build.x86sim,build.hw)
WORK_DIR = work
SRC_DIR = $(shell readlink -f src/)
DATA_DIR = $(shell readlink -f data/)
AIE_LOG = ./aiecompiler.log
SIM_OUTPUT = ./aiesimulator_output

# DEPENDENCIES for make aie
GRAPH_CPP := $(SRC_DIR)/graph.cpp
DEPS := $(GRAPH_CPP)
DEPS += $(SRC_DIR)/aie_kernels.h
DEPS += $(SRC_DIR)/graph.h
DEPS += $(SRC_DIR)/aie_kernels/fir_16taps_symm.cpp
DEPS += $(SRC_DIR)
AIE_FLAGS += --platform=$(XPFM)

.PHONY: help clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make aieemu"
	$(ECHO) "      Run AIE simulator and generate outputs suitable for Vitis Analyzer"
	$(ECHO) ""
	$(ECHO) "  make x86sim"
	$(ECHO) "      Run x86 simulator"
	$(ECHO) ""
	$(ECHO) "  make analyzer"
	$(ECHO) "      Call the Vitis analyzer to show the AIE build summary and review waveform database output"
	$(ECHO) ""
	$(ECHO) "  make get_output"
	$(ECHO) "      Strip simulator output data from timestamps and copy to a new file"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: $(BUILD_DIR)/libadf.a

$(BUILD_DIR)/libadf.a: $(DEPS)
	@mkdir -p $(BUILD_DIR);
	cd $(BUILD_DIR); \
	aiecompiler -v --target=$(TARGET) \
		--stacksize=2000 \
		-include="$(XILINX_VITIS)/aietools/include" \
		-include="$(SRC_DIR)"  \
		-include="$(DATA_DIR)" \
		$(AIE_FLAGS) \
		$(GRAPH_CPP) \
		-workdir=$(WORK_DIR) 2>&1 | tee $(AIE_LOG)

aieemu:
	cd $(BUILD_DIR); \
	aiesimulator --pkg-dir=$(WORK_DIR) --i=.. --profile --online -wdb -ctf -o $(SIM_OUTPUT) --simulation-cycle-timeout $(MAX_CYCLES)

analyzer:
	cd $(BUILD_DIR); \
	vitis_analyzer -report ./work/graph.aiecompile_summary ./aiesimulator_output/default.aierun_summary

get_output:
	cd $(BUILD_DIR); \
	grep -v T ${SIM_OUTPUT}/data/output.txt > ../data/output_aie.txt

x86sim:
	cd $(BUILD_DIR); \
	x86simulator --pkg-dir=$(WORK_DIR) --i=..	

clean:
	rm -rf $(BUILD_DIR)
