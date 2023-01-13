// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <adf.h>
#include "aie_kernels.h"
#include "system_settings.h"

using namespace adf;

template<int R=100>
class MatMultFloatGraph : public graph {
private:
    kernel k;

public:
    port<input> ina, inb;
    port<output> outc;

    MatMultFloatGraph() {
        k = kernel::create(matmult_float);

        connect< window<NSAMPLES_WINDOW_F_A*NBYTES_FLOAT> > float_ina(ina, k.in[0]);
        connect< window<NSAMPLES_WINDOW_F_B*NBYTES_FLOAT> > float_inb(inb, k.in[1]);
        connect< window<NSAMPLES_WINDOW_F_C*NBYTES_FLOAT> > float_outc(k.out[0], outc);
        source(k) = "aie_kernels/matmult_float.cpp";
        runtime<ratio>(k) = float(R/100.0);
    }
};

template<int R=100>
class MatMultInt32Graph : public graph {
private:
    kernel k;

public:
    port<input> ina, inb;
    port<output> outc;

    MatMultInt32Graph() {
        k = kernel::create(matmult_int32);

        connect< window<NSAMPLES_WINDOW_F_A*NBYTES_INT32> > int32_ina(ina, k.in[0]);
        connect< window<NSAMPLES_WINDOW_F_B*NBYTES_INT32> > int32_inb(inb, k.in[1]);
        connect< window<NSAMPLES_WINDOW_F_C*NBYTES_INT32> > int32_outc(k.out[0], outc);
        source(k) = "aie_kernels/matmult_int32.cpp";
        runtime<ratio>(k) = float(R/100.0);
    }
};

template<int R=100>
class MatMultInt16Graph : public graph {
private:
    kernel k;

public:
    port<input> ina, inb;
    port<output> outc;

    MatMultInt16Graph() {
        k = kernel::create(matmult_int16);

        connect< window<NSAMPLES_WINDOW_F_A*NBYTES_INT16> > int16_ina(ina, k.in[0]);
        connect< window<NSAMPLES_WINDOW_F_B*NBYTES_INT16> > int16_inb(inb, k.in[1]);
        connect< window<NSAMPLES_WINDOW_F_C*NBYTES_INT16> > int16_outc(k.out[0], outc);
        source(k) = "aie_kernels/matmult_int16.cpp";
        runtime<ratio>(k) = float(R/100.0);
    }
};

template<int R=100>
class MatMultInt8Graph : public graph {
private:
    kernel k;

public:
    port<input> ina, inb;
    port<output> outc;

    MatMultInt8Graph() {
        k = kernel::create(matmult_int8);

        connect< window<NSAMPLES_WINDOW_F_A*NBYTES_INT8> > int8_ina(ina, k.in[0]);
        connect< window<NSAMPLES_WINDOW_F_B*NBYTES_INT8> > int8_inb(inb, k.in[1]);
        connect< window<NSAMPLES_WINDOW_F_C*NBYTES_INT8> > int8_outc(k.out[0], outc);
        source(k) = "aie_kernels/matmult_int8.cpp";
        runtime<ratio>(k) = float(R/100.0);
    }
};


class TopGraph : public graph {
public:
    input_plio in[8];
    output_plio out[4];

    MatMultFloatGraph<100> FG;
    MatMultInt32Graph<100> I32G;
    MatMultInt16Graph<100> I16G;
    MatMultInt8Graph<100> I8G;

    TopGraph() {
        in[0] = input_plio::create("DataInFP_A", plio_64_bits, "data/inputa_float.txt");
        in[1] = input_plio::create("DataInFP_B", plio_64_bits, "data/inputb_float.txt");
        out[0] = output_plio::create("DataOutFP", plio_64_bits, "float_output.txt");

        in[2] = input_plio::create("DataInI32_A", plio_64_bits, "data/inputa_int32.txt");
        in[3] = input_plio::create("DataInI32_B", plio_64_bits, "data/inputb_int32.txt");
        out[1] = output_plio::create("DataOutI32", plio_64_bits, "int32_output.txt");

        in[4] = input_plio::create("DataInI16_A", plio_64_bits, "data/inputa_int16.txt");
        in[5] = input_plio::create("DataInI16_B", plio_64_bits, "data/inputb_int16.txt");
        out[2] = output_plio::create("DataOutI16", plio_64_bits, "int16_output.txt");

        in[6] = input_plio::create("DataInI8_A", plio_64_bits, "data/inputa_int8.txt");
        in[7] = input_plio::create("DataInI8_B", plio_64_bits, "data/inputb_int8.txt");
        out[3] = output_plio::create("DataOutI8", plio_64_bits, "int8_output.txt");

        connect <> net0(in[0].out[0], FG.ina);
        connect <> net1(in[1].out[0], FG.inb);
        connect <> net2(FG.outc, out[0].in[0]);

        connect <> net3(in[2].out[0], I32G.ina);
        connect <> net4(in[3].out[0], I32G.inb);
        connect <> net5(I32G.outc, out[1].in[0]);

        connect <> net6(in[4].out[0], I16G.ina);
        connect <> net7(in[5].out[0], I16G.inb);
        connect <> net8(I16G.outc, out[2].in[0]);

        connect <> net9(in[6].out[0], I8G.ina);
        connect <> net10(in[7].out[0], I8G.inb);
        connect <> net11(I8G.outc, out[3].in[0]);
    }
};
