// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "matmult_generic.h"
#include "system_settings.h"

#include <adf.h>

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
Block dimensions    ->  4x8, 8x8
*/
void matmult_int8(input_window<int8> *__restrict matA,
                  input_window<int8> *__restrict matB,
                  output_window<int8> *__restrict matC)
{
    mmul_blocked<4, 8, 8, int8>(F_Ra/4, F_Ca/8, F_Cb/8, matA->ptr, matB->ptr, matC->ptr);
}
