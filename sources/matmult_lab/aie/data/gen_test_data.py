# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import numpy as np
from write_file import mat2file_tile

shape_a = (16, 8)
shape_b = (8, 8)


def main():
    """Generate random matrices for different data types and write to files"""
    np.random.seed(12262023)
    # float
    c0 = np.random.randint(2**4, 2**8)
    c1 = np.random.randint(2**6, 2**9)
    a = np.float32(np.trunc(np.random.rand(*shape_a) * c0))
    b = np.float32(np.trunc(np.random.rand(*shape_b) * c1))
    c = np.matmul(a, b)
    mat2file_tile(a, 4, 2, "inputa_float.txt")
    mat2file_tile(b, 2, 4, "inputb_float.txt")
    mat2file_tile(c, 4, 4, "ref_outputc_float.txt", sn=True)

    # int32
    a = np.random.randint(-1*2**12, 2**12, size=(shape_a), dtype=np.int32)
    b = np.random.randint(-1*2**12, 2**12, size=(shape_b), dtype=np.int32)
    c = np.matmul(a, b)
    mat2file_tile(a, 4, 2, "inputa_int32.txt")
    mat2file_tile(b, 2, 4, "inputb_int32.txt")
    mat2file_tile(c, 4, 4, "ref_outputc_int32.txt")

    # 16-bit
    a = np.random.randint(-1*2**5, 2**5, size=(shape_a), dtype=np.int16)
    b = np.random.randint(-1*2**5, 2**5, size=(shape_b), dtype=np.int16)
    c = np.matmul(a, b)
    mat2file_tile(a, 4, 4, "inputa_int16.txt")
    mat2file_tile(b, 4, 8, "inputb_int16.txt")
    mat2file_tile(c, 4, 8, "ref_outputc_int16.txt")

    # 8-bit
    a = np.random.randint(-1*2**4, 2**6, size=(shape_a), dtype=np.int8)
    b = np.random.randint(-1*2**4, 2**6, size=(shape_b), dtype=np.int8)
    c = np.matmul(a, b)
    mat2file_tile(a, 4, 8, "inputa_int8.txt")
    mat2file_tile(b, 8, 4, "inputb_int8.txt")
    mat2file_tile(c, 4, 4, "ref_outputc_int8.txt")

    # Mixed precision
    # A is 16-bit and B is 8-bit
    """
    a = np.random.randint(-1*2**5, 2**5, size=(shape_a), dtype=np.int16)
    b = np.random.randint(-1*2**4, 2**6, size=(shape_b), dtype=np.int8)
    c = np.matmul(a, b)
    mat2file_tile(a,<Tile R>,<Tile C>, "inputa_int_8_16.txt")
    mat2file_tile(b,<Tile R>,<Tile C>, "inputb_int_8_16.txt")
    mat2file_tile(c,<Tile R>,<Tile C>, "ref_outputc_int_8_16.txt")
    """


if __name__ == '__main__':
    main()
