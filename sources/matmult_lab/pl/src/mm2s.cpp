// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "config.hpp"

extern "C" {

void mm2s(ap_int<DWIDTH>* mem, hls::stream<data >& s, int size) {
data_mover:
    for (int i = 0; i < size; i++) {
        data x;
        x.data = mem[i];
        x.keep_all();
        s.write(x);
    }
}
}