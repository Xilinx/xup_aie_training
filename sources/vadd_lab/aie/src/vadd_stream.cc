// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <adf.h>
#include "aie_api/aie.hpp"

void aie_vadd_stream(input_stream_int32 *in0, input_stream_int32 *in1, output_stream_int32 *out){
    auto a = readincr_v4(in0);
    auto b = readincr_v4(in1);
    auto res = aie::add(a, b);
    writeincr_v4(out, res);
}