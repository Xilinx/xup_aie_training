# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

PLATFORM := xilinx_vck5000_gen4x8_xdma_2_202210_1
TARGET := hw

XPFM = $(shell platforminfo -p $(PLATFORM) --json="file")
XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))

# OUTPUT PRODUCTS 
BUILD_DIR = build.$(TARGET)
WORK_DIR = work
SRC_DIR = $(shell readlink -f src/)
DATA_DIR = $(shell readlink -f data/)

# DEPENDENCIES for make aie
GRAPH_CPP := $(SRC_DIR)/nbody_x4_x100.cpp
DEPS := $(GRAPH_CPP)
DEPS += $(SRC_DIR)/globals.h
DEPS += $(SRC_DIR)/include.h
DEPS += $(SRC_DIR)/kernels.h
DEPS += $(SRC_DIR)/Makefile
DEPS += $(SRC_DIR)/nbody.cc
DEPS += $(SRC_DIR)/nbody_subsystem.h
DEPS += $(SRC_DIR)/nbody_x4_x100.h
DEPS += $(SRC_DIR)/transmit_new_i.cc
AIE_FLAGS += --platform=$(XPFM)

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
	rm -rf $(BUILD_DIR)