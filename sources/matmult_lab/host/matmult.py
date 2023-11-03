# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import argparse
import numpy as np
import pyxrt

shape_a = (16, 8)
shape_b = (8, 8)
shape_c = (shape_a[0], shape_b[1])

kernels = {
    'np.float32': {
        'mm2s': ['mm2s:{mm2s_fp_a}', 'mm2s:{mm2s_fp_b}'],
        's2mm': 's2mm:{s2mm_fp}',
        'dtype': np.float32
    },
    'np.int32': {
        'mm2s': ['mm2s:{mm2s_i32_a}', 'mm2s:{mm2s_i32_b}'],
        's2mm': 's2mm:{s2mm_i32}',
        'dtype': np.int32
    },
    'np.int16': {
        'mm2s': ['mm2s:{mm2s_i16_a}', 'mm2s:{mm2s_i16_b}'],
        's2mm': 's2mm:{s2mm_i16}',
        'dtype': np.int16
    },
    'np.int8': {
        'mm2s': ['mm2s:{mm2s_i8_a}', 'mm2s:{mm2s_i8_b}'],
        's2mm': 's2mm:{s2mm_i8}',
        'dtype': np.int8
    }
}


class AIEMatMult:
    """Run Matrix Multiplication on the VCK5000 for multiple datatypes"""

    def __init__(self, device: pyxrt.xclbin, uuid: pyxrt.uuid,
                 datatype: str = 'np.int8') -> None:

        if datatype not in (keys := kernels.keys()):
            raise TypeError(f'Datatype \'{datatype}\' is not supported. '
                            f'Supported datatypes are: {list(keys)}')

        self.mm2s_a = pyxrt.kernel(device, uuid, kernels[datatype]['mm2s'][0])
        self.mm2s_b = pyxrt.kernel(device, uuid, kernels[datatype]['mm2s'][1])
        self.s2mm = pyxrt.kernel(device, uuid, kernels[datatype]['s2mm'])
        self._datatype = datatype
        self.dtype = kernels[datatype]['dtype']
        itemsize = np.dtype(self.dtype).itemsize

        if self._datatype == 'np.float32':
            c0 = np.random.randint(2**4, 2**8)
            c1 = np.random.randint(2**6, 2**9)
            self.mata = np.float32(np.random.rand(shape_a[0], shape_a[1]) * c0)
            self.matb = np.float32(np.random.rand(shape_b[0], shape_b[1]) * c1)
        else:
            bitw = itemsize * 8 - 1
            self.mata = np.random.randint(-(2**bitw), (2**bitw)-1,
                                          size=shape_a, dtype=self.dtype)
            self.matb = np.random.randint(-(2**bitw), (2**bitw)-1,
                                          size=shape_b, dtype=self.dtype)

        self.matc = np.zeros(shape_c, dtype=np.dtype)

        self._size_a = self.mata.size * itemsize
        self._size_b = self.matb.size * itemsize
        self._size_c = self.matc.size * itemsize
        self.buf_a = pyxrt.bo(device, self._size_a,
                              pyxrt.bo.normal, self.mm2s_a.group_id(0))
        self.buf_b = pyxrt.bo(device, self._size_b,
                              pyxrt.bo.normal, self.mm2s_b.group_id(0))
        self.buf_c = pyxrt.bo(device, self._size_c,
                              pyxrt.bo.normal, self.s2mm.group_id(0))

        self.buf_a.write(self.mata, 0)
        self.buf_b.write(self.matb, 0)

    def matmult(self, debug: bool = False) -> bool:
        """Run Matrix Multiplication on the VCK5000"""
        self.buf_a.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE,
                        self._size_a, 0)
        self.buf_b.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE,
                        self._size_b, 0)

        run_mm2s_a = self.mm2s_a(self.buf_a, None, self._size_a)
        run_mm2s_b = self.mm2s_b(self.buf_b, None, self._size_b)
        run_s2mm = self.s2mm(self.buf_c, None, self._size_c)

        run_mm2s_a.wait()
        run_mm2s_b.wait()
        run_s2mm.wait()

        self.buf_c.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                        self._size_c, 0)

        self.matc = np.reshape(np.frombuffer(self.buf_c.read(self._size_c, 0),
                               dtype=self.dtype), shape_c)

        golden = np.matmul(self.mata, self.matb)
        if self._datatype == 'np.float32':
            equal = np.allclose(golden, self.matc, 0.1, 0.1)
        else:
            equal = np.array_equal(golden, self.matc)

        print(f"AIE result for \'{self._datatype}\' match golden? {equal}")

        if debug:
            print(f'{self.mata=}\n\n{self.matb=}\n\n'
                  f'{golden=}\n\n{self.matc=}')

        return equal


def main(xclbin: str = None, idx: int = 0):
    """Run Matrix Multiplication on the VCK5000"""
    device = pyxrt.device(idx)
    xbin = pyxrt.xclbin(xclbin)
    uuid = device.load_xclbin(xbin)

    fp = AIEMatMult(device, uuid, 'np.float32')
    fp.matmult()
    i32 = AIEMatMult(device, uuid, 'np.int32')
    i32.matmult()
    i16 = AIEMatMult(device, uuid, 'np.int16')
    i16.matmult()
    i8 = AIEMatMult(device, uuid, 'np.int8')
    i8.matmult()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Run Matrix Multiplication on VCK5000")
    parser.add_argument("--xclbin", help="XCLBIN files", required=True)
    parser.add_argument("--device", type=int, default=0,
                        help="Device Index", required=False)
    args = parser.parse_args()

    main(args.xclbin, args.device)
