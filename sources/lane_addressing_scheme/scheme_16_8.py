# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

import numpy as np
from scheme import Scheme, offset_to_array, check_square
from scheme_16_16 import Scheme16bx16b


class Scheme16bx8b(Scheme16bx16b):
    def coefficient(self, zstart, zoffset, zstep, zsquare):
        offs = offset_to_array(zoffset, self.lanes)
        square = offset_to_array(zsquare, 4)
        check_square(square)
        matrix = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(int(self.cols * self.rows)):
            c = i % self.cols
            r = i // self.cols
            offset = offs[r]*2
            step = c//2*zstep + (c % 2)
            matrix[r][c] = f'z[{zstart + offset + step}]'

        return self._square(matrix, square)


if __name__ == "__main__":
    t = Scheme16bx8b(rows=16, cols=2, lanes=16)
    data = t.data(xstart=0, xoffset=0x5454545410101010,
                  xstep=0, xsquare=0x1010)
    coeff = t.coefficient(zstart=0, zoffset=0x7654321076543210, zstep=1,
                          zsquare=0x3210)
    print(f'Data Scheme: \n{data}\n\nCoefficient Scheme: \n{coeff}\nmacs: '
          f'{t.macs}')