// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "kernel.hpp"
#include "config.hpp"

using namespace adf;

class Filter2DGraph : public adf::graph {
    private:
        kernel f2d;

    public:
        input_plio in;
        output_plio out;

        Filter2DGraph() {
            // create kernel
            f2d = kernel::create(filter2D);

            in = input_plio::create("DataIn1", plio_128_bits, "data/input.txt");
            out = output_plio::create("DataOut1", plio_128_bits, "data/output.txt");

            //Make AIE connections
            adf::connect<window<TILE_WINDOW_SIZE> >(in.out[0], f2d.in[0]);
            adf::connect<window<TILE_WINDOW_SIZE> >(f2d.out[0], out.in[0]);

            source(f2d) = "xf_filter2d.cpp";
            runtime<ratio>(f2d) = 1;
    };
};

#endif //__GRAPH_H__
