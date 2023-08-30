// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <cstring>
#include <functional>
#include <iostream>
#include <iomanip>
#include <random>

/**
 * Contains 2D array and provides basic functions such as initialization, multiplication and formatted output.
 */
template <typename T, unsigned rows, unsigned cols>
class Matrix {
public:
    // Constructs a matrix with undefined values
    Matrix() = default;

    // Constructs a matrix whose values are initialized by a generator function
    explicit Matrix(const std::function<T()> &generator) {
        for (unsigned r = 0; r < rows; r++) {
            for (unsigned c = 0; c < cols; c++) {
                matrix[r][c] = generator();
            }
        }
    }

    // Returns dimensions for this matrix
    std::array<unsigned, 2> size() const { return { rows, cols }; }

    // Returns number of elements in the matrix
    unsigned elements() const { return rows * cols; }

    // Returns total number of bytes
    unsigned size_bytes() const { return elements() * sizeof(T); }

    // Compares two matrix objects for equality
    bool operator==(const Matrix &other) const {
        const T *left = &matrix[0][0], *right = &other.matrix[0][0];
        auto same_element_value = [](T lhs, T rhs) -> bool {
            if (std::is_floating_point<T>::value) {
                // Floating point addition and multiplication are not commutative
                // Elements may not be identical binary-wise if we change the order of the operations
                T relative_error = std::abs((rhs - lhs) / lhs);
                return relative_error < 0.001f;
            } else {
                return lhs == rhs;
            }
        };
        return std::equal(left, left + elements(), right, same_element_value);
    }

    // Performs a basic matrix-matrix multiplication and returns a new matrix with the result
    template <unsigned cols_b>
    Matrix<T, rows, cols_b> operator*(const Matrix<T, cols, cols_b> &b) const {
        Matrix<T, rows, cols_b> result;
        for (unsigned c = 0; c < cols_b; c++) {
            for (unsigned r = 0; r < rows; r++) {
                T inner_product = 0;
                for (unsigned idx = 0; idx < cols_b; idx++) {
                    inner_product += matrix[r][idx] * b.matrix[idx][c];
                }
                result.matrix[r][c] = inner_product;
            }
        }
        return result;
    }

    T matrix[rows][cols];
};

// Formatted output of a matrix
template <typename T, unsigned rows, unsigned cols>
inline std::ostream &operator<<(std::ostream &os, const Matrix<T, rows, cols> &m) {
    constexpr unsigned w = std::is_floating_point<T>::value? 12 : 6;
    std::ios::fmtflags osflags = os.flags();
    os.precision(5);
    os.setf(std::ios::fixed);
    const char *sep = "[[";
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            auto value = +m.matrix[r][c]; // promote uint8_t/char to integral
            os << sep << std::setw(w) << value;
            sep = ", ";
        }
        sep = "],\n[ ";
    }
    os << "]]\n";
    os.flags(osflags);
    return os;
}

#endif // MATRIX_H
