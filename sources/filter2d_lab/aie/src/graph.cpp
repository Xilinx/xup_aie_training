// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "graph.hpp"

// Graph object
Filter2DGraph filter_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char** argv) {
    filter_graph.init();
    filter_graph.run(1);
    filter_graph.end();
    return 0;
}
#endif