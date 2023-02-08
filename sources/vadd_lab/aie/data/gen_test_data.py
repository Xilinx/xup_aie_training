# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import numpy as np


def write_file(file: str, data, plio: int=128):
    with open(file, 'w', newline='') as f:
        for idx, v in enumerate(data):
            f.write(f"{v} ")
            if ((idx + 1) % (plio//32)) == 0:
                f.write("\n")


def main():
    size = 2048
    a = np.random.randint(-2**20, 2**20, size=size, dtype=np.int32)
    b = np.random.randint(-2**20, 2**20, size=size, dtype=np.int32)
    c = a + b
    write_file('input0.txt', a, 32)
    write_file('input1.txt', b, 32)
    write_file('golden.txt', c, 32)


if __name__ == '__main__':
    main()
