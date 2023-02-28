// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

extern "C" {

void pl_mm2s(ap_int<32>* mem, hls::stream<ap_axiu<32, 0, 0, 0> >& s, int size) {
data_mover:
    for (int i = 0; i < size; i++) {
        ap_axiu<32, 0, 0, 0> x;
        x.data = mem[i];
        s.write(x);
    }
}
}
