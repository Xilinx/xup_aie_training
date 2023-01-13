---
layout: default
---

# Setup Vitis and the VCK5000 on your own computer

Coming soon

<!--

Only follow these steps, if you want to reproduce this workshop on your own machine. Do not follow these steps when you are in a live University Program workshop.

## Install Vitis

[Download and install Vitis](https://www.xilinx.com/support/download.html)

## Install XRT

[Download XRT for the Alveo platforms](https://www.xilinx.com/products/design-tools/vitis/xrt.html#gettingstarted)

## Install the VCK5000 platform

Follow the [Get Started with AI Engine Development](https://www.xilinx.com/products/boards-and-kits/vck5000.html) steps to get access to the Development & Deployment files. 

Once you download the files, install them with your package manager.

### Patch the xilinx_vck5000_gen4x8_xdma_2_202210_1 platform

To be able to use the AI Engine on the VCK5000 from the Vitis IDE, please apply this change

```sh
sudo ./sources/patch_vck5000_gen4x8_xdma/aie_domain_patch.sh
```

## Setup the Tools

Add the following to your environment setup.

```sh
source /opt/xilinx/xrt/setup.(c)sh
source $XILINX_VITIS/settings64.(c)sh
export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/
```
-->
---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>
