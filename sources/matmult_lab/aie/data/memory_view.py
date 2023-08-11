# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

import numpy as np
from write_file import mat2file_tile
import argparse


def gen_mat_text(name: str = 'a', shape: tuple = (16, 8)) -> np.chararray:
    matrix = np.chararray((shape[0], shape[1]), itemsize=5)
    for r in range(shape[0]):
        for c in range(shape[1]):
            matrix[r][c] = f'{name}{r},{c}'

    return matrix


def main(name: str = 'a', shape: tuple = (16, 8), R: int = 4, C: int = 4,
         show: bool = True) -> None:
    mat_a = gen_mat_text(name, shape)
    filename = f'mat{name}_memory_view.txt'
    mat2file_tile(mat_a, R, C, filename, plio=mat_a.dtype.itemsize * 8 * 8)
    if show:
        with open(filename, 'r') as f:
            txt = f.read()
            print(txt)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description="Generate AIE memory view of a tiled matrix")
    parser.add_argument("--name", help="Name of the Matrix", required=False,
                        type=str, choices=['a', 'b', 'c'], default='a')
    parser.add_argument("--rows", help="Number of Rows", required=True,
                        type=int)
    parser.add_argument("--cols", help="Number of Columns", required=True,
                        type=int)
    parser.add_argument("--R", help="Tile height", required=True,
                        type=int)
    parser.add_argument("--C", help="Tile width", required=True,
                        type=int)
    parser.add_argument("-p", help="Display in console", action='store_true',
                        required=False, default=False)
    args = parser.parse_args()
    shape = (args.rows, args.cols)
    main(name=args.name, shape=shape, R=args.R, C=args.C, show=args.p)
