// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "matmult_generic.h"
#include "system_settings.h"

#include <adf.h>

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
Block dimensions    -> 4x2, 2x4
*/

void matmult_float(input_window<float> *__restrict matA,
                   input_window<float> *__restrict matB,
                   output_window<float> *__restrict matC){

    constexpr unsigned M = 4;
    constexpr unsigned K = 2;
    constexpr unsigned N = 4;
    mmul_blocked<M, K, N, float>(F_Ra/M,
                                 F_Ca/K,
                                 F_Cb/N,
                                 reinterpret_cast<const float *>(matA->ptr),
                                 reinterpret_cast<const float *>(matB->ptr),
                                 reinterpret_cast<float *>(matC->ptr));
}
