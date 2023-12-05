---
layout: default
tags: [Mermaid]
mermaid: true
---
# Low-order FIR Filter Project-Based Learning -- Single Kernel Programming

The Versal™ AI Engines (AIE) are well suited for vector math operations. One specific application that is worthy of consideration is low-order FIR filters.
For low-order symmetric and asymmetric FIR structures, a single AIE can implement multiple filters, each running at the AIE clock rate which can be 1 GHz or more.

This lab provides the entire process of designing a low-pass filter using Python in a Jupyter Notebook environment and deploying it to an AIE, showcasing the filtered results. This lab provides guidelines for coding such a filter in the Vitis™ AI Engine tools: for the experienced programmer it provides a structured approach to code low-order FIR filters. For learners it also provides a background of key concepts such as AIE vectors, AIE APIs, and data flow. The following table shows the specific design steps in each part.

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
    <td rowspan="4">1</td>
    <td rowspan="4"><a href="https://github.com/Xilinx/xup_aie_training/blob/main/pbl/aie_single_kernel/fir_lowpass/notebook/fir_lowpass1.ipynb">Software Implementation</a></td>
    <td>Generate the Complex Signal Data with High-frequency Noise</td>
    <td rowspan="4">Jupyter Notebook</td>
  </tr>
  <tr>
    <td>Designing a Lowpass FIR Filter and Quantization for Hardware Implementation</td>
  </tr>
  <tr>
    <td>Plot the Magnitude Response of the Filter</td>
  </tr>
  <tr>
    <td>Apply the Filter to Signal Using lfilter and Plot the Original and Software Filtered Signals</td>
  </tr>
  <tr>
    <td rowspan="7">2</td>
    <td rowspan="7"><a href="https://github.com/Xilinx/xup_aie_training/blob/main/pbl/aie_single_kernel/fir_lowpass/notebook/fir_lowpass2.ipynb">Single Kernel Programming</a></td>
    <td>Determine the Design Specifications</td>
    <td rowspan="7">AMD Vitis 2022.2</td>
  </tr>
  <tr>
    <td>Find the Maximum Performance for the Given Data Type</td>
  </tr>
  <tr>
    <td>Select the Number of Lanes</td>
  </tr>
  <tr>
    <td>Choosing the Proper AIE API for this FIR Application</td>
  </tr>
  <tr>
    <td>Set the Sliding Vector Buffers and Parameterize the Sliding Multiplication API</td>
  </tr>
    <tr>
    <td>Set the Vector Read and Write API While Tracking the Current Position of Pointer</td>
  </tr>
    <tr>
    <td>Putting it All Together -- Kernel Code Analysis</td>
  </tr>
  <tr>
    <td rowspan="4">3</td>
    <td rowspan="4"><a href="https://github.com/Xilinx/xup_aie_training/blob/main/pbl/aie_single_kernel/fir_lowpass/notebook/fir_lowpass3.ipynb">Graph Programming and Performance Analysis</a></td>
    <td>Create the kernel Graph and the test bench</td>
    <td rowspan="4">Jupyter Notebook</td>
  </tr>
  <tr>
    <td>Create the AIE C Test Bench</td>
  </tr>
    <tr>
    <td>Build the AIE Kernel and Run the AIE Emulation and Compare the AIE and SW Filter Results</td>
  </tr>
  <tr>
    <td>Analyze performance and accuracy</td>
  </tr>
</tbody>
</table>

The following design flowchart illustrates the logical relationships between key parameters in the eight design steps outlined in Part 2. For more details please refer to [Part 2](./notebook/fir_lowpass2.ipynb).

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

### Part 1: Software Implementation

Run the Software implementation in Jupyter lab on Windows or Linux system

1. Open the Jupyter Notebook: fir_lowpass1

   ```sh
   cd $HOME/pbl/aie_single_kernel/fir_lowpass/notebook
   py -m jupyter lab
```

2. Run the notebook：fir_lowpass1 in Jupyter Lab
3. Review the input and output reference data file in the following path

   ```terminal
   $HOME/pbl/aie_single_kernel/fir_lowpass/aie/data/
   ```

### Part 2: Single Kernel Programming

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

### Part 3: Graph Programming and Performance Analyze

1. Open the Jupyter Lab environment

   ```sh
   cd $HOME/pbl/aie_single_kernel/fir_lowpass/notebook
   py -m jupyter lab
   ```
2. Copy the AIE emulation output file back and compare the result by running the Python cells

---

<p align="center">Copyright© 2023 Advanced Micro Devices</p>
