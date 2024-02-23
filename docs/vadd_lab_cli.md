---
layout: default
---

# Vector Add lab Using the Command Line

## Introduction

This lab guides you through the steps involved in creating your first Vitis project targeting both the AI Engine and the Adaptable Engine using the command line interface. You will be implementing a very simple adder kernel in the AI Engine and a few data movers on the Adaptable Engine. You will simulate and emulate the project. Finally, you will run the lab on real hardware.

### References

For more details on the Vitis CLI check out [Compile using v++](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Compile-using-v-Unified-Compiler).

* [Compiling AI Engine Graph Applications](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Compiling-AI-Engine-Graph-Applications)

## Objectives of this lab

After completing this lab, you will learn to:

* Compile an AI Engine code using the CLI

* Run AI Engine software simulation

* Run AI Engine software emulation

* Linking kernels between Adaptable and AI Engines

* Run hardware

## Steps

Your journey with Versal devices begins here, buckle up!

### Clone repo

Follow these steps to get the source code for this tutorial, [Get Source Code](setup_tools.md#get-source-code)

### Check the Environment

1. Verify that the tools and VCK5000 platform are setup correctly [here](setup_tools.md#verifying-tools-installation)

2. Additionally, run the following code in a command line to check that the tools are installed.

```sh
which vitis
which aiecompiler
```

### Compile the Vector Add code for `x86sim`

We are going to compile the AI Engine kernel and run software emulation to verify code correctness.

1. Go to the directory with the AIE code

   ```sh
   cd $HOME/xup_aie_training/sources/vadd_lab/aie
   ```

1. Analyze the source code using your code editor of choice

   ```sh
   code src/
   ```

1. Analyze the `Makefile`

   ```sh
   code Makefile
   ```

   The `Makefile` will drive the compilation of the AIE code.

1. Run the compilation, in the CLI run:

   ```sh
   make TARGET=x86sim
   ```

   Learn more about the `aiecompiler` options [here](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/AI-Engine-Compiler-Options)

1. Once the compilation finishes, run the `x86sim` simulation

   ```sh
   make sim TARGET=x86sim
   ```

   Software emulation (x86 Simulation) uses the files in the data folder as stimuli. We will get an output file with the results.

1. The Console will show the following message:

   ```console
   x86simulator --pkg-dir=./Work --i=.. 
   INFO: Reading options file './Work/options/x86sim.options'.
   AI Engine results match golden result
   Simulation completed successfully returning zero
   ```

   This runs a functional simulation, where you have a chance to check that the code is working correctly.

### Compile the Vector Add code for `hw` and run emulation

This is still a software emulation (AIE Simulation), however the simulation takes into account the actual AI Engine array architecture. The AIE Simulation also uses files as input/outputs.

1. Run the compilation, in the CLI run:

   ```sh
   make
   ```

   By default, the compilation target is `hw`

1. Once the compilation finishes, run the `aieemu` simulation

   ```sh
   make sim
   ```

   The emulation takes around 4-5 minutes.

1. Compare the results

   ```sh
   diff data/golden.txt <(grep -v T build.hw/aiesimulator_output/output.txt | sed "s/^[ \t]*//"  | sed "s/[ ^t]*$//")
   ```

   The command above compares the golden reference with the output of the emulation. The sed commands remove time information from the emulation file.

1. Visualize the graph

   ```sh
   vitis_analyzer build.hw/work/graph.aiecompile_summary
   ```

## Build Design for Hardware and Run

1. Navigate to the `vadd_lab` root folder.

   ```sh
   cd $HOME/xup_aie_training/sources/vadd_lab/
   ```

1. Build design for hardware

   ```sh
   make
   ```

   This may take around 80 minutes and it will generate the `vadd.xclbin` and `vadd.exe` files

1. Run in hardware

   ```sh
   cd $HOME/xup_aie_training/sources/vadd_lab/vadd
   ./vadd.exe ../build.hw/vadd.xclbin
   ```

1. Alternatively, you can run in hardware using the Python script

   ```sh
   cd $HOME/xup_aie_training/sources/vadd_lab/vadd
   python3 vadd.py --xclbin ../build.hw/vadd.xclbin
   ```

## Assignments for the Reader

The following assignments are optional, however they will help deepen your knowledge about the AIE programming model. No solution is provided for these assignments.

1. By default the graph instantiates the `vadd_stream` kernel. However, you can also instantiate the `vadd_window` kernel by commenting line 11. Comment line 11, recompile the graph and rerun the AIE. What differences do you notice in the graph?

1. Make the necessary changes to include both `vadd_stream` and `vadd_window` kernels in the `simpleGraph`

   Note that the text files used as stimuli cannot be reused

1. Change the plio bitwidth in the `input_plio::create()` to either `plio_64_bits` or `plio_128_bits`. What changes do you notice? Is it necessary to make any other changes?

   Note that the text files used as stimuli need to be regenerated to match the plio bitwidth, the `write_file` function on the Python file supports this

If you are attending an in-person tutorial, you can request support from your instructor. Otherwise, open a [GitHub issue](https://github.com/Xilinx/xup_aie_training/issues/new/choose)

## Conclusion

In this lab, you used Vitis command line to create a vector add running on the AI Engine. You ran both x86 and AIE simulation.

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>
