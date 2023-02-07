// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <adf.h>
#include "aie_api/aie.hpp"
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

void aie_vadd_window(input_window_int32 *in0, input_window_int32 *in1, output_window<int32> *out){
    for (unsigned int i=0; i< 2048/16; i++) {
        auto a = window_readincr_v<16>(in0);
        auto b = window_readincr_v<16>(in1);
        auto res = aie::add(a, b);
        window_writeincr(out, res);
    }
}