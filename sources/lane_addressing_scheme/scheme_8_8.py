# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

import numpy as np
from scheme import Scheme, offset_to_array, check_square


class Scheme8bx8b(Scheme):
    def data(self, xstart, xoffset, xstep, xsquare):
        offs = offset_to_array(xoffset)
        square = offset_to_array(xsquare, 4)
        check_square(square)
        matrix = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(int(self.rows * self.cols)):
            c = int(i % self.cols)
            r = i // self.cols
            rx = r // 2
            rr = int(r % 4)

            if rr == 0:
                offset = offs[rx]*4
            elif rr == 1:
                offset = offs[rx]*4 + 1
            elif rr == 2:
                offset = offs[rx]*4 + (offs[rx-1] + 1) * 4
            else:
                offset = offs[rx]*4 + (offs[rx-1] + 1) * 4 + 1

            step = c//2*xstep + (c % 2)*2
            matrix[r][c] = f'x[{xstart + offset + step}]'

        return self._square(matrix, square)

    def coefficient(self, zstart, zoffset, zstep, zsquare):
        offs = offset_to_array(zoffset)
        square = offset_to_array(zsquare, 4)
        check_square(square)
        matrix = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(int(self.cols * self.rows)):
            c = i % self.cols
            r = i // self.cols
            rz = int(r // 4) * 2 + int(r % 2)
            offset = offs[rz]*2
            step = c//2*zstep + (c % 2)
            matrix[r][c] = f'z[{zstart + offset + step}]'

        return self._square(matrix, square)

    def _square(self, matrix, square):
        matrix_sqr = np.chararray((self.rows, self.cols), itemsize=5)
        for i in range(self.rows//4):
            for j in range(self.cols//2):
                for idx, value in enumerate(square):
                    ro, co = (idx//2)*2, int(idx % 2)
                    ri, ci = (value//2)*2, int(value % 2)
                    matrix_sqr[4*i+ro][2*j+co] = matrix[4*i+ri][2*j+ci]
                    matrix_sqr[4*i+ro+1][2*j+co] = matrix[4*i+ri+1][2*j+ci]
        return matrix_sqr


if __name__ == "__main__":
    t = Scheme8bx8b(rows=16, cols=8, lanes=8)
    data = t.data(xstart=0, xoffset=0x11101110, xstep=16, xsquare=0x3120)
    coeff = t.coefficient(zstart=0, zoffset=0x44440000, zstep=2,
                          zsquare=0x3210)
    print(f'Data Scheme: \n{data}\n\nCoefficient Scheme: \n{coeff}\nmacs: '
          f'{t.macs}')
