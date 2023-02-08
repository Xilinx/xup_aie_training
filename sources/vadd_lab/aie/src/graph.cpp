// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <iostream>
#include <fstream>
#include "graph.hpp"

simpleGraph vadd_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char** argv) {
    vadd_graph.init();
#ifdef STREAM
    vadd_graph.run(512);
#else
    vadd_graph.run(1);
#endif
    vadd_graph.end();
    std::ifstream golden_file, aie_file;
    golden_file.open("../data/golden.txt");
    if(golden_file.fail()){
      std::cerr << "Error opening golden file." << endl;
      golden_file.close();
      return -1;
    }

#if defined(__X86SIM__)
    aie_file.open("x86simulator_output/output.txt");
#else
    aie_file.open("aiesimulator_output/output.txt");
#endif
    if(aie_file.fail()){
      std::cerr<<"Error opening produced file."<<endl;
      return -1;
    }

    std::string line_golden, line_aie;
    bool match = true;
    int count = 0;
    while (getline(golden_file, line_golden)){
        getline(aie_file, line_aie);
        if (aie_file.eof()){
            std::cerr << "AI Engine results are too short to match golden result" << std::endl;
            break;
        }
        while (line_aie[0]=='T')
            getline(aie_file, line_aie);
        if (std::stoi(line_golden) != std::stoi(line_aie)){
            match = false;
            break;
        }
        else
            count++;
    }
    std::cout << "Count reached: " << count << std::endl;
    if (!aie_file.eof()){
        getline(aie_file, line_aie);
        if (!aie_file.eof())
            std::cerr << "AI Engine results are too long to match golden result" << std::endl;
    }
    if (match)
        std::cout << "AI Engine results match golden result" << std::endl;
    else
        std::cout << "AI Engine results DO NOT match golden result" << std::endl;

    golden_file.close();
    aie_file.close();
    return 0;
}
#endif