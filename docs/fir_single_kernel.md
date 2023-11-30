---
layout: default
tags: [Mermaid]
mermaid: true
---


# Low-order FIR Filter Project Based Learning -- Single Kernel Programming

The Versal™ AI Engines (AIE) are well suited for vector math operations. One specific application that is worthy of consideration is low-order FIR filters.
For low-order symmetric and asymmetric FIR structures a single AIE can implement multiple filters, each running at the AIE clock rate which can be 1 GHz or more.

This lab provides the entire process of designing a low-pass filter using Python in a Jupyter notebook environment and deploying it to an AIE, showcasing the filtered results. This lab provides guidelines for coding such a filter in the Vitis™ AI Engine tools: for the experienced programmer it provides a structured approach to code low-order FIR filters. For learners it also provides background of key concepts such as AIE vectors, AIE APIs, and data flow.

The lab is divided into three parts. The first part shows the whole process of designing a low-pass filter and verifying it using Python. The second part focus on the Single Kernel Programming using the AIE APIs to achieve the highest performance on the AI Engine. The primary goal of single kernel programming is to ensure the use of the vector processor approaches its theoretical maximum. Vectorization of the algorithm is important, but managing the vector registers, memory access, and software pipelining are also required. The third part goes through the steps on creating the ADF graph and analyze the performance. The following table shows the key content of the three parts of the tutorial.

<table border="2">
<thead>
  <tr>
    <th>Part</th>
    <th>Topic</th>
    <th>Description</th>
    <th>Environment</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td rowspan="3">1</td>
    <td rowspan="3"><a href="https://github.com/Xilinx/xup_aie_training/blob/main/pbl/aie_single_kernel/fir_lowpass/notebook/fir_lowpass1.ipynb">Software implementation</a></td>
    <td>Demonstrate the software implementation of the application</td>
    <td rowspan="3">Jupyter Notebook</td>
  </tr>
  <tr>
    <td>Using Python and its powerful extensible library</td>
  </tr>
  <tr>
    <td>Generate the input and golden data files for the AIE</td>
  </tr>
  <tr>
    <td rowspan="2">2</td>
    <td rowspan="2"><a href="https://github.com/Xilinx/xup_aie_training/blob/main/pbl/aie_single_kernel/fir_lowpass/notebook/fir_lowpass2.ipynb">Single Kernel Programming</a></td>
    <td>Design the FIR single kernel in AIE</td>
    <td rowspan="2">AMD Vitis 2022.2</td>
  </tr>
  <tr>
    <td>Analyze and optimize the read and write efficiency of ports</td>
  </tr>
  <tr>
    <td rowspan="3">3</td>
    <td rowspan="3"><a href="https://github.com/Xilinx/xup_aie_training/blob/main/pbl/aie_single_kernel/fir_lowpass/notebook/fir_lowpass3.ipynb">Graph Programming and Performance Analysis</a></td>
    <td>Create the kernel Graph and the test bench</td>
    <td rowspan="3">Jupyter Notebook</td>
  </tr>
  <tr>
    <td>Compare with AIE HW Emulation result with the SW result</td>
  </tr>
  <tr>
    <td>Analyze performance and accuracy</td>
  </tr>
</tbody>
</table>

<summary>Single Kernel Programming Design Flow In Part 2</summary>

<div class="mermaid">
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
</div>

- Note: A basic understanding of FIR filters, C language, and the Xilinx® Vitis™ tools is assumed.

## Steps

### Part 1

Run the Software implementation in Jupyter lab on Windows or Linux system

1. Open the Jupyter Notebook: fir_lowpass1

   ```sh
   cd $HOME/pbl/aie_single_kernel/fir_lowpass/notebook
   py -m jupyter lab
   ```

2. Run the notebooks in Jupyter lab

3. Review the input and output reference data file in the following path

   ```terminal
   $HOME/pbl/aie_single_kernel/fir_lowpass/aie/data/
   ```

### Part 2

The project will use Makefile files to automate the building process in Vitis.

1. Build the AIE Component and Run the AIE Emulation

   Navigate to the AIE folder and run make all to build the HLS kernel project

   ```sh
   cd $HOME/pbl/fir_stream_memory/prj/aie
   make all
   ```

2. Run the AIE Emulation

   ```sh
   make aieemu
   ```

3. Analyze the AIE Emulation and compilation results

   ```sh
   make analyzer
   ```

4. Check the AIE Emulation result

   ```sh
   make get_output
   ```

   The output files can be found here: `$HOME/pbl/fir_stream_memory/prj/aie/data/output_aie.txt`

5. Build the Whole Application for Hardware Run (optional)

    Build for hardware targets and generate the FPGA binary (.xclbin file) and host executable, which includes build the PL kernels and integrate the PL kernels and AIE kernels together in the VCK5000 platform.

    - Note:This step can take a couple of hours, or more significant time depending on the design complexity, the machine you are building on, and its current workload.

   ```sh
   cd $HOME/pbl/fir_stream_memory/prj
   make all
   ```

6. Run the application on a system with the AMD VCK5000 card using the following command (optional)

   ```sh
   cd $HOME/pbl/fir_stream_memory/prj/host
   ./fir_s_m.exe ../build.hw/fir_s_m.xclbin
   ```

1. Check the hardware output file (optional)

   ```sh
   $HOME/pbl/fir_stream_memory/prj/host/output.txt
   ```

### Part 3

1. Open the Jupyter Notebook: fir_lowpass3

   ```sh
   cd $HOME/pbl/aie_single_kernel/fir_lowpass/notebook
   py -m jupyter lab
   ```

2. Copy the AIE output file back and compare the result by run the Python cells

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>
