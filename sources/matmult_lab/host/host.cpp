// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <chrono>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include "xrt.h"
#include "experimental/xrt_kernel.h"

#define MATRIX_A_COLS 8
#define MATRIX_A_ROWS 16
#define MATRIX_B_COLS 8
#define MATRIX_B_ROWS 8

//#define DEBUG

template <size_t rows, size_t cols, typename T>
void print_matrix(T matrix[rows][cols], std::string name){
    std::cout << "Matrix " << name << std::endl;
    for (size_t r = 0; r < rows; r++){
        if (r == 0)
            std::cout << "[";
        std::cout << "[";
        for (size_t c = 0; c < cols; c++){
            std::cout << std::fixed << std::setprecision(5) << matrix[r][c];
            if (c < (cols - 1))
                 std::cout << ", ";
        }
        std::cout << "]";
        if (r < (rows - 1))
            std::cout << ",";
        else
            std::cout << "]" << std::endl;
        std::cout << std::endl;
    }
}

template <typename T>
class Matrix {
private:
    unsigned int cols_b;
    unsigned int rows_a;
    template <size_t rows, size_t cols>
    void init_matrix(T matrix[rows][cols]){
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++){
            double random = static_cast <T> (rand()) / static_cast <T> (RAND_MAX) * 10;
            matrix[r][c] = random * ((rand() % 2) ? -1 : 1); // include sign
        }
}

public:
    size_t matrix_a_size = sizeof(T) * MATRIX_A_ROWS * MATRIX_A_COLS;;
    size_t matrix_b_size = sizeof(T) * MATRIX_B_ROWS * MATRIX_B_COLS;;
    size_t matrix_c_size = sizeof(T) * MATRIX_A_ROWS * MATRIX_B_COLS;;
    T matrix_a[MATRIX_A_ROWS][MATRIX_A_COLS];
    T matrix_b[MATRIX_B_ROWS][MATRIX_B_COLS];
    T matrix_c[MATRIX_A_ROWS][MATRIX_B_COLS];
    T matrix_g[MATRIX_A_ROWS][MATRIX_B_COLS];

    Matrix(){
        init_matrix<MATRIX_A_ROWS, MATRIX_A_COLS>(matrix_a);
        init_matrix<MATRIX_B_ROWS, MATRIX_B_COLS>(matrix_b);
        cols_b = MATRIX_B_COLS; 
        rows_a = MATRIX_A_ROWS;
    }

    void matmult(){
        for (size_t c = 0; c < cols_b; c++){
            for (size_t r = 0; r < rows_a; r++){
                T inner_product = 0.0;
                for (size_t idx = 0; idx < cols_b; idx++){
                    inner_product += matrix_a[r][idx] * matrix_b[idx][c];
                }
            matrix_g[r][c] = static_cast <T>(inner_product);
            }
        }
    }

    int compare(){
        return memcmp(matrix_c, matrix_g, matrix_c_size);
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    // Get device index and download xclbin
    std::cout << "Open the device" << std::endl;
    auto device = xrt::device(0);
    std::string binaryFile = argv[1];
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    // Get reference to the kernels
    std::cout << "Get references to datamovers compute units" << std::endl;
    auto pl_mm2s_fp_a = xrt::kernel(device, uuid, "mm2s:{mm2s_fp_a}");
    auto pl_mm2s_fp_b = xrt::kernel(device, uuid, "mm2s:{mm2s_fp_b}");
    auto s2mm_fp = xrt::kernel(device, uuid, "s2mm:{s2mm_fp}");

    // Allocating the input size of sizeIn to MM2S
    Matrix<float> mat_float; 
    auto in_buff_1 = xrt::bo(device, mat_float.matrix_a_size, pl_mm2s_fp_a.group_id(0));
    auto in_buff_2 = xrt::bo(device, mat_float.matrix_b_size, pl_mm2s_fp_b.group_id(0));
    auto out_buff_1 = xrt::bo(device, mat_float.matrix_c_size, s2mm_fp.group_id(0));
  
    auto start = std::chrono::high_resolution_clock::now();
    mat_float.matmult();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration_sw = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "Time taken by function: " << duration_sw.count() << " nanoseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    in_buff_1.write(mat_float.matrix_a);
    in_buff_2.write(mat_float.matrix_b);
    
    // Synchronize input buffers data to device global memory
    in_buff_1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    in_buff_2.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Execute the compute units
    auto run_pl_mm2s_fp_a = pl_mm2s_fp_a(in_buff_1, nullptr, mat_float.matrix_a_size);
    auto run_pl_mm2s_fp_b = pl_mm2s_fp_b(in_buff_2, nullptr, mat_float.matrix_b_size);
    auto run_s2mm_fp = s2mm_fp(out_buff_1, nullptr, mat_float.matrix_c_size);
    
    // Wait for kernels to complete
    run_pl_mm2s_fp_a.wait();
#ifdef DEBUG    
    std::cout << "pl_mm2s_fp_a completed" << std::endl;
#endif
    run_pl_mm2s_fp_b.wait();
#ifdef DEBUG
    std::cout << "pl_mm2s_fp_b completed" << std::endl;
#endif
    run_s2mm_fp.wait();
#ifdef DEBUG
    std::cout << "s2mm_fp completed" << std::endl;
#endif

    // Synchronize the output buffer data from the device
    out_buff_1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Read output buffer data to local buffer
    out_buff_1.read(mat_float.matrix_c);
    stop = std::chrono::high_resolution_clock::now();
    auto duration_aie = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << "AIE Engine time: " << duration_aie.count() << " nanoseconds" << std::endl;

    std::cout << "Ratio: " << duration_aie.count()/duration_sw.count() << std::endl;
#ifdef DEBUG
    print_matrix<MATRIX_A_ROWS, MATRIX_A_COLS, float>(mat_float.matrix_a, "A");
    print_matrix<MATRIX_B_ROWS, MATRIX_B_COLS, float>(mat_float.matrix_b, "B");
    print_matrix<MATRIX_A_ROWS, MATRIX_B_COLS, float>(mat_float.matrix_g, "GOLDEN");
    print_matrix<MATRIX_A_ROWS, MATRIX_B_COLS, float>(mat_float.matrix_c, "C from AIE");
#endif

    std::string test = "FAILED";
    auto error  = mat_float.compare();

    if (error == 0)
        test = "PASSED";

    std::cout << "TEST " << test << std::endl;

    return error;
}
