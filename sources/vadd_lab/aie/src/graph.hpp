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
            // create kernel & define source code
#ifdef STREAM
            vadd = kernel::create(aie_vadd_stream);
            source(vadd) = "vadd_stream.cc";
#else
            vadd = kernel::create(aie_vadd_window);
            source(vadd) = "vadd_window.cc";
#endif
            // Define connection names and text file source/sink
            p_s0 = input_plio::create("StreamIn0", plio_32_bits, "data/input0.txt");
            p_s1 = input_plio::create("StreamIn1", plio_32_bits, "data/input1.txt");
            p_s2 = output_plio::create("StreamOut0", plio_32_bits, "output.txt");

            //connect ports and kernel
            connect(p_s0.out[0], vadd.in[0]);
            connect(p_s1.out[0], vadd.in[1]);
            connect(vadd.out[0], p_s2.in[0]);

#ifndef STREAM
            adf::dimensions(vadd.in[0]) = {2048};
            adf::dimensions(vadd.in[1]) = {2048};
            adf::dimensions(vadd.out[0]) = {2048};
#endif

            // Define kernel runtime ratio
            runtime<ratio>(vadd) = 1;
        };
};

#endif /**********__GRAPH_H__**********/
