// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "tiled_mm2s.h"

extern "C" {

// int32 matrix multiplication
// A in blocks of 2x4
void mm2s_i32_a(ap_int<32> *mem, hls::stream<ap_axiu<32, 0, 0, 0>> &s, int size_bytes)
{
    mm2s_tiled<16, 8, 4, 2, 32>(mem, s, size_bytes);
}

} // extern "C"
