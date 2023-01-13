# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

import numpy as np


def offset_to_array(offset, elements=8):
    offs = []
    for i in range(elements):
        val = offset & 0xF
        offs.append(val)
        offset = offset >> 4
    return offs


def check_square(square):
    for idx, value in enumerate(square):
        if value > 3:
            raise ValueError(f'Square value invalid. Value {value} in index '
                             f'{idx} is bigger than 3')


class Scheme:
    def __init__(self, rows:int, cols:int, lanes:int=8):
        self.rows = rows
        self.cols = cols
        self.lanes = lanes

    def data(self, xstart, xoffset, xstep=0):
        """General coefficient scheme"""
        offs = offset_to_array(xoffset, self.lanes)
        matrix = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(int(self.cols * self.rows)):
            c = int(i % self.cols)
            r = i // self.cols
            matrix[r][c] = f'x[{xstart + offs[r] + xstep*c}]'
        return matrix

    def coefficient(self, zstart, zoffset, zstep):
        """General coefficient scheme"""
        offs = offset_to_array(zoffset, self.lanes)
        matrix = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(int(self.cols * self.rows)):
            c = int(i % self.cols)
            r = i // self.cols
            matrix[r][c] = f'z[{zstart + offs[r] + zstep*c}]'
        return matrix

    @property
    def macs(self):
        return self.rows * self.cols

    def _square(self, matrix, square):
        """General square parameter"""
        matrix_sqr = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(self.rows//2):
            for j in range(self.cols//2):
                for idx, value in enumerate(square):
                    ro, co = idx//2, int(idx % 2)
                    ri, ci = value//2, int(value % 2)
                    matrix_sqr[2*i+ro][2*j+co] = matrix[2*i+ri][2*j+ci]

        return matrix_sqr
