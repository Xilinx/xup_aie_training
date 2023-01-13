<!-- 
DO NOT add layout, it will prevent README.md to be render as index.html by GitHub pages
-->

# Xilinx University Program AI Engine Tutorial

## Introduction

Welcome to the XUP Vitis-based AI Engine tutorial. These labs will provide hands-on experience using the [Vitis unified software platform](https://www.xilinx.com/products/design-tools/vitis.html) with Xilinx FPGA hardware. You will learn how to develop applications using the Vitis development environment.

The tutorial instructions target the following hardware and software:

* Vitis 2022.2
* XRT 2.14.354
* AWS EC2 Instances, no VCK5000 hardware at the moment.

This tutorial shows you how to use Vitis with AWS EC2 F1. Source code is provided. You may be able to use the Vitis tutorial instructions with other cloud providers or your local hardware.

## Run Tutorial

You can run this tutorial in different ways.

1. If you have an VCK5000 board, you can run all parts of the tutorial on a local machine.

1. You can use the Vitis software in the cloud, with hardware in the cloud (VMAccel).

1. You can use the Vitis software on a local machine for building designs, and only switch to the cloud to deploy in hardware, make sure you build for the correct shell.

Once you have decided how you want to run the tutorial, follow the appropriate instructions below.

### XUP AWS Tutorial

If you are attending an instructor-led XUP tutorial, preconfigured instances will be provided for you. Use the following instructions to [connect to your assigned AWS XUP tutorial instance](setup_workshop.md).

### Local computer

To use your own computer, [install and set up Vitis and install the VCK5000 packages](setup_local_computer.md).

### Clone repository

You also need to clone this repository to get a copy of the source code, the lab steps consider that this repository is cloned directly in the home directory (`$HOME`).

```sh
cd $HOME
git clone https://github.com/Xilinx/xup_aie_training.git
```

## Tutorial overview

The complete set of labs includes the following modules; it is recommended to complete each lab before proceeding to the next

1. [Vector Addition using Streams](vadd_lab.md)

1. [Matrix Multiply, multiple kernels and data type support](matmult_lab.md)

1. [DSP Library Lab](dsplib_lab.md)

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>
