// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "sk_types.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "adf/adf_api/XRTConfig.h"
#include "xrt/experimental/xrt_aie.h"
#include "xrt/experimental/xrt_kernel.h"
#include "xrt/xrt_bo.h"
#include <xaiengine.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

class xrtHandles : public pscontext {
    public:
        xrt::device dhdl;
        xrt::graph graphhdl;
        xrt::kernel mm2s;
        xrt::kernel s2mm;

        xrtHandles(xclDeviceHandle dhdl_in, const xuid_t xclbin_uuid) : dhdl(dhdl_in),
                   graphhdl(dhdl, xclbin_uuid, "filter_graph") {
            std::cout << "[ps kernel] xrtHandles constructor " << std::endl;
        }
};

__attribute__((visibility("default"))) pscontext* ps_kernel_init(xclDeviceHandle dhdl, const xuid_t xclbin_uuid) {
    xrtHandles* handles = new xrtHandles(dhdl, xclbin_uuid);
    std::cout << "[ps kernel] initializing function " << std::endl;
    return handles;
}


__attribute__((visibility("default"))) int ps_kernel(
    int iterCnt,
    xrtHandles* handles) {
    auto timeStart = std::chrono::high_resolution_clock::now();
    std::cout << "[ps kernel] Start " << std::endl;
    auto timeEnd = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd-timeStart);

    std::cout<<elapsed.count()<<std::endl;

    std::cout << "Iteration: " << iterCnt << std::endl;

    datamover dmaHls[FFT2D_INSTS];
    for(int i = 0; i < FFT2D_INSTS; ++i)    {
      std::cout << "Initialising datamover " << i << std::endl;
      dmaHls[i].init(handles.dhdl, top, i, iterCnt);
    }

    std::cout << "Initialising 2D-FFT Graph " << std::endl;
    int ret = fft2d_gr.init(dhdl, top, 0);

    if (ret != 0)
        return ret;
   
    std::cout << "Running 2D-FFT Graph " << std::endl;
    ret = handles.graphhdl.run(iterCnt);

    if (ret != 0)
        return ret;

    for(int i = 0; i < FFT2D_INSTS; ++i) {
        std::cout << "Running datamover " << i << std::endl;
        dmaHls[i].run();
    }

    for(int i = 0; i < FFT2D_INSTS; ++i) {
        std::cout << "Waiting for datamover " << i << " to complete", i << std::endl;
        dmaHls[i].waitTo_complete();
    }

    //Closing PL-Handles
    for(int i = 0; i < FFT2D_INSTS; ++i) {
        std::cout << "Closing Datamover " << i << " handles" << std::endl;
        dmaHls[i].close();
    }

    //Closing Graphs
    std::cout << "Closing 2D-FFT Graph" << "...\n";
    fft2d_gr.close();
    std::cout << "2D-FFT Graph %d end." << std::endl;

    return 0;
}

__attribute__((visibility("default"))) int ps_kernel_fini(xrtHandles* handles) {
    std::cout << "Releasing remaining XRT objects...\n";
    handles->graphhdl.end();
    delete handles;
    return 0;
}

#ifdef __cplusplus
}
#endif