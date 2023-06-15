// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

#include <adf.h>

void matmult_float(
  input_window_float* __restrict matA,
  input_window_float* __restrict matB,
  output_window_float* __restrict matC);

void matmult_int32(
    input_window_int32* __restrict matA,
    input_window_int32* __restrict matB,
    output_window_int32* __restrict matC);

void matmult_int16(
    input_window_int16* __restrict matA,
    input_window_int16* __restrict matB,
    output_window_int16* __restrict matC);

  void matmult_int8(
    input_window_int8* __restrict matA,
    input_window_int8* __restrict matB,
    output_window_int8* __restrict matC);
