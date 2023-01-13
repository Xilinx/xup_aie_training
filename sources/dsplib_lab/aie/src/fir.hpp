// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

#include <adf.h>
#include "fir_sr_sym_graph.hpp"

#define FIR129_LENGTH 129
#define FIR129_SHIFT 15
#define FIR129_ROUND_MODE 0
#define FIR129_INPUT_SAMPLES 2048
#define FIR129_CASCADE_LEN 1


namespace fir_taps {

    std::vector<int16> bp_taps = std::vector<int16> {
        -5, 4, 18, 28, 28, 17, 1, -9, -9, 1, 13, 11, -10, -45, -74, -78, -50, -6, 
        27, 27, -3, -36, -33, 26, 121, 199, 208, 134, 18, -67, -68, 6, 87, 80, -56, -274, 
        -451, -472, -307, -47, 145, 153, -10, -188, -179, 113, 593, 998, 1064, 711, 124, -335, -374, 12, 
        474, 490, -291, -1752, -3251, -3905, -3062, -724, 2350, 4943, 5954
    };

    std::vector<int16> sb_taps = std::vector<int16> {
        14, -2, -2, 5, -5, -26, -22, 13, 37, 19, -7, -1, 11, -20, -66, -41, 47, 88, 
        31, -22, 4, 21, -68, -153, -61, 134, 178, 36, -47, 26, 26, -180, -302, -52, 310, 312, 
        18, -80, 77, 3, -416, -535, 38, 646, 503, -48, -114, 182, -112, -939, -941, 353, 1404, 854, 
        -245, -139, 463, -639, -2852, -2372, 2169, 6052, 3649, -3499, 25337
    };

    std::vector<int16> hp_taps = std::vector<int16> {
        12, -5, -7, 14, -8, -6, 17, -12, -6, 22, -19, -4, 29, -28, -1, 36, -41, 4, 
        44, -57, 14, 52, -78, 29, 58, -102, 50, 61, -130, 79, 59, -161, 116, 51, -194, 164, 
        34, -230, 224, 6, -266, 299, -38, -301, 392, -104, -335, 510, -200, -366, 665, -344, -393, 884, 
        -572, -415, 1237, -985, -432, 1958, -1985, -442, 4710, -8794, 10473
    };

    std::vector<int16> lp_taps = std::vector<int16>{
        -26, -26, -25, -24, -22, -20, -16, -12, -6, 1, 9, 20, 32, 45, 59, 73, 87, 100, 
        112, 121, 127, 129, 125, 116, 100, 77, 47, 10, -33, -83, -137, -195, -255, -315, -372, -424, 
        -468, -501, -521, -525, -510, -475, -417, -334, -227, -95, 61, 242, 445, 668, 908, 1160, 1422, 1688, 
        1954, 2215, 2465, 2700, 2915, 3105, 3267, 3396, 3491, 3548, 3568
    };
}

class FIR_129_sym : public graph {
    private:
        xf::dsp::aie::fir::sr_sym::fir_sr_sym_graph<cint16, int16, FIR129_LENGTH, FIR129_SHIFT, FIR129_ROUND_MODE, FIR129_INPUT_SAMPLES, FIR129_CASCADE_LEN> firGraph;

    public:
        adf::port<input> in;
        adf::port<output> out;

        // Default constructor, if no taps are passed
        FIR_129_sym() : firGraph(fir_taps::bp_taps) {
            // Margin gets automatically added within the FIR graph class.
            // Margin equals to FIR length rounded up to nearest multiple of 32 Bytes.
            adf::connect<>(in, firGraph.in[0]);
            adf::connect<>(firGraph.out[0], out);
        };

        FIR_129_sym(const std::vector<int16> &taps) : firGraph(taps) {
            // Margin gets automatically added within the FIR graph class.
            // Margin equals to FIR length rounded up to nearest multiple of 32 Bytes.
            adf::connect<>(in, firGraph.in[0]);
            adf::connect<>(firGraph.out[0], out);
        };

};