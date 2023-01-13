// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include <stdint.h>

template <int WM, int WS, int BLEN = 32>
void loadMasterToStream(ap_uint<WM>* mm, hls::stream<ap_axiu<WS, 0, 0, 0> >& s, uint64_t size) {
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
            ap_axiu<WS, 0, 0, 0> tmp;
            tmp.data = mm[cnt];
            tmp.keep = -1;
            tmp.last = 0;
            s.write(tmp);
            cnt++;
        }
    }
    int leftBlks = nBlks % BLEN;
    if (leftBlks > 0) {
        for (int i = 0; i < leftBlks; i++) {
#pragma HLS pipeline II = 1
            ap_axiu<WS, 0, 0, 0> tmp;
            tmp.data = mm[cnt];
            tmp.keep = -1;
            tmp.last = 0;
            s.write(tmp);
            cnt++;
        }
    }
}


extern "C" void m2s_x2(
    // 0
    ap_uint<32>* ibuff,
    hls::stream<ap_axiu<32, 0, 0, 0> >& istrm,
    uint64_t sz0,

    // 1
    ap_uint<32>* jbuff,
    hls::stream<ap_axiu<32, 0, 0, 0> >& jstrm,
    uint64_t sz1

    ) {
    ; // clang-format off
#pragma HLS interface m_axi offset=slave bundle=gmem0 port=ibuff \
    max_read_burst_length=32 num_read_outstanding=4 latency=128
#pragma HLS interface s_axilite bundle=control port=ibuff
#pragma HLS interface axis port=istrm
#pragma HLS interface s_axilite bundle=control port=sz0

#pragma HLS interface m_axi offset=slave bundle=gmem1 port=jbuff \
    max_read_burst_length=32 num_read_outstanding=4 latency=128
#pragma HLS interface s_axilite bundle=control port=jbuff
#pragma HLS interface axis port=jstrm
#pragma HLS interface s_axilite bundle=control port=sz1

#pragma HLS interface s_axilite bundle=control port=return
    ; // clang-format on

#pragma HLS dataflow

    loadMasterToStream(ibuff, istrm, sz0);
    loadMasterToStream(jbuff, jstrm, sz1);
}