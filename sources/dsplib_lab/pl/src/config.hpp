// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define DWIDTH 128
typedef ap_axiu<DWIDTH, 0, 0, 0> data;
