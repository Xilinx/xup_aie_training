// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "aie_kernels.hpp"
#include "settings.h"

using namespace adf;

class simpleGraph : public graph {
    private:
        kernel fir_out;
        kernel fir_in;

    public:
        input_plio p_s0;
        output_plio p_s1;

        simpleGraph() {
            // create kernel & define source code

            fir_out = kernel::create(fir_8t_16int_cascade_in);
            source(fir_out) = "fir_cascade_in.cc";

            fir_in = kernel::create(fir_8t_16int_cascade_out);
            source(fir_in) = "fir_cascade_out.cc";

            // Define connection names and text file source/sink
            p_s0 = input_plio::create("StreamIn0", plio_32_bits, "data/input.txt");
            p_s1 = output_plio::create("StreamOut0", plio_32_bits, "output.txt");

            //connect ports and kernel
            connect<stream>(p_s0.out[0], fir_in.in[0]);
            connect<cascade>(fir_in.out[0], fir_out.in[0]);
            connect<stream>(fir_out.out[0], p_s1.in[0]);

            // Define kernel runtime ratio
            runtime<ratio>(fir_out) = 0.9;
            runtime<ratio>(fir_in) = 0.9;
        };
};

#endif /**********__GRAPH_H__**********/
