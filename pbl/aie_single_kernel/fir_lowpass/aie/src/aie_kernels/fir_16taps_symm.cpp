// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

/**
 * 16-tap symmetric FIR filter
 *
 * Coefficients:            c0 c1 c2 c3 c4 c5 c6 c7 c7 c6 c5 c4 c3 c2 c1 c0
 * Data:                    d0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 ...
 * Intermediate:           o0  = c0*( 0+15) + c1*( 1+14) + c2*( 2+13) + c3*( 3+12)
 *                         o1  = c0*( 1+16) + c1*( 2+15) + c2*( 3+14) + c3*( 4+13)
 *                         o2  = c0*( 2+17) + c1*( 3+16) + c2*( 4+15) + c3*( 5+14)
 *                         o3  = c0*( 3+18) + c1*( 4+17) + c2*( 5+16) + c3*( 6+15)
 *
 *                         o0 += c4*( 4+11) + c5*( 5+10) + c6*( 6+ 9) + c7*( 7+ 8)
 *                         o1 += c4*( 5+12) + c5*( 6+11) + c6*( 7+10) + c7*( 8+ 9)
 *                         o2 += c4*( 6+13) + c5*( 7+12) + c6*( 8+11) + c7*( 9+10)
 *                         o3 += c4*( 7+14) + c5*( 8+13) + c6*( 9+12) + c7*(10+11)
 *
 * o0..3  = f(c0..3, d0..6,  d12..18)
 * o0..3 += f(c4..7, d4..10, d8..14)
 *
 * Operator Definition
 *		using mulop = aie::sliding_mul_sym_xy_ops<  4  ,  16   ,      1     ,      1    ,   int16   , cint16>;
 *		                                            ^      ^          ^            ^          ^          ^
 *		                                         Lanes   Points   CoeffStep   DataStepXY  CoeffType  DataType
 *
 * Function
 *		auto acc = mulop::mul_sym(coeff,       0,   sbuff,     0);
 *		                            ^          ^      ^        ^
 *                            Coefficient   Coeff   Data     Data
 *                            Vector        Start   Vector   Start
 *
 */

#include <adf.h>
#include "system_settings.h"
#include <aie_api/aie.hpp>

alignas(32) int16_t taps[FIR_LEN] = {-29, -115, -122, 268, 1379, 3209, 5226, 6566, 6566, 5226, 3209, 1379,  268, -122, -115, -29};//2^15
//alignas(32) int16_t taps[FIR_LEN] = {-57, -229, -242, 536, 2758, 6418, 10452, 13131, 13131, 10452, 6418, 2758, 536, -242, -229, -57};//2^16
void fir_16taps_symm // _single_buf_array_indexing
(
		input_window<cint16> * cb_input,
		output_window<cint16> * cb_output
)
{
	const unsigned samples  = window_size(cb_output);
	const int shift         = SRS_SHIFT ;

	const aie::vector<int16,FIR_LEN> coeff = aie::load_v<FIR_LEN>((int16 *)taps) ;
	aie::vector<cint16,32> sbuff;

	const unsigned LSIZE = (samples / 4); // assuming # of samples is integer power of 2 and >= 64

	for ( unsigned i=0; i<LSIZE; i+=2)
	{
		sbuff.insert(0, window_readincr_v<8>(cb_input)); 	// 00++|04++|____|____    ____|____|____|____
		sbuff.insert(1, window_readincr_v<8>(cb_input));	// 00..|04..|08++|12++    ____|____|____|____
		sbuff.insert(2, window_read_v<8>(cb_input)); 		// 00..|04..|08..|12..    16++|20++|____|____

		constexpr unsigned Lanes=4, Points=16, CoeffStep=1, DataStepXY=1;
		using mulop = aie::sliding_mul_sym_xy_ops<Lanes, Points, CoeffStep, DataStepXY,int16,cint16>;

		auto acc = mulop::mul_sym(coeff,0,sbuff,0); // o0..3  =f(c0..7, d0..10,  d8..18)
	    window_writeincr(cb_output, acc.to_vector<cint16>(shift));

		acc = mulop::mul_sym(coeff,0,sbuff,4); // o4..7  =f(c0..7, d4..14,  d12..22)
	    window_writeincr(cb_output, acc.to_vector<cint16>(shift));

		window_decr_v<8>(cb_input,1);
	}
}

