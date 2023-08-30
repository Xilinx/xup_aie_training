// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "config.h"

extern "C" {

void s2mm(ap_int<DWIDTH> *mem, hls::stream<data> &s, int size_bytes)
{
data_mover:
    const unsigned words = size_bytes * CHAR_BIT / DWIDTH;
    for (int i = 0; i < words; i++) {
        data x = s.read();
        mem[i] = x.data;
    }
}

} // extern "C"
