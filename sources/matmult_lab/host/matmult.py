# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import argparse
import numpy as np

from xrt_binding import *
from ert_binding import *

import pyxrt
import pdb
import re

shape_a = (16, 8)
shape_b = (8, 8)
shape_c = (8, 8)
size_a, size_b, size_c = shape_a[0] * shape_a[1], shape_b[0] * shape_b[1], shape_c[0] * shape_c[1]

def main(xclbin: str = None, idx: int = 0):
    device = pyxrt.device(idx)
    xbin = pyxrt.xclbin(xclbin)
    uuid = device.load_xclbin(xbin)
    #memlist = xbin.get_mems()

    #pdb.set_trace()
    #rule = re.compile("mm2s*")

    pl_mm2s_fp_a = pyxrt.kernel(device, uuid, "mm2s:{mm2s_fp_a}")
    pl_mm2s_fp_b = pyxrt.kernel(device, uuid, "mm2s:{mm2s_fp_b}")
    s2mm_fp = pyxrt.kernel(device, uuid, "s2mm:{s2mm_fp}")

    print("Allocate and initialize buffers")
    in_buff_1 = pyxrt.bo(device, size_a, pyxrt.bo.normal, pl_mm2s_fp_a.group_id(0))
    in_buff_2 = pyxrt.bo(device, size_b, pyxrt.bo.normal, pl_mm2s_fp_b.group_id(0))
    out_buff_1 = pyxrt.bo(device, size_c, pyxrt.bo.normal, s2mm_fp.group_id(0))

    matrix_a = np.asarray(in_buff_1.map())
    matrix_b = np.asarray(in_buff_2.map())
    matrix_c = np.asarray(out_buff_1.map())

    print("Generate random values")
    c0 = np.random.randint(2**4, 2**8)
    c1 = np.random.randint(2**6, 2**9)
    matrix_a[:] = np.ndarray.flatten(np.float32(np.trunc(np.random.rand(shape_a[0], shape_a[1]) * c0)))
    matrix_b[:] = np.ndarray.flatten(np.float32(np.trunc(np.random.rand(shape_b[0], shape_b[1]) * c1)))

    print("Move ")

    in_buff_1.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, size_a, 0)
    in_buff_2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, size_b, 0)

    run_pl_mm2s_fp_a = pl_mm2s_fp_a(in_buff_1, None, size_a)
    run_pl_mm2s_fp_b = pl_mm2s_fp_b(in_buff_2, None, size_b)
    run_s2mm_fp = s2mm_fp(out_buff_1, None, size_c)

    run_pl_mm2s_fp_a.wait()
    run_pl_mm2s_fp_b.wait()
    run_s2mm_fp.wait()

    print("Get the output data from the device and validate it")
    out_buff_1.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE, size_c, 0)
    print(np.float32(np.reshape(matrix_c, shape_c)))
    print(matrix_c)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Run Matrix Multiplication on VCK5000")
    parser.add_argument("--xclbin", help="XCLBIN files", required=True)
    args = parser.parse_args()

    main(args.xclbin, 0)