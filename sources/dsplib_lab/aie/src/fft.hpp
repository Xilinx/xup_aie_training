// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

#define FFT_POINT_SIZE 2048
#define FFT_CASCADE_LEN 1

#include "adf.h"
#include "fft_ifft_dit_1ch_graph.hpp"

using namespace adf;

class FFT1d_graph: public graph {
    private:
        xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<cint16, cint16, FFT_POINT_SIZE, 1, 0, FFT_CASCADE_LEN, 0> FFT1d_kernel;
    public:
        adf::port<input> in;
        adf::port<output> out;

        FFT1d_graph() {
            //connect ports and kernel
            adf::connect< window<FFT_POINT_SIZE * 4> > (in,  FFT1d_kernel.in[0]);
            adf::connect< window<FFT_POINT_SIZE * 4> > (FFT1d_kernel.out[0], out);
            // Define kernel runtime ratio         
            runtime<ratio>(*FFT1d_kernel.getKernels()) = 0.8;
        }
};
