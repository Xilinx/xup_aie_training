---
layout: default
---

# Steps to install OpenCV 4.4.0

## Clone repositories

<!-- based on https://linuxize.com/post/how-to-install-opencv-on-ubuntu-20-04/-->

```sh
export OPENCV_TAG=4.4.0
export DIR=/tmp/
mkdir -p $DIR/opencv_build && cd $DIR/opencv_build
git clone https://github.com/opencv/opencv.git -b $OPENCV_TAG
git clone https://github.com/opencv/opencv_contrib.git -b $OPENCV_TAG
```

## Create a temporary build directory

```sh
cd $DIR/opencv_build/opencv
mkdir -p build && cd build
```

## Set up the OpenCV build with CMake

```CMAKE
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_GENERATE_PKGCONFIG=ON \
    -D OPENCV_EXTRA_MODULES_PATH=$DIR/opencv_build/opencv_contrib/modules ..
```
Note: Make sure Vivado is not source as its CMake will take priority

## Start the compilation process

```sh
make -j32
```

## Install OpenCV

```
sudo make install
```

## Verify the installation

```sh
pkg-config --modversion opencv4
```

The output should be 4.4.0

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>