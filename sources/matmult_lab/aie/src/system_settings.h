// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

/* Matrix Multiply C = A x B


Dimensions
  A: 16 x 8
  B:  8 x 8

  C: 8 x 8

*/

// Floating point example
#define F_Ra 16
#define F_Ca 8
#define F_Rb (F_Ca)
#define F_Cb 8

#define F_Rc (F_Ra)
#define F_Cc (F_Cb)

// Window size
#define NSAMPLES_WINDOW_F_A (F_Ra*F_Ca)
#define NSAMPLES_WINDOW_F_B (F_Rb*F_Cb)
#define NSAMPLES_WINDOW_F_C (F_Rc*F_Cc)
