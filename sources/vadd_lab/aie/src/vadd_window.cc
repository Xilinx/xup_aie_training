// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "aie_api/aie.hpp"
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

void aie_vadd_window(int32_t* in0, int32_t* in1, int32_t* out){
    for (unsigned int i=0; i< 2048/8; i++) {
        auto a = aie::load_v<8>(in0);
        auto b = aie::load_v<8>(in1);
        auto res = aie::add(a, b);
        aie::store_v(out, res);
        in0 += 8;
        in1 += 8;
        out += 8;
    }
}