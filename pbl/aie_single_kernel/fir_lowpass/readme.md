# Low-order FIR Filter Project Based Learning -- Single Kernel Programming

The Versal™ AI Engines (AIE) are well suited for vector math operations. One specific application that is worthy of consideration is low-order FIR filters.
For low-order symmetric and asymmetric FIR structures a single AIE can implement multiple filters, each running at the AIE clock rate which can be 1 GHz or more.

This lab provides the entire process of designing a low-pass filter using Python in a Jupyter notebook environment and deploying it to an AIE, showcasing the filtered results. This lab provides guidelines for coding such a filter in the Vitis™ AI Engine tools: for the experienced programmer it provides a structured approach to code low-order FIR filters. For learners it also provides background of key concepts such as AIE vectors, AIE APIs, and data flow. The following table shows the specific design steps in each part.

| Part | Topic                                                                   | Steps                                                                                        |
| ---- | ----------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| 1    | [Software Implementation](./notebook/fir_lowpass1.ipynb)                   | Generate the Complex Signal Data with High-frequency Noise                                   |
|      |                                                                         | Designing a Lowpass FIR Filter and Quantization for Hardware Implementation                  |
|      |                                                                         | Plot the Magnitude Response of the Filter                                                    |
|      |                                                                         | Apply the Filter to Signal Using lfilter and Plot the Original and Software Filtered Signals |
| 2    | [Single Kernel Programming](./notebook/fir_lowpass2.ipynb)                 | Determine the Design Specifications                                                          |
|      |                                                                         | Find the Maximum Performance for the Given Data Type                                         |
|      |                                                                         | Select the Number of Lanes                                                                   |
|      |                                                                         | Choosing the Proper AIE API for this FIR Application                                         |
|      |                                                                         | Set the Sliding Vector Buffers and Parameterize the Sliding Multiplication API               |
|      |                                                                         | Choose the Proper Interface Type for Data Input and Output                                   |
|      |                                                                         | Set the Vector Read and Write API While Tracking the Current Position of Pointer             |
|      |                                                                         | Putting it All Together -- Kernel Code Analysis                                              |
| 3    | [Graph Programming and Performance Analyze](./notebook/fir_lowpass3.ipynb) | Create the Single Kernel ADF Graph                                                           |
|      |                                                                         | Create the AIE C Test Bench                                                                  |
|      |                                                                         | Build the AIE Kernel and Run the AIE Emulation and Compare the AIE and SW Filter Results     |
|      |                                                                         | Performance Analysis                                                                         |

The following design flowchart illustrates the logical relationships between key parameters in the eight design steps outlined in Part 2. For more details please refer to [Part 2](./notebook/fir_lowpass2.ipynb).

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

## Steps

### Part 1: Software Implementation

Run the Software implementation in Jupyter lab on Windows or Linux system

1. Open the Jupyter lab environment
   ```
   cd $HOME\pbl\aie_single_kernel\fir_lowpass\notebook> py -m jupyter lab
   ```
2. Run the notebook：fir_lowpass1 in Jupyter Lab
3. Review and download the input and output reference data file in the following path
   ```
   $HOME\pbl\aie_single_kernel\fir_lowpass\aie\data\
   ```

### Part 2: Single Kernel Programming

The project will use Makefile files to automate the building process in Vitis.

1. Build the AIE Component and Run the AIE Emulation

Navigate to the AIE folder and run make all to build the HLS kernel project

```
   cd $HOME/fir_stream_memory/prj/aie
   make all
```

2. Run the AIE Emulation

```
   make aieemu
```

3. Analyze the AIE Emulation and compilation results

```
   make analyzer
```

4. Check the AIE Emulation result

```
   make get_output
```

The output files can be found here:

```
   $HOME/fir_stream_memory/prj/aie/data/output_aie.txt
```

### Part 3: Graph Programming and Performance Analyze

1. Open the Jupyter Lab environment

```
cd $HOME\pbl\aie_single_kernel\fir_lowpass\notebook> py -m jupyter lab
```

2. Copy the AIE emulation output file back and compare the result by running the Python cells

---

<p align="center">Copyright© 2023 Advanced Micro Devices</p>