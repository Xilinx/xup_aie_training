# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import numpy as np


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
