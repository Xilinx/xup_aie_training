// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include <stdint.h>

template <int WM, int WS, int BLEN = 32>
void storeStreamToMaster(hls::stream<ap_axiu<WS, 0, 0, 0> >& s, ap_uint<WM>* mm, uint64_t size) {
#ifndef __SYNTHESIS__
    static_assert(WM == WS, "WM should be equal to WS in the current implementation");
#endif
    const int bytePerData = WM / 8;
    const int nBlks = size / bytePerData + ((size % bytePerData) > 0);
    const int nBurst = nBlks / BLEN;
    int cnt = 0;
    for (int i = 0; i < nBurst; i++) {
        for (int j = 0; j < BLEN; j++) {
#pragma HLS pipeline II = 1
            ap_axiu<WS, 0, 0, 0> tmp = s.read();
            mm[cnt] = tmp.data;
            cnt++;
        }
    }
    int leftBlks = nBlks % BLEN;
    if (leftBlks > 0) {
        for (int i = 0; i < leftBlks; i++) {
#pragma HLS pipeline II = 1
            ap_axiu<WS, 0, 0, 0> tmp = s.read();
            mm[cnt] = tmp.data;
            cnt++;
        }
    }
}

extern "C" void s2m_x4(
    // 0
    hls::stream<ap_axiu<32, 0, 0, 0> >& k0_strm,
    ap_uint<32>* k0_buff,
    uint64_t sz0,

    // 1
    hls::stream<ap_axiu<32, 0, 0, 0> >& k1_strm,
    ap_uint<32>* k1_buff,
    uint64_t sz1,

    // 2
    hls::stream<ap_axiu<32, 0, 0, 0> >& k2_strm,
    ap_uint<32>* k2_buff,
    uint64_t sz2,

    // 3
    hls::stream<ap_axiu<32, 0, 0, 0> >& k3_strm,
    ap_uint<32>* k3_buff,
    uint64_t sz3

    ) {
    ; // clang-format off
#pragma HLS interface axis port=k0_strm
#pragma HLS interface m_axi offset=slave bundle=gmem0 port=k0_buff \
    max_write_burst_length=32 num_write_outstanding=4 latency=128
#pragma HLS interface s_axilite bundle=control port=k0_buff
#pragma HLS interface s_axilite bundle=control port=sz0

#pragma HLS interface axis port=k1_strm
#pragma HLS interface m_axi offset=slave bundle=gmem1 port=k1_buff \
    max_write_burst_length=32 num_write_outstanding=4 latency=128
#pragma HLS interface s_axilite bundle=control port=k1_buff
#pragma HLS interface s_axilite bundle=control port=sz1

#pragma HLS interface axis port=k2_strm
#pragma HLS interface m_axi offset=slave bundle=gmem2 port=k2_buff \
    max_write_burst_length=32 num_write_outstanding=4 latency=128
#pragma HLS interface s_axilite bundle=control port=k2_buff
#pragma HLS interface s_axilite bundle=control port=sz2

#pragma HLS interface axis port=k3_strm
#pragma HLS interface m_axi offset=slave bundle=gmem3 port=k3_buff \
    max_write_burst_length=32 num_write_outstanding=4 latency=128
#pragma HLS interface s_axilite bundle=control port=k3_buff
#pragma HLS interface s_axilite bundle=control port=sz3

#pragma HLS interface s_axilite bundle=control port=return
    ; // clang-format on

#pragma HLS dataflow

    storeStreamToMaster(k0_strm, k0_buff, sz0);
    storeStreamToMaster(k1_strm, k1_buff, sz1);
    storeStreamToMaster(k2_strm, k2_buff, sz2);
    storeStreamToMaster(k3_strm, k3_buff, sz3);
}