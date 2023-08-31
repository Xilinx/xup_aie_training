// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef TILED_MM2S_HPP
#define TILED_MM2S_HPP

#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

namespace {

/**
 * Reads a single from memory and writes it into a stream in blocks of `tile_rows` by
 * `tile_cols`.
 * Both input matrix and output blocks are represented in row-major order.
 */
template <unsigned rows, unsigned cols, unsigned tile_rows, unsigned tile_cols, unsigned data_width>
void tile_matrix(ap_int<data_width> *mem, hls::stream<ap_axiu<data_width, 0, 0, 0>> &stream)
{
    constexpr unsigned I = rows / tile_rows;
    constexpr unsigned J = cols / tile_cols;

    using data = ap_axiu<data_width, 0, 0, 0>;

    for (unsigned i = 0; i < I; ++i) {
        for (unsigned j = 0; j < J; ++j) {
            for (unsigned ti = 0; ti < tile_rows; ++ti) {
                for (unsigned tj = 0; tj < tile_cols; ++tj) {
                    unsigned index = (i * tile_rows + ti) * cols + j * tile_cols + tj;

                    data x { mem[index] };
                    x.keep_all();
                    stream.write(x);
                }
            }
        }
    }
}

/**
 * Reads multiple matrices from memory and writes them into a stream in blocks of `tile_rows` by `tile_cols`.
 * Stream and memory accesses are performed in chunks of `data_width` bits, which is usually the number of bits of each
 * matrix's element.
 */
template <unsigned rows, unsigned cols, unsigned tile_rows, unsigned tile_cols, unsigned data_width>
void mm2s_tiled(ap_int<data_width> *mem, hls::stream<ap_axiu<data_width, 0, 0, 0>> &s, int size_bytes)
{
    constexpr unsigned element_size = data_width / CHAR_BIT;
    constexpr unsigned matrix_size  = rows * cols;
    const unsigned     elements     = size_bytes / element_size;

    for (unsigned offset = 0; offset < elements; offset += matrix_size) {
        tile_matrix<rows, cols, tile_rows, tile_cols, data_width>(mem + offset, s);
    }
}

} // namespace

#endif // TILED_MM2S_HPP
