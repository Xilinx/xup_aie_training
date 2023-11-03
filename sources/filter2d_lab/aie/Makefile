# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

PLATFORM := xilinx_vck5000_gen4x8_qdma_2_202220_1
TARGET := hw

XPFM = $(shell platforminfo -p $(PLATFORM) --json="file")
XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))
VISION_LIB = $(shell readlink -f ../../Vitis_Libraries/vision/)

# OUTPUT PRODUCTS 
BUILD_DIR = build.$(TARGET)
WORK_DIR = work
SRC_DIR = $(shell readlink -f src/)
DATA_DIR = $(shell readlink -f data/)
CONSTRAINTS_DIR = $(shell readlink -f constraints/)

# DEPENDENCIES for make aie
GRAPH_CPP := $(SRC_DIR)/graph.cpp
DEPS := $(GRAPH_CPP)
DEPS += $(SRC_DIR)/graph.hpp
DEPS += $(SRC_DIR)/kernel.hpp
DEPS += $(SRC_DIR)/config.hpp
DEPS += $(SRC_DIR)/xf_filter2d.cpp


AIE_FLAGS = --platform=$(XPFM)
AIE_FLAGS += -include=$(VISION_LIB)/L1/include/aie/

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

aieemu:
	cd $(BUILD_DIR); \
	aiesimulator --pkg-dir=$(WORK_DIR) --i=..

x86sim:
	cd $(BUILD_DIR); \
	x86simulator --pkg-dir=$(WORK_DIR) --i=..