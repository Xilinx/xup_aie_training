# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import numpy as np

shape_a = (16, 8)
shape_b = (8, 8)


def mat2file_float(mat: np.array, file: str, sn: bool = False, plio: int = 64):
    """
    mat: matrix
    file: filename
    sn: scientific notation enabled
    plio: PLIO interface bitwidth
    """
    with open(file, 'w', newline='') as f:
        for i in range(mat.shape[0]):
            for ii in range(mat.shape[1]):
                if sn:
                    v = np.format_float_scientific(mat[i][ii], min_digits=9)
                else:
                    v = mat[i][ii]

                f.write(f'{v}')
                if ((ii + 1) % (plio//32)) == 0:
                    f.write('\n')
                else:
                    f.write(' ')


def mat2fileint(mat: np.array, file: str, plio: int = 64):
    """
    mat: matrix
    file: filename
    """
    cols = plio/(mat.dtype.itemsize*8)
    with open(file, 'w', newline='') as f:
        for i in range(mat.shape[0]):
            for ii in range(mat.shape[1]):
                f.write(f'{mat[i][ii]}')
                if (ii+1) % cols == 0:
                    f.write('\n')
                else:
                    f.write(' ')


def main():
    # float
    c0 = np.random.randint(2**4, 2**8)
    c1 = np.random.randint(2**6, 2**9)
    a = np.float32(np.trunc(np.random.rand(*shape_a) * c0))
    b = np.float32(np.trunc(np.random.rand(*shape_b) * c1))
    c = np.matmul(a, b)
    mat2file_float(a, "inputa_float.txt")
    mat2file_float(b, "inputb_float.txt")
    mat2file_float(c, "ref_outputc_float.txt", True)

    # int32
    a = np.random.randint(-1*2**12, 2**12, size=(shape_a), dtype=np.int32)
    b = np.random.randint(-1*2**12, 2**12, size=(shape_b), dtype=np.int32)
    c = np.matmul(a, b)
    mat2fileint(a, "inputa_int32.txt")
    mat2fileint(b, "inputb_int32.txt")
    mat2fileint(c, "ref_outputc_int32.txt")

    # 16-bit
    a = np.random.randint(-1*2**5, 2**5, size=(shape_a), dtype=np.int16)
    b = np.random.randint(-1*2**5, 2**5, size=(shape_b), dtype=np.int16)
    c = np.matmul(a, b)
    mat2fileint(a, "inputa_int16.txt")
    mat2fileint(b, "inputb_int16.txt")
    mat2fileint(c, "ref_outputc_int16.txt")

    # 8-bit
    a = np.random.randint(-1*2**4, 2**6, size=(shape_a), dtype=np.int8)
    b = np.random.randint(-1*2**4, 2**6, size=(shape_b), dtype=np.int8)
    c = np.matmul(a, b)
    mat2fileint(a, "inputa_int8.txt")
    mat2fileint(b, "inputb_int8.txt")
    mat2fileint(c, "ref_outputc_int8.txt")

    # Mixed precision
    # A is 16-bit and B is 8-bit
    """
    a = np.random.randint(-1*2**5, 2**5, size=(shape_a), dtype=np.int16)
    b = np.random.randint(-1*2**4, 2**6, size=(shape_b), dtype=np.int8)
    c = np.matmul(a, b)
    mat2fileint(a, "inputa_int_8_16.txt")
    mat2fileint(b, "inputb_int_8_16.txt")
    mat2fileint(c, "ref_outputc_int_8_16.txt")
    """


if __name__ == '__main__':
    main()
