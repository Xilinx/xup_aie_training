# Low-order FIR Filter Project Based Learning -- Single Kernel Programming

The Versal™ AI Engines (AIE) are well suited for vector math operations. One specific application that is worthy of consideration is low-order FIR filters. 
For low-order symmetric and asymmetric FIR structures a single AIE can implement multiple filters, each running at the AIE clock rate which can be 1 GHz or more.

This lab provides the entire process of designing a low-pass filter using Python in a Jupyter notebook environment and deploying it to an AIE, showcasing the filtered results. This lab provides guidelines for coding such a filter in the Vitis™ AI Engine tools: for the experienced programmer it provides a structured approach to code low-order FIR filters. For learners it also provides background of key concepts such as AIE vectors, AIE APIs, and data flow. 

The tutorial is divided into three parts. The first part shows the whole process of designing a low-pass filter and verifying it using Python.
The second part focus on the Single Kernel Programming using the AIE APIs to achieve the highest performance on the AI Engine. The primary goal of single kernel programming is to ensure the use of the vector processor approaches its theoretical maximum. Vectorization of the algorithm is important, but managing the vector registers, memory access, and software pipelining are also required. The third part goes through the steps on creating the ADF graph and analyze the performance. The following table shows the key content of the three parts of the tutorial.

|PART  | TOPIC   |  SPECIFICATION   |
|--------|:---------:|:----------|
| 1 | [Software implementation](./fir_lowpass1.ipynb) | Demonstrate the software implementation of the application |
|   |                         |Using python language and the powerful extensible library |
|   |                          |Generate the input data and golden file for the AIE |
| 2 | [Single Kernel Programming](./fir_lowpass2.ipynb) | Design the FIR single kernel in AIE |
|   |                           |Analyze and optimize the read and write efficiency of ports |
| 3 | [Graph Programming and Performance Analysis](./fir_lowpass3.ipynb) | Create the kernel Graph and the test bench |
|   |                           |Compare with AIE HW Emulation result with the SW result  |
|   |   |Analyze performance and accuracy |

<summary>Single Kernel Programming Design Flow In Part 2</summary>

```mermaid

graph TD

    A[Design Specification] --> B(Sample Rate)
    A[Design Specification] --> C(Symmetry)
    A[Design Specification] --> D(Data Type)
    A[Design Specification] --> E(Coefficient)
    B(Sample Rate) --> F[Interface Type]
    C(Symmetry) --> F[Interface Type]
    D(Data Type) --> G[Max MACs per Cycle]
    E(Coefficient) --> G[Max MACs per Cycle]
    G[Max MACs per Cycle]  --> H[Lanes and Samples]
    H[Lanes and Samples]  --> I[Vector Data Type and Pointer Position] 
    I[Vector Data Type and Pointer Position] --> K[API calls for I/O]
    F[Interface Type] --> K[API calls for I/O]
    I[Vector Data Type and Pointer Position] --> L[API calls for Computation]
    K(API calls for I/O) --> M(window_readincr_v<8> aie::load_v)
    L(API calls for Computation) --> N(Special Multiplications aie::sliding_mul_sym_xy_ops)

```

- Note: A basic understanding of FIR filters, C language, and the Xilinx® Vitis™ tools is assumed.

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>