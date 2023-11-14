// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "aie_kernels.hpp"
#include "settings.h"

#define CONNECTION window< NUM_SAMPLES * NBYTES_DATA>

using namespace adf;

class simpleGraph : public graph {
    private:
        kernel fir1;
        kernel fir2;

    public:
        input_plio p_s0;
        output_plio p_s1;

        simpleGraph() {
            // create kernel & define source code

            fir1 = kernel::create(fir_asym_8t_16int_window);
            source(fir1) = "fir_window.cc";

            fir2 = kernel::create(fir_asym_8t_16int_window);
            source(fir2) = "fir_window.cc";

            // Define connection names and text file source/sink
            p_s0 = input_plio::create("StreamIn0", plio_32_bits, "data/input.txt");
            p_s1 = output_plio::create("StreamOut0", plio_32_bits, "output.txt");

            //connect ports and kernel
            connect<CONNECTION>(p_s0.out[0], fir1.in[0]);
            connect<CONNECTION>(fir1.out[0], fir2.in[0]);
            connect<CONNECTION>(fir2.out[0], p_s1.in[0]);

            // Define kernel runtime ratio
            runtime<ratio>(fir1) = 0.9;
            runtime<ratio>(fir2) = 0.9;
        };
};

#endif /**********__GRAPH_H__**********/
