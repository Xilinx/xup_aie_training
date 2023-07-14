// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "tiled_s2mm.h"

extern "C" {

// int32 matrix multiplication
// C in blocks of 2x4
void s2mm_i32_c(ap_int<32> *mem, hls::stream<qdma_axis<32, 0, 0, 0>> &s, int size_bytes)
{
    s2mm_tiled<16, 8, 4, 4, 32>(mem, s, size_bytes);
}

} // extern "C"
