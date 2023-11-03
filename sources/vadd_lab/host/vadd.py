# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT

import argparse
import numpy as np
import pyxrt


def main(xclbin: str = None, idx: int = 0, size: int = 1024):
    """Run vector addition on the VCK5000"""
    device = pyxrt.device(idx)
    xbin = pyxrt.xclbin(xclbin)
    uuid = device.load_xclbin(xbin)

    mm2s_1 = pyxrt.kernel(device, uuid, "pl_mm2s:{pl_mm2s_1}")
    mm2s_2 = pyxrt.kernel(device, uuid, "pl_mm2s:{pl_mm2s_2}")
    s2mm_1 = pyxrt.kernel(device, uuid, "pl_s2mm:{pl_s2mm_1}")

    buf_size = np.dtype(np.int32).itemsize * size

    print("Allocate and initialize buffers")
    in_buf_1 = pyxrt.bo(device, buf_size, pyxrt.bo.normal, mm2s_1.group_id(0))
    in_buf_2 = pyxrt.bo(device, buf_size, pyxrt.bo.normal, mm2s_2.group_id(0))
    out_buf_1 = pyxrt.bo(device, buf_size, pyxrt.bo.normal,
                         s2mm_1.group_id(0))

    print("Generate random values")
    in0 = np.random.randint(-(2**31), (2**31)-1, size=size, dtype=np.int32)
    in1 = np.random.randint(-(2**31), (2**31)-1, size=size, dtype=np.int32)
    out = np.zeros(size, dtype=np.int32)
    in_buf_1.write(in0, 0)
    in_buf_2.write(in1, 0)
    print("Move vectors from host memory to VCK5000 memory")
    in_buf_1.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE,
                  buf_size, 0)
    in_buf_2.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_TO_DEVICE,
                  buf_size, 0)

    print("Run vadd on the VCK5000")
    run_mm2s_1 = mm2s_1(in_buf_1, None, size)
    run_mm2s_2 = mm2s_2(in_buf_2, None, size)
    run_s2mm_1 = s2mm_1(out_buf_1, None, size)

    run_mm2s_1.wait()
    run_mm2s_2.wait()
    run_s2mm_1.wait()

    print("Move vectors from VCK5000 memory to host memory")
    out_buf_1.sync(pyxrt.xclBOSyncDirection.XCL_BO_SYNC_BO_FROM_DEVICE,
                   buf_size, 0)
    out = np.frombuffer(out_buf_1.read(buf_size, 0), dtype=np.int32)
    res = in0 + in1
    print(f'Vectors size: {size}\nResults equal {np.array_equal(res, out)}')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Run vector addition on VCK5000")
    parser.add_argument("--xclbin", help="XCLBIN files", required=True)
    parser.add_argument("--size", type=int, default=2048,
                        help="Number of elements", required=False)
    parser.add_argument("--device", type=int, default=0,
                        help="Device Index", required=False)
    args = parser.parse_args()

    main(args.xclbin, args.device, args.size)
