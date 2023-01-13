# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

from scheme import Scheme


class Scheme32bx32b(Scheme):
    pass


if __name__ == "__main__":
    t = Scheme32bx32b(rows=8, cols=1, lanes=8)
    data = t.data(xstart=0, xoffset=0x00000000, xstep=0)
    coeff = t.coefficient(zstart=0, zoffset=0x7654321076543210, zstep=8)
    print(f'Data Scheme: \n{data}\n\nCoefficient Scheme: \n{coeff}\nmacs: '
          f'{t.macs}')