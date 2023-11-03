// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "tiled_mm2s.h"

extern "C" {

// int8 matrix multiplication
// A in blocks of 2x4
void mm2s_i8_b(ap_int<8> *mem, hls::stream<ap_axiu<8, 0, 0, 0>> &s, int size_bytes)
{
    mm2s_tiled<8, 8, 8, 4, 8>(mem, s, size_bytes);
}

} // extern "C"
