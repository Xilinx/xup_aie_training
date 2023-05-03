// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#ifndef __SK_TYPES_H_
#define __SK_TYPES_H_

#include "xclhal2_mpsoc.h"
#include <memory>

/*
 * Helper functions for kernel to use.
 * getHostBO : create a BO handle from given physical address.
 * mapBO     : map BO handle to process's memory space.
 * freeBO    : free BO handle.
 * logMsg    : send log messages to XRT driver for saving as per ini settings
 */
struct sk_operations {
    unsigned int (*getHostBO)(unsigned long paddr, size_t size);
    void* (*mapBO)(unsigned int boHandle, bool write);
    void (*freeBO)(unsigned int boHandle);
    int (*getBufferFd)(unsigned int boHandle);
    int (*logMsg)(xrtLogMsgLevel level, const char* tag, const char* format, ...);
};

/*
 * Each soft kernel fucntion has two arguments.
 * args: provide reg file (data input, output, size etc.,
 *       for soft kernel to run.
 * ops:  provide help functions for soft kernel to use.
 */
typedef int (*kernel_t)(void* args, struct sk_operations* ops);

/*
 * PS Context Data Structure included by user PS kernel code
 */
class pscontext {
   public:
    ~pscontext();
    pscontext() : pimpl{std::make_shared<pscontext::impl>()} {}

   protected:
    struct impl;
    std::shared_ptr<impl> pimpl;
};

struct pscontext::impl {
   private:
    bool aie_profile_en;
};

typedef pscontext* (*kernel_init_t)(xclDeviceHandle device, unsigned char* uuid);
typedef int (*kernel_fini_t)(pscontext* xrtHandles);

#endif