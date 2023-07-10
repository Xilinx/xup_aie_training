// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "matmult_generic.h"
#include "system_settings.h"

#include <adf.h>

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
Block dimensions    -> 4x8, 8x4
*/

void matmult_int8(input_window<int8> *__restrict matA,
                  input_window<int8> *__restrict matB,
                  output_window<int8> *__restrict matC){

    constexpr unsigned M = 4;
    constexpr unsigned K = 8;
    constexpr unsigned N = 4;
    mmul_blocked<M, K, N, int8>(F_Ra/M,
                                F_Ca/K,
                                F_Cb/N,
                                matA->ptr,
                                matB->ptr,
                                matC->ptr);
}
