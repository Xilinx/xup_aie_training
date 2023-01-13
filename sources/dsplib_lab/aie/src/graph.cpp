// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "graph.hpp"

DSPLibGraph dsp_sim_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)

int main(int argc, char ** argv){
   dsp_sim_graph.init();
   dsp_sim_graph.run(1);
   dsp_sim_graph.end();
   return 0;
}

#endif
