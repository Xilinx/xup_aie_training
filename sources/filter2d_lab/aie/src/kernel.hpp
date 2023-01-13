// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef _KERNELS_16B_H_
#define _KERNELS_16B_H_

#include <adf/window/types.h>
#include <adf/stream/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void filter2D(input_window_int16* input, output_window_int16* output);

#endif