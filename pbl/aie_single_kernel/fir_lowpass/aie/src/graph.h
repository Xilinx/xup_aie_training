// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

#include "aie_kernels.h"
#include "system_settings.h"

using namespace adf;

class Fir_Lowpass16 : public graph {
private:
	kernel k1;
public:
	input_plio in;
	output_plio out;

	Fir_Lowpass16()
	{
		// Create the PLIO ports
		in = input_plio::create("DataIn", plio_32_bits, "data/input.txt", 250.0);
		out = output_plio::create("DataOut", plio_32_bits, "data/output.txt", 250.0);		

		//Create Kernel
		k1 = kernel::create(fir_16taps_symm);

		// Create nets to connect kernels and IO ports
		connect< window<INPUT_SAMPLES*NBYTES_DATA,FIR_LEN*NBYTES_DATA> > net1 (in.out[0],k1.in[0]);
		connect< window<OUTPUT_SAMPLES*NBYTES_DATA> > net2 (k1.out[0],out.in[0]);

		// Specify kernel sources
		source(k1) = "aie_kernels/fir_16taps_symm.cpp";

		// Specify kernel runtime
		runtime<ratio>(k1) = 0.8;
	};
};
