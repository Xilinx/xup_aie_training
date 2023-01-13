// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <adf.h>
#include "system_settings.h"

/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
*/

void matmult_int32(
    input_window_int32* __restrict matA,
    input_window_int32* __restrict matB,
    output_window_int32* __restrict matC)
{

   v16int32 xbuff = window_readincr_v16(matA);      // holds the first 16 elements of matA
   v8int32 zbuff = undef_v8int32();  // Holds 8 elements of matB
   v8acc80 acc0 = undef_v8acc80();   // 8 accumulator registers on even rows
   v8acc80 acc1 = undef_v8acc80();   // 8 accumulator registers on odd rows
   v8int32 row_even = undef_v8int32();   // 8 lane registers for C even rows
   v8int32 row_odd = undef_v8int32();   // 8 lane registers for C odd rows


   #define LMUL   \
   zbuff = window_readincr_v8(matB); \
   acc0 = lmul8(xbuff, 0, 0x00000000, zbuff, 0, 0x76543210);\
   acc1 = lmul8(xbuff, 8, 0x00000000, zbuff, 0, 0x76543210)

   #define LMAC(Start1, Start2)\
   zbuff = window_readincr_v8(matB);\
   acc0 = lmac8(acc0, xbuff, Start1, 0x00000000, zbuff, 0, 0x76543210);\
   acc1 = lmac8(acc1, xbuff, Start2, 0x00000000, zbuff, 0, 0x76543210)


   for (unsigned int i=0;i<F_Ra/2;i++)  // Each iteration computes 2 rows of C
   chess_prepare_for_pipelining
   chess_loop_range(8,)
   {
     LMUL;
     LMAC(1,9);
     LMAC(2,10);
     LMAC(3,11);
     LMAC(4,12);
     LMAC(5,13);
     LMAC(6,14);
     LMAC(7,15);

     xbuff = window_readincr_v16(matA); // reads the next 2 rows of A
     window_decr_v8(matB, 8); // reset B pointer
     // shift round saturate
     row_even = srs(acc0, 0);
     window_writeincr(matC, row_even); // Writes 1 row of C
     row_odd = srs(acc1, 0);
     window_writeincr(matC, row_odd); // Writes the next row of C
   }
}
