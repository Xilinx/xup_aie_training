# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

PLATFORM := xilinx_vck5000_gen4x8_qdma_2_202220_1
TARGET := hw
#TARGET := x86sim
FREQ := 250

#MAX_CYCLES AIE EMU max cycles
MAX_CYCLES := 10000

XPFM = $(shell platforminfo -p $(PLATFORM) --json="file")
XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))

# OUTPUT PRODUCTS 
BUILD_DIR := $(if $(filter x86sim,$(TARGET)),build.x86sim,build.hw)
WORK_DIR = work
SRC_DIR = $(shell readlink -f src/)
DATA_DIR = $(shell readlink -f data/)
CONSTRAINTS_DIR = $(shell readlink -f constraints/)
SIM_OUTPUT = $(shell readlink -f $(BUILD_DIR)/aiesimulator_output/)

# DEPENDENCIES for make aie
GRAPH_CPP := $(SRC_DIR)/graph.cpp 
DEPS := $(GRAPH_CPP)
DEPS += $(SRC_DIR)/graph.hpp
DEPS += $(SRC_DIR)/fir_stream.cc
DEPS += $(SRC_DIR)/fir_window.cc
DEPS += $(SRC_DIR)/aie_kernels.hpp
DEPS += $(SRC_DIR)/settings.h

# Add your own dependencies

AIE_FLAGS = --platform=$(XPFM)
AIE_FLAGS += --pl-freq=$(FREQ)
AIE_FLAGS += --dataflow
AIE_FLAGS += --Xchess="main:bridge.llibs=softfloat m"

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
	$(ECHO) "      Call the Vitis analyer to show the aie build summary and review waveform database output"
	$(ECHO) ""
	$(ECHO) "  make get_output"
	$(ECHO) "      Strip simulator output data from timestamps and copy to new file"
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
		-workdir=$(WORK_DIR) 2>&1 | tee aiecompiler.log

clean:
	rm -rf $(BUILD_DIR) *.log *.jou

aieemu:
	cd $(BUILD_DIR); \
	aiesimulator --pkg-dir=$(WORK_DIR) --i=.. --profile --dump-vcd=foo

analyzer:
	cd $(BUILD_DIR); \
	vitis_analyzer -report ./work/graph.aiecompile_summary ./aiesimulator_output/default.aierun_summary

get_output:
	cd $(BUILD_DIR); \
	grep -v T ${SIM_OUTPUT}/output.txt > ${DATA_DIR}/output_aie.txt
	
x86sim:
	cd $(BUILD_DIR); \
	x86simulator --pkg-dir=$(WORK_DIR) --i=..