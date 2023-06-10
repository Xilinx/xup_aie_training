// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "config.h"

extern "C" {

void mm2s(ap_int<DWIDTH> *mem, hls::stream<data> &s, int size_bytes)
{
data_mover:
    const unsigned words = size_bytes * CHAR_BIT / DWIDTH;
    for (int i = 0; i < words; i++) {
        data x;
        x.data = mem[i];
        x.keep_all();
        s.write(x);
    }
}

} // extern "C"
