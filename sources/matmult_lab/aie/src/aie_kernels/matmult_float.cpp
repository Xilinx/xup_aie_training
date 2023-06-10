// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "matmult_generic.h"
#include "system_settings.h"

#include <adf.h>

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
*/

void matmult_float(input_window<float> *__restrict matA,
                   input_window<float> *__restrict matB,
                   output_window<float> *__restrict matC)
{
    mmul_blocked<2, 4, 4, float>(F_Ra/2,
                                 F_Ca/4,
                                 F_Cb/4,
                                 reinterpret_cast<const float *>(matA->ptr),
                                 reinterpret_cast<const float *>(matB->ptr),
                                 reinterpret_cast<float *>(matC->ptr));
}
