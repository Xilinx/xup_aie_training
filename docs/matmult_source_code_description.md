---
layout: default
---

# Matrix Multiplication Source Code Description

This document describes the kernel code to compute the matrix multiplication of A(8,8) x B(8,8) for four different data types. Depending on the data type, we tweak the algorithm to suit best the architecture of the AIE Tile, this slightly variation is mostly in the form of computing partial results to leverage the accumulator and data path of the vector processor architecture.

Before looking at each kernel in detail, it is important to highlight that we are going to use advanced MAC intrinsic functions to achieve our goal. This is well described in the [Full Lane Addressing Scheme documentation](https://www.xilinx.com/htmldocs/xilinx2022_2/aiengine_intrinsics/intrinsics/group__vect__mult__intrinsics__explained.html). We map matrix A and B to the X buffer (up to 1024-bit wide) and the Z buffer (256-bit wide), Y buffer is not used.

The operations in advanced MAC can be described using lanes and columns. The number of columns is the number of multiplications that are done per output lane, with each of these multiplications added together. The product of lanes and columns gives the MACs/cycle. The maximum number of MACs/cycle for a given data type is described in this [table](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/Functional-Overview?section=gjd1525480035443__table_vfm_pr5_w2b).

The number of lanes and columns is a function of the data types we use. Note that not all combination of data types are supported for X and Z buffers. For this lab, we are going to use the same data type for both matrix A and matrix B.

## Matrix Multiplication

This section is only intended to provide context in order to understand the following sections. We all are familiar with matrix multiplication. The inner product of each row of the first matrix times each column of the second matrix produces one element of the result. This is shown in the following animation.

Animation coming soon

![](images/animations/MatrixMult.mp4)

Matrix multiplication has a high computation intensity due to the number of required operations to compute the output result. Accelerating matrix multiplication is well covered in the literature both for CPU and GPU.

## Matrix Multiplication on the AI Engine Tile

In the remainder of this document, we cover how the matrix multiplication is performed in the AI Engine tile for 4 different data types

* float
* int32
* int16
* int8

For simplicity, we consider the following operation A x B = C, where the size of matrix A and B (for the purpose of this description) are described below:

- A is either 8x8 or 4x8. The described algorithm for AIE can be extrapolated for A = Nx8, where N must be a multiple of 4
- B is 8x8. The described algorithm for AIE can be extrapolated for B = 8xN

Please note that for a given data type, there may be other ways of breaking down the computation on the AI Engine tile. We only describe one of them.

For each data type we are going to fill the following table:

| MACs/cycles | lanes | columns | MatA maps to | MatB maps to | Elements on X buffer | Elements on Z buffer | Accumulator | MAC/MUL intrinsic |
|-------------|-------|---------|--------------|--------------|----------------------|----------------------|-------------|-------------------|
|             |       |         |              |              |                      |                      |             |                   |

This table will help us drive the discussion about how the matrix multiplication is mapped to the AI Engine tile.

### Matrix Multiplication int32 and float

For `int32` and `float` data types the computation is done in the same way. However, for `float`, the computation is done in the [floating-point vector unit](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/Floating-Point-Vector-Unit), whereas for `int32` the computation is done in the [fixed-point vector unit](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/Fixed-Point-Vector-Unit).

Based on the [documentation](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/Functional-Overview?section=gjd1525480035443__table_vfm_pr5_w2b), we know that for `int32` and float (real data types only) the maximum number of MACs is 8. So, we need to find out a way of parallelizing the computation doing 8 multiply accumulate operations per cycle.


| MACs/cycles | lanes | columns | MatA maps to | MatB maps to | Elements on X buffer | Elements on Z buffer | Accumulator | MAC intrinsic |
|-------------|-------|---------|--------------|--------------|----------------------|----------------------|-------------|---------------|
|     8       |  8    |   1     | X buffer     | Z buffer     |         16           |          8           |     acc80   |  [Link](https://www.xilinx.com/htmldocs/xilinx2022_2/aiengine_intrinsics/intrinsics/group__vect__mult__32x32.html#ga63b63eacca61ba92cd8b314bc6a27c41) |


What the table tells us is that at any given time we can only access to 16 elements of matrix A (2 rows) and 8 elements of matrix B (1 row). So, it is not possible to perform a complete inner product, hence, we can generate partial results and store such partial result on the accumulator registers or vector register (in the case of the floating-point vector processor).

At a high level, in each iteration we multiply one element of X buffer times all of the elements in Z buffer, add to the previous computation and move to the next row of matrix B, when all of the rows of B are completed, we have the result of the inner product. At this point, we can move to the next two rows of matrix A. Note, that as we have 16 elements of A in X buffer we can do the same operation for the odd rows of matrix A without having to load from memory, thus minimizing data movement.

Once we have the final result of the inner product, we can write the result to memory. However, for `int32` we were accumulating in an 80-bit accumulator. Hence, we need to reduce the precision from 80-bit to 32-bit, this is done using the Shift-round Saturate path. After this, we can finally write the result to memory.

The final part is to find the appropriate parameters to configure the intrinsic and achieve the lane addressing we desired. You can explore different lane addressing for this mac running the Python code `sources/lane_addressing_scheme/scheme_32_32.py`

<video  controls="" height="560" loop="true" autoplay="true">
    <source src="images/animations/int32_float.mp4" type="video/mp4">
</video>

Note that the animation is an **educational vehicle only** and may contain inaccuracies that do not fully reflect the AIE architecture.

### Matrix Multiplication int16

Based on the [documentation](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/Functional-Overview?section=gjd1525480035443__table_vfm_pr5_w2b), we know that for `int16` (real data types only) the maximum number of MACs is 32. So, we need to find out a way of parallelizing the computation doing 32 multiply accumulate operations per cycle.

| MACs/cycles | lanes | columns | MatA maps to | MatB maps to | Elements on X buffer | Elements on Z buffer | Accumulator | MUL intrinsic |
|-------------|-------|---------|--------------|--------------|----------------------|----------------------|-------------|---------------|
|     32      |  16   |   2     | Z buffer     | A buffer     |         32           |          16          |     acc48   |  [Link](https://www.xilinx.com/htmldocs/xilinx2022_2/aiengine_intrinsics/intrinsics/group__vect__mult__16x16.html#ga1e00ad6eedd92916e22e27f83abe5f01) |

What the table tells us is that at any given time we can access to 32 elements of matrix A (4 rows) and 16 elements of matrix B (2 rows). So, it is not possible to perform a complete inner product, hence, we can generate partial results and store such partial result on the accumulator registers.

At a high level, in each iteration we can tile the inner product by using two-element sub row of A times two-element sub column of B, add to the previous computation and move to the next two rows of matrix B, when all of the rows of B are completed, we have the result of the inner product. At this point, we can move to the next four rows of matrix A. Note, that as we have 32 elements of A in X buffer we can do the same operation for the two consecutive rows of matrix A without having to load from memory, thus minimizing data movement. Also, not that as we are using 16-lane output, each mac intrinsic is performing the tiled inner product for row `i` and `i+1` of A.

Once we have the final result of the inner product, we can write the result to memory. However, we were accumulating in a 48-bit accumulator. Hence, we need to reduce the precision from 48-bit to 16-bit, this is done using the Shift-round Saturate path.

The final part is to find the appropriate parameters to configure the intrinsic and achieve the lane addressing we desired. You can explore different lane addressing for this mac running the Python code `sources/lane_addressing_scheme/scheme_16_16.py`

<video controls="" height="560" loop="true" autoplay="true">
    <source src="images/animations/int16.mp4" type="video/mp4">
</video>

Note that the animation is an **educational vehicle only** and may contain inaccuracies that do not fully reflect the AIE architecture.

### Matrix Multiplication int8

Based on the [documentation](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/Functional-Overview?section=gjd1525480035443__table_vfm_pr5_w2b), we know that for `int8` (real data types only) the maximum number of MACs is 128. So, we need to find out a way of parallelizing the computation doing 128 multiply accumulate operations per cycle.

| MACs/cycles | lanes | columns | MatA maps to | MatB maps to | Elements on X buffer | Elements on Z buffer | Accumulator | MUL intrinsic |
|-------------|-------|---------|--------------|--------------|----------------------|----------------------|-------------|---------------|
|     128     |  16   |   8     | Z buffer     | A buffer     |         64           |          32          |     acc48   |  [Link](https://www.xilinx.com/htmldocs/xilinx2022_2/aiengine_intrinsics/intrinsics/group__vect__mult__8x8.html#ga8676a949b7605d65c4dc6974c793d37b) |

What the table tells us is that at any given time we can access to 32 elements of matrix A (4 rows) and 64 elements of matrix B (8 rows). So, it is possible to perform a complete inner product, at least for a subset of matrix A.

At a high level, in each iteration we produce the inner product of two rows of matrix A and all columns of matrix B. Then, we can move to the next 4 rows of matrix A. Note, that as we have 32 elements of matrix A in Z buffer we can do the same operation for the rows `i+2` and `i+3` of matrix A without having to load from memory, thus minimizing data movement.

Once we have the final result of the inner product, we can write the result to memory. However, we stored the inner product in a 48-bit accumulator. Hence, we need to reduce the precision from 48-bit to 8-bit, this is done using the Shift-round Saturate path.

The final part is to find the appropriate parameters to configure the intrinsic and achieve the lane addressing we desired. You can explore different lane addressing for this mac running the Python code `sources/lane_addressing_scheme/scheme_8_8.py`

<video  controls="" height="560" loop="true" autoplay="true">
    <source src="images/animations/int8.mp4" type="video/mp4">
</video>

Note that the animation is an **educational vehicle only** and may contain inaccuracies that do not fully reflect the AIE architecture.

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>