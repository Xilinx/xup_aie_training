# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

PLATFORM := xilinx_vck5000_gen4x8_qdma_2_202220_1
TARGET := hw
FREQ := 200

XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))

BUILD_DIR = build.$(TARGET)
VPP_FLAGS = -t $(TARGET) --platform $(PLATFORM)# --save-temps
VPP_FLAGS += --kernel_frequency $(FREQ)
INCLUDE = -I../aie/$(BUILD_DIR)/work/temp/

kernel_list = m2s_x2 s2m_x4 packet_sender packet_receiver
BINARY_OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .xo, $(kernel_list)))

all: $(BINARY_OBJS)

$(BUILD_DIR)/m2s_x2.xo: datamover/m2s_x2.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -o $@ $^

$(BUILD_DIR)/s2m_x4.xo: datamover/s2m_x4.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -o $@ $^

$(BUILD_DIR)/packet_sender.xo: packet/packet_sender.cpp
	v++ $(VPP_FLAGS) $(INCLUDE) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -o $@ $^

$(BUILD_DIR)/packet_receiver.xo: packet/packet_receiver.cpp
	v++ $(VPP_FLAGS) $(INCLUDE) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -o $@ $^

clean:
	rm -rf $(BUILD_DIR) *.log *.jou .Xil/
