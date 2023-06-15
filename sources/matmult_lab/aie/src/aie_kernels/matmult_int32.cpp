// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "matmult_generic.h"
#include "system_settings.h"

#include <adf.h>

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
Block dimensions    -> 2x4, 4x4
*/

void matmult_int32(input_window<int32> *__restrict matA,
                   input_window<int32> *__restrict matB,
                   output_window<int32> *__restrict matC)
{
    mmul_blocked<2, 4, 4, int32>(F_Ra/2,
                                 F_Ca/4,
                                 F_Cb/4,
                                 reinterpret_cast<const int32 *>(matA->ptr),
                                 reinterpret_cast<const int32 *>(matB->ptr),
                                 reinterpret_cast<int32 *>(matC->ptr));
}
