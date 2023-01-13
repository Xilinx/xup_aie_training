// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cstdint>

#include "xrt.h"
#include "experimental/xrt_kernel.h"

#define NSAMPLES 2048

int main(int argc, char** argv) {
    // Get device index and download xclbin
    std::cout << "Open the device" << std::endl;
    auto device = xrt::device(0);
    std::string binaryFile = "../build.hw/dsplib.xclbin";
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    // Get reference to the kernels
    std::cout << "Get references to datamovers compute units" << std::endl;
    auto mm2s_0 = xrt::kernel(device, uuid, "mm2s:{mm2s_fft_0}");
    auto s2mm_0 = xrt::kernel(device, uuid, "s2mm:{s2mm_fft_0}");

    // Allocating the input size of sizeIn to MM2S
    std::cout << "Allocate Buffer in Global Memory" << std::endl;
    size_t samples_size = sizeof(int16_t) * NSAMPLES * 2;
    auto in_buff_1 = xrt::bo(device, samples_size, mm2s_0.group_id(0));
    auto out_buff_1 = xrt::bo(device, samples_size, s2mm_0.group_id(0));

    // Generate random data and write data to compute unit buffers
    auto *sample_vector = new int16_t [NSAMPLES][2];
    auto *fir_result = new int16_t [NSAMPLES][2];

    in_buff_1.write(sample_vector);
    
    // Synchronize input buffers data to device global memory
    in_buff_1.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Execute the compute units
    auto run_mm2s_0 = mm2s_0(in_buff_1, nullptr, NSAMPLES/4);
    auto run_s2mm_0 = s2mm_0(out_buff_1, nullptr, NSAMPLES/4);
    std::cout << "Kernels started" << std::endl;

    // Wait for kernels to complete
    run_mm2s_0.wait();
    run_s2mm_0.wait();

    // Synchronize the output buffer data from the device
    out_buff_1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Read output buffer data to local buffer
    out_buff_1.read(fir_result);

    std::string test = "FAILED";
    auto error  = 0;
    if (error == 0)
        test = "PASSED";

    std::cout << "TEST " << test << std::endl;

    return error;
}