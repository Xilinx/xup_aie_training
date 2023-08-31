// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "tiled_mm2s.h"

extern "C" {

// int16 matrix multiplication
// A in blocks of 2x4
void mm2s_i16_a(ap_int<16> *mem, hls::stream<ap_axiu<16, 0, 0, 0>> &s, int size_bytes)
{
    mm2s_tiled<16, 8, 4, 4, 16>(mem, s, size_bytes);
}

} // extern "C"
