# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import numpy as np


def write_file(file: str, mat):
    with open(file, 'w', newline='') as f:
        for e in mat:
            f.write("{}\n".format(e))


def main():
    size = 2048
    a = np.random.randint(-2**20, 2**20, size=size, dtype=np.int32)
    b = np.random.randint(-2**20, 2**20, size=size, dtype=np.int32)
    c = a + b
    write_file('input0.txt', a)
    write_file('input1.txt', b)
    write_file('golden.txt', c)


if __name__ == '__main__':
    main()
