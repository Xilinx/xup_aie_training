// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "matrix.h"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <typeinfo>

#include <xrt.h>
#include <experimental/xrt_kernel.h>

#define MATRIX_A_COLS 8
#define MATRIX_A_ROWS 16
#define MATRIX_B_COLS 8
#define MATRIX_B_ROWS 8

//#define DEBUG

/**
 * PL kernel names to exchange matrices between host and AIE array
 */
struct PLKernelNames {
    std::string a, b, c;
};

/**
 * Returns the input and output PL kernel names for the element type T
 */
template <typename T>
const PLKernelNames &kernelNames() = delete;

template <>
const PLKernelNames &kernelNames<float>() {
    static PLKernelNames names{"mm2s_i32_a:{mm2s_fp_a}", "mm2s_i32_b:{mm2s_fp_b}", "s2mm_i32:{s2mm_fp}"};
    return names;
}

template <>
const PLKernelNames &kernelNames<int32_t>() {
    static PLKernelNames names{"mm2s_i32_a:{mm2s_i32_a}", "mm2s_i32_b:{mm2s_i32_b}", "s2mm_i32:{s2mm_i32}"};
    return names;
}

template <>
const PLKernelNames &kernelNames<int16_t>() {
    static PLKernelNames names{"mm2s_i16_a:{mm2s_i16_a}", "mm2s:{mm2s_i16_b}", "s2mm:{s2mm_i16}"};
    return names;
}

template <>
const PLKernelNames &kernelNames<int8_t>() {
    static PLKernelNames names{"mm2s:{mm2s_i8_a}", "mm2s_i8_b:{mm2s_i8_b}", "s2mm_i8:{s2mm_i8}"};
    return names;
}

/**
 * Generates random numbers of type T. Used to initialize matrices A and B.
 */
template <typename T>
class RNG {
private:
    using uniform_distribution_t = typename std::conditional<std::is_floating_point<T>::value,
                                                             std::uniform_real_distribution<T>,
                                                             std::uniform_int_distribution<T>>::type;
    std::default_random_engine rng;
    uniform_distribution_t dist;
public:
    RNG(T min, T max) :
#ifdef DEBUG
        rng(),  // don't change seed in DEBUG mode
#else
        rng(time(nullptr)), // use a different seed to get different values each time
#endif
        dist(min, max) // generate numbers between these two values
    {
    }

    /**
     * Generate numbers between min and max values
     */
    T operator()() { return dist(rng); }
};

/**
 * Performs multiplication of two matrices A and B either at the host CPU or the AIE.
 */
template <typename T>
class MatrixMultiplication {
public:
    Matrix<T, MATRIX_A_ROWS, MATRIX_A_COLS> a;
    Matrix<T, MATRIX_B_ROWS, MATRIX_B_COLS> b;
    Matrix<T, MATRIX_A_ROWS, MATRIX_B_COLS> c;
    Matrix<T, MATRIX_A_ROWS, MATRIX_B_COLS> g;
private:
    xrt::kernel mm2s_a;
    xrt::kernel mm2s_b;
    xrt::kernel s2mm;
    xrt::bo in_buff_1;
    xrt::bo in_buff_2;
    xrt::bo out_buff_1;

    MatrixMultiplication(const xrt::device &device, const xrt::uuid &uuid, RNG<T> &&generator) :
        a(generator),
        b(generator),
        c(),
        g(),
        mm2s_a(xrt::kernel(device, uuid, kernelNames<T>().a)),
        mm2s_b(xrt::kernel(device, uuid, kernelNames<T>().b)),
        s2mm(xrt::kernel(device, uuid, kernelNames<T>().c)),
        in_buff_1(xrt::bo(device, a.size_bytes(), mm2s_a.group_id(0))),
        in_buff_2(xrt::bo(device, b.size_bytes(), mm2s_b.group_id(0))),
        out_buff_1(xrt::bo(device, c.size_bytes(), s2mm.group_id(0)))
    {
    }

public:
    MatrixMultiplication(const xrt::device &device, const xrt::uuid &uuid) :
        MatrixMultiplication(device, uuid, RNG<T>{-128, 127})
    {
    }

    /**
     * Performs matrix multiplication in the host CPU and stores the result in g
     */
    void matmulsw(){
        g = a * b;
    }

    /**
     * Offloads matrix multiplication to the AIE accelerator and stores the result in c
     */
    void matmulhw(){
        // Write matrices to ACAP device
        in_buff_1.write(a.matrix);
        in_buff_2.write(b.matrix);
        // Synchronize input buffers data to device global memory
        in_buff_1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        in_buff_2.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        // Execute the compute units
        auto a_to_device = mm2s_a(in_buff_1, nullptr, a.size_bytes());
        auto b_to_device = mm2s_b(in_buff_2, nullptr, b.size_bytes());
        auto c_to_host = s2mm(out_buff_1, nullptr, c.size_bytes());
        // Wait for kernels to complete
        a_to_device.wait();
        #ifdef DEBUG
        std::cout << "a_to_device completed" << std::endl;
        #endif
        b_to_device.wait();
        #ifdef DEBUG
        std::cout << "b_to_device completed" << std::endl;
        #endif
        c_to_host.wait();
        #ifdef DEBUG
        std::cout << "c_to_host completed" << std::endl;
        #endif
        // Synchronize the output buffer data from the device
        out_buff_1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        // Read output buffer data to local buffer
        out_buff_1.read(c.matrix);
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

    // Create Matrix object for float datatype
    MatrixMultiplication<float> mat_float(device, uuid);

    std::chrono::nanoseconds duration_sw, duration_aie;
    auto start = std::chrono::high_resolution_clock::now();
    mat_float.matmulsw();
    auto stop = std::chrono::high_resolution_clock::now();
    duration_sw = stop - start;

    start = std::chrono::high_resolution_clock::now();
    mat_float.matmulhw();
    stop = std::chrono::high_resolution_clock::now();
    duration_aie = stop - start;

    std::cout
        << "Time taken by function: " << duration_sw.count() << " nanoseconds\n"
        << "AIE Engine time: " << duration_aie.count() << " nanoseconds\n"
        << "Ratio: " << duration_aie.count()/duration_sw.count() << std::endl;
#ifdef DEBUG
    std::cout
        << "A:\n" << mat_float.a
        << "B:\n" << mat_float.b
        << "GOLDEN:\n" << mat_float.g
        << "C from AIE:\n" << mat_float.c;
#endif

    bool pass_fp = mat_float.c == mat_float.g;
    std::cout << "TEST for float matrices " << (!pass_fp ? "FAILED" : "PASSED") << std::endl;

    MatrixMultiplication<int32_t> mat_i32(device, uuid);
    MatrixMultiplication<int16_t> mat_i16(device, uuid);
    MatrixMultiplication<int8_t> mat_i8(device, uuid);

    mat_i32.matmulsw();
    mat_i16.matmulsw();
    mat_i8.matmulsw();

    mat_i32.matmulhw();
    mat_i16.matmulhw();
    mat_i8.matmulhw();

    bool pass_i32 = mat_i32.c == mat_i32.g;
    bool pass_i16 = mat_i16.c == mat_i16.g;
    bool pass_i8 = mat_i8.c == mat_i8.g;

    std::cout << "TEST for int32 matrices " << (!pass_i32 ? "FAILED" : "PASSED") << std::endl;
    std::cout << "TEST for int16 matrices " << (!pass_i16 ? "FAILED" : "PASSED") << std::endl;
    std::cout << "TEST for int8 matrices " << (!pass_i8? "FAILED" : "PASSED") << std::endl;

    return !pass_fp + !pass_i32 + !pass_i16 + !pass_i8;
}
