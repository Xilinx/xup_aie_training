// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "matmult_generic.h"
#include "system_settings.h"

#include <adf.h>

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
Block dimensions    ->  2x4, 4x8
*/

void matmult_int16(input_window<int16> *__restrict matA,
                   input_window<int16> *__restrict matB,
                   output_window<int16> *__restrict matC)
{
    mmul_blocked<2, 4, 8, int16>(F_Ra/2,
                                 F_Ca/4,
                                 F_Cb/8,
                                 reinterpret_cast<const int16 *>(matA->ptr),
                                 reinterpret_cast<const int16 *>(matB->ptr),
                                 reinterpret_cast<int16 *>(matC->ptr));
}
