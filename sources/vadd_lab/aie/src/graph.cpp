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
#if defined(STREAM)
    vadd_graph.run(512);
#else
    vadd_graph.run(1);
#endif
    vadd_graph.end();
    std::ifstream file1, file2;
    file1.open("../data/golden.txt");
    if(file1.fail()){
      std::cerr << "Error opening golden file." << endl;
      file1.close();
      return -1;
    }

#if defined(__X86SIM__)
    file2.open("x86simulator_output/output.txt");
#else
    file2.open("aiesimulator_output//output.txt");
#endif
    if(file2.fail()){
      std::cerr<<"Error opening produced file."<<endl;
      return -1;
    }

    std::string line1, line2;
    bool match = true;
    while (getline(file1, line1)){
        getline(file2, line2);
        if (file2.eof()){
            std::cerr << "AI Engine results are too short to match golden result" << std::endl;
            break;
        }
        if (line2[0]=='T')
            getline(file2, line2);
        if (std::stoi(line1) != std::stoi(line2)){
            match = false;
            break;
        }
    }
    if (!file2.eof()){
        getline(file2, line2);
        if (!file2.eof())
            std::cerr << "AI Engine results are too long to match golden result" << std::endl;
    }
    if (match)
        std::cout << "AI Engine results match golden result" << std::endl;
    else
        std::cout << "AI Engine results DO NOT match golden result" << std::endl;

    file1.close();
    file2.close();
    return 0;
}
#endif