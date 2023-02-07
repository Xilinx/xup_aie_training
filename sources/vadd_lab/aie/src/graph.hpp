// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "kernels.hpp"

#define STREAM

using namespace adf;

class simpleGraph : public graph {
    private:
        kernel vadd;

    public:
        input_plio p_s0;
        input_plio p_s1;
        output_plio p_s2;

        simpleGraph() {
            // create kernel
#ifdef STREAM
            vadd = kernel::create(aie_vadd_stream);
#else
            vadd = kernel::create(aie_vadd_window);
#endif
            // Define connection names and text file source/sink
            p_s0 = input_plio::create("StreamIn0", plio_32_bits, "data/input0.txt");
            p_s1 = input_plio::create("StreamIn1", plio_32_bits, "data/input1.txt");
            p_s2 = output_plio::create("StreamOut0", plio_32_bits, "output.txt");
#ifdef STREAM
            //connect ports and kernel
            connect<stream>(p_s0.out[0], vadd.in[0]);
            connect<stream>(p_s1.out[0], vadd.in[1]);
            connect<stream>(vadd.out[0], p_s2.in[0]);
            // Define kernel source code
            source(vadd) = "vadd_stream.cc";
#else
            //connect ports and kernel
            connect<window<2048 * sizeof(int)>>(p_s0.out[0], vadd.in[0]);
            connect<window<2048 * sizeof(int)>>(p_s1.out[0], vadd.in[1]);
            connect<window<2048 * sizeof(int)>>(vadd.out[0], p_s2.in[0]);

            // Define kernel source code
            source(vadd) = "vadd_window.cc";
#endif
            // Define kernel runtime ratio
            runtime<ratio>(vadd) = 1;
        };
};

#endif /**********__GRAPH_H__**********/
