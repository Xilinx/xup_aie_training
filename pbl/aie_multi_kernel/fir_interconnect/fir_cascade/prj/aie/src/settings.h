// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

#define FIR_TAPS          			8
#define NUM_SAMPLES            	    1024
#define NBYTES_DATA                 2
#define AIE_MUL_LANES				8
#define NUM_MUL_8_LANES				(NUM_SAMPLES/AIE_MUL_LANES)
#define NUM_MUL_LOOP				2
#define LOOP_CYCLES					(NUM_MUL_8_LANES/NUM_MUL_LOOP)
#define FIR_T_COEFF					int16
#define FIR_T_DATA					int16
#define SRS_SHIFT 					3
