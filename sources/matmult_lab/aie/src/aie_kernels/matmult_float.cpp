// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <adf.h>
#include "system_settings.h"


/*
Matrix A dimensions -> 16x8
Matrix B dimensions ->  8x8
*/

void matmult_float(
    input_window_float* __restrict matA,
    input_window_float* __restrict matB,
    output_window_float* __restrict matC)
{

   v16float xbuff = window_readincr_v16(matA);      // holds the first 16 elements of matA
   v8float zbuff = undef_v8float();  // Holds 8 elements of matB
   v8float acc1 = undef_v8float();   // 8 accumulation values on even rows
   v8float acc2 = undef_v8float();   // 8 accumulation values on odd rows

   #define FPMUL   \
   zbuff = window_readincr_v8(matB); \
   acc1 = fpmul(xbuff,0,0x00000000,zbuff,0,0x76543210);\
   acc2 = fpmul(xbuff,8,0x00000000,zbuff,0,0x76543210)

   #define FPMAC(Start1, Start2)\
   zbuff = window_readincr_v8(matB);\
   acc1 = fpmac(acc1,xbuff,Start1,0x00000000,zbuff,0,0x76543210);\
   acc2 = fpmac(acc2,xbuff,Start2,0x00000000,zbuff,0,0x76543210)


   for (unsigned int i=0;i<F_Ra/2;i++)  // Each iteration computes 2 rows of C
   chess_prepare_for_pipelining
   chess_loop_range(8,)
   {
     FPMUL;
     FPMAC(1,9);
     FPMAC(2,10);
     FPMAC(3,11);
     FPMAC(4,12);
     FPMAC(5,13);
     FPMAC(6,14);
     FPMAC(7,15);

     xbuff = window_readincr_v16(matA); // reads the next 2 rows of A
     window_decr_v8(matB,8); // reset B pointer
     window_writeincr(matC,acc1); // Writes 1 row of C
     window_writeincr(matC,acc2); // Writes the next row of C
   }
}
