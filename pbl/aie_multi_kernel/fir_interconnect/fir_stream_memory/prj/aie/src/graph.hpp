// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "aie_kernels.hpp"
#include "settings.h"

//#define STREAM

#ifdef STREAM
#define CONNECTION stream
#else
#define CONNECTION window< NUM_SAMPLES * NBYTES_DATA >
#endif

using namespace adf;

class simpleGraph : public graph {
    private:
        kernel fir;

    public:
        input_plio p_s0;
        output_plio p_s1;

        simpleGraph() {
            // create kernel & define source code
#ifdef STREAM
            fir = kernel::create(fir_asym_8t_16int_stream);
            source(fir) = "fir_stream.cc";
#else
            fir = kernel::create(fir_asym_8t_16int_window);
            source(fir) = "fir_window.cc";
#endif
            // Define connection names and text file source/sink
            p_s0 = input_plio::create("StreamIn0", plio_32_bits, "data/input.txt");
            p_s1 = output_plio::create("StreamOut0", plio_32_bits, "output.txt");

            //connect ports and kernel
            connect<CONNECTION>(p_s0.out[0], fir.in[0]);
            connect<CONNECTION>(fir.out[0], p_s1.in[0]);

            // Define kernel runtime ratio
            runtime<ratio>(fir) = 0.9;
        };
};

#endif /**********__GRAPH_H__**********/
