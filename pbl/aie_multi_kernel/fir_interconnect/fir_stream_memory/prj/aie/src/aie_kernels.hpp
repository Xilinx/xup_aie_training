// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <adf/stream/types.h>

// Kernel prototype
void fir_asym_8t_16int_window(input_window_int16 * in, output_window_int16 * out);

void fir_asym_8t_16int_stream(input_stream_int16 * in, output_stream_int16 * out);

#endif /**********__KERNELS_H__**********/
