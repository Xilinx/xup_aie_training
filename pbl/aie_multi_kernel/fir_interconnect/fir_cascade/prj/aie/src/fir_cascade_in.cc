// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "aie_api/aie.hpp"
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
#include "aie_kernels.hpp"
#include "settings.h"

static constexpr int16_t taps[FIR_TAPS] = { 1, 1, 1, 1, 1, 1, 1, 1};

void fir_8t_16int_cascade_in(input_stream_acc48 * __restrict cascade_in, output_stream_int16 * __restrict out)
{
	aie::vector<int16, 16> data;
	const aie::vector<int16, 8> coeff = aie::load_v<8>(taps);
	aie::accum<acc48, AIE_MUL_LANES> acc0;

    constexpr unsigned CoeffStep   	= 1;
    constexpr unsigned DataStepXY   = 1;
    constexpr unsigned CoeffStart 	= 0;
    const int shift         = SRS_SHIFT ;


	using mul_ops = aie::sliding_mul_xy_ops<AIE_MUL_LANES, FIR_TAPS, CoeffStep,DataStepXY, FIR_T_COEFF , FIR_T_DATA>;

	data = aie::zeros<int16, 16>();

	//Main loop
	for(int i = 0; i<LOOP_CYCLES; i++){

		// Read 8 samples and insert then in the second part of the data vector
		auto v1 = readincr_v<8>(cascade_in).to_vector<int16>(shift);
		data.insert(1, v1);		// 0...0 | d0..d7
		//Run multiply operation
		acc0 = mul_ops::mul(coeff,CoeffStart,data,1) ;
		//Output 8 samples
		writeincr(out, acc0.to_vector<int16>(shift));

		// Read 8 samples and insert then in the first part of the data vector
		auto v2 = readincr_v<8>(cascade_in).to_vector<int16>(shift);
		//auto v2 = input_cas2.to_vector<int16>(shift);
		data.insert(0, v2);		// d8...d15 | d0..d7
		//Run multiply operation
		acc0 = mul_ops::mul(coeff,CoeffStart,data,9) ;
		//Output the next 8 samples
		writeincr(out, acc0.to_vector<int16>(shift));
	}

}