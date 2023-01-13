---
layout: default
---

# Setup Tools and Source code


## Get Source Code

```sh
cd $HOME
git clone --recursive https://github.com/Xilinx/xup_aie_training.git
```

## Setup VCK5000 platform

Once the VCK5000 platform is installed, make it available by running:

```sh
echo "export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/" >> $HOME/.bashrc
source $HOME/.bashrc
```

## Verifying Tools Installation

If you are in a live instructor-led XUP tutorial, the tools have been already setup for you. You can also verify the installation of the tools.

### Verify Vitis & AIE compiler Installation

Verify Vitis by running the code below, this should return the Vitis version

```sh
vitis -version
```

Verify the AIE compiler by running the code below, this should return the aiecompiler version

```sh
aiecompiler --version
```

### Verify XRT installation

Verify XRT by running the code below, this should return the XRT version

```sh
xbutil --version
```

### Verify the VCK5000 platform

Verify that the VCK5000 is available for use by running the following code, this should return a list of files and one of them should have the `xpfm` extension

```sh
ls $PLATFORM_REPO_PATHS
```

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>
