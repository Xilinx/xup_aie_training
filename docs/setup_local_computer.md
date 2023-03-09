---
layout: default
---

# Setup Vitis and the VCK5000 on your own computer

Only follow these steps, if you want to reproduce this workshop on your own machine. Do not follow these steps when you are in a live University Program workshop.

The steps described below are only a high level overview, for more information refer to [Alveo Versal Vitis Platforms](xilinx.github.io/Alveo-Versal-Platforms/).

## Install Vitis

[Download and install Vitis](https://www.xilinx.com/support/download.html)

## Install XRT

[Download XRT for the Alveo platforms](https://www.xilinx.com/products/design-tools/vitis/xrt.html#gettingstarted)

We assume an Ubuntu distribution, change the variable `UBUNTU` to match your distribution.

```sh
UBUNTU="22.04"
XRT="202220.2.14.354"
wget "https://www.xilinx.com/bin/public/openDownload?filename=xrt_${XRT}_${UBUNTU}-amd64-xrt.deb" -O /tmp/xrt.deb
sudo apt install /tmp/xrt.deb
```

## Install the VCK5000 platform

Follow the [Get Started with AI Engine Development](https://www.xilinx.com/products/boards-and-kits/vck5000.html) steps to get access to the Development & Deployment files.

You need to install:

- Development platform, this allows you to build your own designs. Follow steps [here](https://xilinx.github.io/Alveo-Versal-Platforms/alveoversalplatforms/build/html/docs/getting-started/vck5000_migration.html)

- Deployment platform, this allows you to validate the proper functioning of your VCK5000 and deploy your own designs

- XRT-APU, this allows you to run kernels on the PS

Once you download the files, install them with your package manager. We recommend you to install the `gen4x8-qdma_2022.2` platform

## Setup the Tools

Add the following to your environment setup.

```sh
source /opt/xilinx/xrt/setup.(c)sh
source $XILINX_VITIS/settings64.(c)sh
export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/
```

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>
