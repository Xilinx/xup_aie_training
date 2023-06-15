// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#pragma once

#include "aie_kernels.h"
#include "system_settings.h"

#include <adf.h>

template<int R=100>
class MatMultFloatGraph : public adf::graph {
private:
  adf::kernel k;

public:
  adf::port<adf::input> ina, inb;
  adf::port<adf::output> outc;

  MatMultFloatGraph() {
    using namespace adf;
    k = kernel::create(matmult_float);

    connect<window<NSAMPLES_WINDOW_F_A * NBYTES_FLOAT>> float_ina(ina, k.in[0]);
    connect<window<NSAMPLES_WINDOW_F_B * NBYTES_FLOAT>> float_inb(inb, k.in[1]);
    connect<window<NSAMPLES_WINDOW_F_C * NBYTES_FLOAT>> float_outc(k.out[0], outc);
    source(k) = "aie_kernels/matmult_float.cpp";
    runtime<ratio>(k) = float(R/100.0);
  }
};

template<int R=100>
class MatMultInt32Graph : public adf::graph {
private:
  adf::kernel k;

public:
  adf::port<adf::input> ina, inb;
  adf::port<adf::output> outc;

  MatMultInt32Graph() {
    using namespace adf;
    k = kernel::create(matmult_int32);

    connect<window<NSAMPLES_WINDOW_F_A * NBYTES_INT32>> int32_ina(ina, k.in[0]);
    connect<window<NSAMPLES_WINDOW_F_B * NBYTES_INT32>> int32_inb(inb, k.in[1]);
    connect<window<NSAMPLES_WINDOW_F_C * NBYTES_INT32>> int32_outc(k.out[0], outc);
    source(k) = "aie_kernels/matmult_int32.cpp";
    runtime<ratio>(k) = float(R/100.0);
  }
};

template<int R=100>
class MatMultInt16Graph : public adf::graph {
private:
  adf::kernel k;

public:
  adf::port<adf::input> ina, inb;
  adf::port<adf::output> outc;

  MatMultInt16Graph() {
    using namespace adf;
    k = kernel::create(matmult_int16);

    connect<window<NSAMPLES_WINDOW_F_A * NBYTES_INT16>> int16_ina(ina, k.in[0]);
    connect<window<NSAMPLES_WINDOW_F_B * NBYTES_INT16>> int16_inb(inb, k.in[1]);
    connect<window<NSAMPLES_WINDOW_F_C * NBYTES_INT16>> int16_outc(k.out[0], outc);
    source(k) = "aie_kernels/matmult_int16.cpp";
   runtime<ratio>(k) = float(R/100.0);
  }
};

template<int R=100>
class MatMultInt8Graph : public adf::graph {
private:
  adf::kernel k;

public:
  adf::port<adf::input> ina, inb;
  adf::port<adf::output> outc;

  MatMultInt8Graph() {
    using namespace adf;
    k = kernel::create(matmult_int8);

    connect<window<NSAMPLES_WINDOW_F_A * NBYTES_INT8>> int8_ina(ina, k.in[0]);
    connect<window<NSAMPLES_WINDOW_F_B * NBYTES_INT8>> int8_inb(inb, k.in[1]);
    connect<window<NSAMPLES_WINDOW_F_C * NBYTES_INT8>> int8_outc(k.out[0], outc);
    source(k) = "aie_kernels/matmult_int8.cpp";
    runtime<ratio>(k) = float(R/100.0);
  }
};

class TopGraph : public adf::graph {
public:
  static constexpr unsigned num_input = 8, num_output = 4;
  std::array<adf::input_plio, num_input> in;
  std::array<adf::output_plio, num_output> out;

  MatMultFloatGraph<100> FG;
  MatMultInt32Graph<100> I32G;
  MatMultInt16Graph<100> I16G;
  MatMultInt8Graph<100> I8G;

  TopGraph()
      : TopGraph({"DataInFP_A", "DataInFP_B", "DataInI32_A", "DataInI32_B",
                  "DataInI16_A", "DataInI16_B", "DataInI8_A", "DataInI8_B"},
                 {"data/inputa_float.txt", "data/inputb_float.txt",
                  "data/inputa_int32.txt", "data/inputb_int32.txt",
                  "data/inputa_int16.txt", "data/inputb_int16.txt",
                  "data/inputa_int8.txt", "data/inputb_int8.txt"},
                 {"DataOutFP", "DataOutI32", "DataOutI16", "DataOutI8"},
                 {"float_output.txt", "int32_output.txt", "int16_output.txt",
                  "int8_output.txt"}) {}

private:
  TopGraph(const std::array<const char *, num_input> &input_names,
           const std::array<const char *, num_input> &input_files,
           const std::array<const char *, num_output> &output_names,
           const std::array<const char *, num_output> &output_files) {
    using namespace adf;
    for (unsigned i = 0; i < in.size(); ++i) {
      in[i] = input_plio::create(input_names[i], plio_64_bits, input_files[i]);
    }
    for (unsigned i = 0; i < out.size(); ++i) {
      out[i] =
          output_plio::create(output_names[i], plio_64_bits, output_files[i]);
    }

    connect<> netlist[] = {
        {in[0].out[0], FG.ina},    {in[1].out[0], FG.inb},
        {FG.outc, out[0].in[0]},

        {in[2].out[0], I32G.ina},  {in[3].out[0], I32G.inb},
        {I32G.outc, out[1].in[0]},

        {in[4].out[0], I16G.ina},  {in[5].out[0], I16G.inb},
        {I16G.outc, out[2].in[0]},

        {in[6].out[0], I8G.ina},   {in[7].out[0], I8G.inb},
        {I8G.outc, out[3].in[0]},
    };
  }
};
