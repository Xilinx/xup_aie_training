// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "tiled_mm2s.h"

extern "C" {

// int32 matrix multiplication
// B in blocks of 4x4
void mm2s_i32_b(ap_int<32> *mem, hls::stream<ap_axiu<32, 0, 0, 0>> &s, int size_bytes)
{
    mm2s_tiled<8, 8, 2, 4, 32>(mem, s, size_bytes);
}

} // extern "C"
