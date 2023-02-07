// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <adf/stream/types.h>

#define STREAM

void aie_vadd_stream(input_stream_int32 *in0, input_stream_int32 *in1, output_stream_int32 *out);

void aie_vadd_window(input_window<int32> *in0, input_window<int32> *in1, output_window<int32> *out);

#endif /**********__KERNELS_H__**********/
