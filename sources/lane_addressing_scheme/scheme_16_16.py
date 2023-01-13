# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

import numpy as np
from scheme import Scheme, offset_to_array, check_square


class Scheme16bx16b(Scheme):
    def data(self, xstart, xoffset, xstep, xsquare):
        offs = offset_to_array(xoffset, 16)
        square = offset_to_array(xsquare, 4)
        check_square(square)
        matrix = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(int(self.rows * self.cols)):
            c = int(i % self.cols)
            r = i // self.cols
            if (r % 2 == 0):
                try:
                    offset = offs[r]*2
                except IndexError:
                    print(f"error with: {r}")
            else:
                offset = offs[r]*2 + (offs[r-1] + 1)*2

            step = c//2*xstep + c % 2
            matrix[r][c] = f'x[{xstart + offset + step}]'

        return self._square(matrix, square)


if __name__ == "__main__":
    t = Scheme16bx16b(rows=16, cols=2, lanes=16)
    data = t.data(xstart=0, xoffset=0x5454545410101010,
                  xstep=0, xsquare=0x1010)
    coeff = t.coefficient(zstart=0, zoffset=0x7654321076543210, zstep=8)
    print(f'Data Scheme: \n{data}\n\nCoefficient Scheme: \n{coeff}\nmacs: '
          f'{t.macs}')