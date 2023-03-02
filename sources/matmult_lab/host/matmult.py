# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import argparse
import numpy as np
import pyxrt

shape_a = (16, 8)
shape_b = (8, 8)
shape_c = (shape_a[0], shape_b[1])


def main(xclbin: str = None, idx: int = 0):
    """Run Matrix Multiplication on the VCK5000"""
    device = pyxrt.device(idx)
    xbin = pyxrt.xclbin(xclbin)
    uuid = device.load_xclbin(xbin)

    mm2s_fp_a = pyxrt.kernel(device, uuid, "mm2s:{mm2s_fp_a}")
    mm2s_fp_b = pyxrt.kernel(device, uuid, "mm2s:{mm2s_fp_b}")
    s2mm_fp = pyxrt.kernel(device, uuid, "s2mm:{s2mm_fp}")

    print("Generate random values")
    c0 = np.random.randint(2**4, 2**8)
    c1 = np.random.randint(2**6, 2**9)
    mata = np.float32(np.random.rand(shape_a[0], shape_a[1]) * c0)
    matb = np.float32(np.random.rand(shape_b[0], shape_b[1]) * c1)
    matc = np.zeros(shape_c, dtype=np.float32)

    print("Allocate and initialize buffers")
    buf_a = pyxrt.bo(device,
                     size_a := mata.size * np.dtype(np.float32).itemsize,
                     pyxrt.bo.normal, mm2s_fp_a.group_id(0))
    buf_b = pyxrt.bo(device,
                     size_b := matb.size * np.dtype(np.float32).itemsize,
                     pyxrt.bo.normal, mm2s_fp_b.group_id(0))
    buf_c = pyxrt.bo(device,
                     size_c := matc.size * np.dtype(np.float32).itemsize,
                     pyxrt.bo.normal, s2mm_fp.group_id(0))

    buf_a.write(mata, 0)
    buf_b.write(matb, 0)

    print("Move vectors from host memory to VCK5000 memory")

    buf_a.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, size_a, 0)
    buf_b.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE, size_b, 0)

    print("Run Matrix Multiplication on the VCK5000")
    run_mm2s_fp_a = mm2s_fp_a(buf_a, None, size_a)
    run_mm2s_fp_b = mm2s_fp_b(buf_b, None, size_b)
    run_s2mm_fp = s2mm_fp(buf_c, None, size_c)

    run_mm2s_fp_a.wait()
    run_mm2s_fp_b.wait()
    run_s2mm_fp.wait()

    print("Move vectors from VCK5000 memory to host memory")
    buf_c.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE, size_c, 0)
    matc = np.reshape(np.frombuffer(buf_c.read(size_c, 0),
                                    dtype=np.float32), shape_c)

    golden = np.matmul(mata, matb)

    print(f"Array allclose {np.allclose(golden, matc, 0.1, 0.1)}")

    print(f"Array equal {np.array_equal(golden, matc)}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Run Matrix Multiplication on VCK5000")
    parser.add_argument("--xclbin", help="XCLBIN files", required=True)
    parser.add_argument("--device", type=int, default=0,
                        help="Device Index", required=False)
    args = parser.parse_args()

    main(args.xclbin, args.device)
