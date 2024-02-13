# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

STATIC_LIB = $(shell readlink -f ./lib)
VIDEOLIB_ROOT = $(shell readlink -f ../../Vitis_Libraries/vision/)
CONFIG_FILE = $(shell readlink -f ../aie/src)

FLAGS := -Wall -c -g -fmessage-length=0
FLAGS += -std=c++17 -Wno-unknown-pragmas -Wno-unused-label
FLAGS += -Wno-int-to-pointer-cast

INCLUDES +=	-I$(XILINX_VITIS)/aietools/include
INCLUDES +=	-I$(XILINX_VITIS)/include
INCLUDES +=	-I$(XILINX_XRT)/include
INCLUDES +=	-I$(VIDEOLIB_ROOT)/L1/include/aie/
INCLUDES +=	-I$(CONFIG_FILE)
#INCLUDES +=	-I$(XILINX_HLS)/include/

LIBS := -lxrt_core -lxrt_coreutil -lgcc -lc
LIBS += -lxilinxopencl -lpthread -lrt -ldl -lcrypt -lstdc++ -lOpenCL
LIBS += -Wno-unused-label -Wno-narrowing -DVERBOSE
LIBS += -L$(XILINX_XRT)/lib/
LIBS += -L$(STATIC_LIB)/
LIBS += -lsmartTilerStitcher
OPENCV = `pkg-config --cflags --libs opencv4`

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR = build
EXECUTABLE = filter2d.exe
# ################ TARGET: make all ################
all: host

# ################ TARGET: make host ################
.PHONY: host
host: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR);

$(EXECUTABLE): $(BUILD_DIR)/host.o
	g++ $^ $(LIBS) $(INCLUDES) $(OPENCV) -std=c++17 -o $@
	@echo "COMPLETE: Host application $@ created."

# Create object files
$(BUILD_DIR)/host.o: host.cpp xfcvDataMovers.hpp
	g++ $(FLAGS) $(INCLUDES) $(OPENCV) -o $@ $<

# ################ TARGET: make clean ################
clean:
	rm -rf $(BUILD_DIR)
	rm -rf *~
	rm -rf .run/
	rm -rf *.csv
	rm -rf *.run_summary
	rm -rf .Xil/
	rm -rf *.log *.jou
	rm -rf $(EXECUTABLE)
