// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

#include "adf.h"
#include "fft.hpp"
#include "fir.hpp"

class SpectrumGraph: public graph {
    private:
        FFT1d_graph fft;
        FIR_129_sym fir;

    public:
        adf::port<input> in;
        adf::port<output> out;
        SpectrumGraph() {
            connect<stream>(in, fir.in);
            connect< window<FFT_POINT_SIZE * 4> > (fir.out,  fft.in);
            connect<stream>(fft.out, out);
        }
};


class MultiFIRGraph: public graph {
    private:
        FIR_129_sym fir_lp;
        FIR_129_sym fir_hp;
        FIR_129_sym fir_sb;
    public:
        adf::port<input> in[3];
        adf::port<output> out[3];
        MultiFIRGraph() : fir_lp(fir_taps::lp_taps), fir_hp(fir_taps::hp_taps), fir_sb(fir_taps::sb_taps) {
            connect<stream>(in[0], fir_lp.in);
            connect<stream>(fir_lp.out, out[0]);
            connect<stream>(in[1], fir_hp.in);
            connect<stream>(fir_hp.out, out[1]);
            connect<stream>(in[2], fir_sb.in);
            connect<stream>(fir_sb.out, out[2]);
        }
};


class DSPLibGraph: public graph {
    private:
        FFT1d_graph fft;
        FIR_129_sym fir;
        SpectrumGraph spectrum;
        MultiFIRGraph multi_fir;
    public:
        adf::input_plio s_in[4];
        adf::output_plio s_out[6];
        DSPLibGraph() {
            // FIR connections
            s_in[0] = input_plio::create("DataInFIR0", adf::plio_128_bits, "data/DataInFIR0.txt");
            s_out[0] = output_plio::create("DataOutFIR0", adf::plio_128_bits, "DataOutFIR0.txt");
            adf::connect<> (s_in[0].out[0],  fir.in);
            adf::connect<> (fir.out, s_out[0].in[0]);

            // FFT connections
            s_in[1] = input_plio::create("DataInFFT0", adf::plio_128_bits, "data/DataInFFT0.txt");
            s_out[1] = output_plio::create("DataOutFFT0", adf::plio_128_bits, "DataOutFFT0.txt");
            adf::connect<stream> (s_in[1].out[0],  fft.in);
            adf::connect<stream> (fft.out, s_out[1].in[0]);
            
            // Spectrum connections
            s_in[2] = input_plio::create("DataInFIR1", plio_128_bits, "data/DataInFIR1.txt");
            s_out[2] = output_plio::create("DataOutFFT1", plio_128_bits, "DataOutFFT1.txt");
            adf::connect<stream> (s_in[2].out[0],  spectrum.in);
            adf::connect<stream> (spectrum.out, s_out[2].in[0]);

            // Multi FIR connections, input is broadcasted to all filters
            s_in[3] = input_plio::create("DataInFIR2", plio_128_bits, "data/DataInFIR2.txt");
            s_out[3] = output_plio::create("DataOutFIRLp", plio_128_bits, "DataOutFIRLp.txt");
            s_out[4] = output_plio::create("DataOutFIRHp", plio_128_bits, "DataOutFIRHp.txt");
            s_out[5] = output_plio::create("DataOutFIRSb", plio_128_bits, "DataOutFIRSb.txt");

            for (int i=0; i < 3; i++) {
                adf::connect<stream> (s_in[3].out[0],  multi_fir.in[i]);
                adf::connect<stream> (multi_fir.out[i], s_out[i+3].in[0]);
            }
        }
};
