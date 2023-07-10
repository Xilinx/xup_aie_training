# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import numpy as np

shape_a = (16, 8)
shape_b = (8, 8)


def mat2file(mat: np.array, file: str, plio: int = 64, sn: bool = False):
    """ Write a numpy int matrix to file

    mat: matrix
    file: filename
    plio: PLIO interface bitwidth
    sn: scientific notation enabled
    """

    cols = plio//(mat.dtype.itemsize*8)
    with open(file, 'w', newline='', encoding="utf-8") as f:
        for i in range(mat.shape[0]):
            for ii in range(mat.shape[1]):
                if mat.dtype == np.float32 and sn:
                    v = np.format_float_scientific(mat[i][ii], min_digits=9)
                else:
                    v = mat[i][ii]
                f.write(f'{v}')
                if (ii+1) % cols == 0:
                    f.write('\n')
                else:
                    f.write(' ')


def mat2file_tile(mat: np.array, R: int, C: int, file: str,
                  plio: int = 64, sn: bool = False):
    """ Write a numpy int matrix to file using a tiling scheme

    mat: matrix
    R: Rows of tile size
    C: Columns of tile size
    file: filename
    plio: PLIO interface bitwidth
    sn: scientific notation enabled
    """

    cols = plio//(mat.dtype.itemsize*8)
    with open(file, 'w', newline='', encoding="utf-8") as f:
        idx = 0
        for r in range(0, mat.shape[0], R):
            for c in range(0, mat.shape[1], C):
                for rr in range(r, r+R):
                    for cc in range(c, c+C):
                        if mat.dtype == np.float32 and sn:
                            v = np.format_float_scientific(mat[rr][cc],
                                                           min_digits=9)
                        else:
                            v = mat[rr][cc]
                        f.write(f'{v}')
                        if (idx + 1) % cols == 0:
                            idx = 0
                            f.write('\n')
                        else:
                            idx += 1
                            f.write(' ')


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
