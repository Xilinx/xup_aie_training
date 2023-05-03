# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

SYSROOT = /proj/xbuilds/2023.1_daily_latest/internal_platforms/sw/versal/xilinx-versal/sysroots/aarch64-xilinx-linux

CROSSCXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++

CROSSCFLAGS = -I$(SYSROOT)/usr/include/xrt -I$(SYSROOT)/ -fPIC -fvisibility=hidden -lrt --sysroot=$(SYSROOT) -Ilibs -I$(XILINX_VITIS)/aietools/include
CROSSCFLAGS += -Wall -O2 -g -D__PS_ENABLE_AIE__
CROSSLDFLAGS = -shared -lxrt_core -L$(SYSROOT)/usr/lib -lxilinxopencl -lpthread -lrt -ldl -lcrypt -lstdc++ --sysroot=$(SYSROOT)


# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR = build
PS_TARGET = ps_kernel.so
PS_OBJS   = $(BUILD_DIR)/ps_kernel.o
# ################ TARGET: make all ################
all: ps_so

# ################ TARGET: make ps_so ################
.PHONY: ps_so
ps_so: $(BUILD_DIR) $(PS_TARGET)

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR);

$(PS_OBJS) : ps_kernel.cpp
	$(CROSSCXX) -c $(CROSSCFLAGS) -o $@ $<

$(PS_TARGET): $(PS_OBJS)
	$(CROSSCXX) $< $(CROSSLDFLAGS) -o $@

# ################ TARGET: make clean ################
clean:
	rm -rf $(BUILD_DIR)
	rm -rf *~
	rm -rf .Xil/
	rm -rf *.log *.jou
	rm -rf $(PS_TARGET)
