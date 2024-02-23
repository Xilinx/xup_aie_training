# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

FLAGS := -Wall -c -g -fmessage-length=0
FLAGS += -std=c++17 -Wno-unknown-pragmas -Wno-unused-label
FLAGS += -Wno-int-to-pointer-cast


INCLUDES +=	-I$(XILINX_VITIS)/aietools/include
INCLUDES +=	-I$(XILINX_VITIS)/include
INCLUDES +=	-I$(XILINX_XRT)/include

LIBS := -lxrt_core -lxrt_coreutil -lgcc -lc
LIBS += -lxilinxopencl -lpthread -lrt -ldl -lcrypt -lstdc++ -lOpenCL
LIBS += -Wno-unused-label -Wno-narrowing -DVERBOSE
LIBS += -L$(XILINX_XRT)/lib/

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR = build
EXECUTABLE = app.exe
EXECUTABLE += app_animate.exe

# ################ TARGET: make all ################
all: host

# ################ TARGET: make host ################
.PHONY: host
host: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR);

app.exe: $(BUILD_DIR)/app.o $(BUILD_DIR)/log.o $(BUILD_DIR)/nbody.o
	g++ $^ $(LIBS) -std=c++17 -o $@
	@echo "COMPLETE: Host application $@ created."

%.exe: $(BUILD_DIR)/app_animate.o $(BUILD_DIR)/log.o $(BUILD_DIR)/nbody.o
	g++ $^ $(LIBS) -std=c++17 -o $@
	@echo "COMPLETE: Host application $@ created."

# Create object files
$(BUILD_DIR)/app.o: src/main_xrt.cpp src/main_xrt.hpp
	g++ $(FLAGS) $(INCLUDES) -o $(BUILD_DIR)/app.o src/main_xrt.cpp

$(BUILD_DIR)/app_animate.o: src/main_animate.cpp src/main_xrt.hpp
	g++ $(FLAGS) $(INCLUDES) -o $(BUILD_DIR)/app_animate.o src/main_animate.cpp

$(BUILD_DIR)/log.o: src/log.cpp src/log.hpp
	g++ $(FLAGS) $(INCLUDES) -o $(BUILD_DIR)/log.o src/log.cpp

$(BUILD_DIR)/nbody.o: src/nbody.cpp src/nbody.hpp
	g++ $(FLAGS) $(INCLUDES) -o $(BUILD_DIR)/nbody.o src/nbody.cpp


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
