// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <adf.h>
#include "system_settings.h"

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
*/

void matmult_int8(
  input_window_int8* __restrict matA,
  input_window_int8* __restrict matB,
  output_window_int8* __restrict matC) {
   
   // holds the complete matB
   v64int8 xbuff = window_readincr_v64(matB);
   // Holds 32 elements of matA
   v32int8 zbuff = window_readincr_v32(matA);
   // 16 accumulator registers
   v16acc48 acc0 = undef_v16acc48();
   v16acc48 acc1 = undef_v16acc48();
   // 16 lane registers for C even rows
   v16int8 group_0 = undef_v16int8();
   // 16 lane registers for C odd rows
   v16int8 group_1 = undef_v16int8();

   //(xbuff, xstart, xoffsets, xstep, xsquare, zbuff, zstart, zoffsets, zstep, zsquare)
   #define IMUL \
     acc0 = mul16(xbuff,  0, 0x11101110, 16, 0x3120, zbuff, 0, 0x44440000, 2, 0x3210);\
     acc1 = mul16(xbuff,  0, 0x11101110, 16, 0x3120, zbuff, 0, 0xCCCC8888, 2, 0x3210)


   for (unsigned int i=0; i<F_Ra/4; i++)  // Each iteration computes 4 rows of C
   chess_prepare_for_pipelining
   chess_loop_range(4,)
   {
     IMUL;
     zbuff = window_readincr_v32(matA);
     // shift round saturate
     group_0 = bsrs(acc0, 0);
     window_writeincr(matC, group_0);
     group_1 = bsrs(acc1, 0);
     window_writeincr(matC, group_1);
   }
}
