# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

PLATFORM := xilinx_vck5000_gen4x8_qdma_2_202220_1
TARGET := hw

XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))
AIE_DIR = $(shell readlink -f ./aie)
PL_DIR = $(shell readlink -f ./pl)
HOST_DIR = $(shell readlink -f ./host)
HW_LINK = $(shell readlink -f ./hw_link/config.cfg)

XCLBIN_NAME = nbody
JOBS = 16
FREQ = 150
PACKAGE_FLAGS = -t $(TARGET) --platform $(PLATFORM) --save-temps

VPP_FLAGS = $(PACKAGE_FLAGS)
VPP_FLAGS += -g --optimize 2
VPP_FLAGS += --hls.jobs $(JOBS)
VPP_FLAGS += --config $(HW_LINK)
VPP_FLAGS += --clock.defaultFreqHz $(shell echo "scale=0;$(FREQ)*1000000/1" | bc)
VPP_LDFLAGS := --vivado.synth.jobs $(JOBS) --vivado.impl.jobs $(JOBS)

BUILD_DIR = build.$(TARGET)
OUTPUT_DIR = $(shell readlink -f ./$(BUILD_DIR))

AIE_SRCS = $(AIE_DIR)/$(BUILD_DIR)/libadf.a
XO_SRCS = $(PL_DIR)/$(BUILD_DIR)/m2s_x2.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/s2m_x4.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/packet_receiver.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/packet_sender.xo
HOST_APP = $(HOST_DIR)/app.exe
HOST_APP += $(HOST_DIR)/app_animate.exe


all: $(OUTPUT_DIR)/${XCLBIN_NAME}.xclbin $(HOST_APP)

$(AIE_SRCS):
	make -C $(AIE_DIR)/ PLATFORM=$(PLATFORM) FREQ=$(FREQ) TARGET=$(TARGET)

$(XO_SRCS): $(AIE_SRCS)
	make -C $(PL_DIR)/ PLATFORM=$(PLATFORM) FREQ=$(FREQ) TARGET=$(TARGET) -j4

$(HOST_APP):
	make -C $(HOST_DIR)

# Building xsa
$(OUTPUT_DIR)/$(XCLBIN_NAME).xsa: $(AIE_SRCS) $(XO_SRCS)
	@echo "### ***** linking pl kernels into $(XCLBIN_NAME).xsa ... *****"
	mkdir -p $(OUTPUT_DIR); \
	cd $(OUTPUT_DIR); \
	v++ -l $(VPP_FLAGS) \
	  --temp_dir _x_temp/ \
	  --report_dir reports/ \
	  $(VPP_LDFLAGS) \
	  $^ \
	  -o $@ 2>&1 | tee $(XCLBIN_NAME)_xsa.log
	@echo "### ***** $(XCLBIN_NAME).xsa linking done! *****"

# Building xclbin
$(OUTPUT_DIR)/${XCLBIN_NAME}.xclbin: $(OUTPUT_DIR)/${XCLBIN_NAME}.xsa
	@echo "### ***** packaging xsa into $(XCLBIN_NAME).xclbin ... *****"
	cd $(OUTPUT_DIR); \
	v++ --package $(PACKAGE_FLAGS) \
	  $^ \
	  $(AIE_SRCS) \
	  --temp_dir _x_temp \
	  --report_dir reports/ \
	  --package.boot_mode=ospi \
	  -o $@ 2>&1 | tee $(XCLBIN_NAME)_xclbin.log
	@echo "### ***** $(XCLBIN_NAME).xclbin packaging done! *****"

clean:
	rm -rf *.log *.jou .Xil/

distclean: clean
	rm -rf $(BUILD_DIR)