---
layout: default
---

# Vadd Lab Source Code

This document describes the vadd source files in detail. For further detail on how this kernel is constructed and tested please see the [UG1079](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Overview?tocId=OerrcATBJkz9SuXKjosb1w) which is a guide to AI Engine Kernel and Graph Programming Guide. AI Engine programs are designed as an Adaptive Data Flow Graph (ADF). This gives designer an intuitive way to work with the flow of data. More information on kernel programming and ADF can be found [here](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Introduction-to-Graph-Programming).

## AIE Kernels

### Streaming Kernel

```c++
#include <adf.h>
#include "aie_api/aie.hpp"

void aie_vadd_stream(input_stream_int32 *in0, input_stream_int32 *in1, output_stream_int32 *out){
    aie::vector<int32, 4> a = readincr_v4(in0);
    aie::vector<int32, 4> b = readincr_v4(in1);
    aie::vector<int32, 4> res = aie::add(a, b);
    writeincr_v4(out, res);
}
 ```

The `aie_vadd_stream` kernel operates on streams of data (sequential access). This kernel uses the AI Engine API which is recommended for all kernel designs. It consists of one function called **void aie_vadd_stream**. This function receives two pointer to instances of the *input_stream_int32* class **in0** and **in1** which represent ports of the vadd kernel where the source data for this computation will come from. The data in and out of this kernel is being moved by the use of streams. The function also has another argument which is a pointer to an instance of the  *output_stream_int32* class, **out**. This argument represent the output port of the Kernel. For more information on data streams for the AI Engine see [here](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Load-and-Store-Using-Streams)

Within the function first the vectors a and b are initialized by reading from the two source ports. The vectors a and b are defined in the context of the aie class which comes from the AI Engine API. They are also defined has having 4 lanes meaning that 4 samples can be added together in parallel. The function *readincr_v4()* returns an int32 vector with 4 samples. The final line of this function adds the two vectors a and b together performing 4 additions in parallel (element-wise) and writes it the the output port.  More information about the streaming interfaces of AI Engines can be found [here](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/AI-Engine-Interfaces)

More information can be found [here](https://www.xilinx.com/htmldocs/xilinx2022_2/aiengine_api/aie_api/doc/index.html). The full source code for this section can be found [here](https://github.com/Xilinx/xup_aie_training/blob/main/sources/vadd_lab/aie/src/vadd_stream.cc)


### Window Kernel

```c++
#include "aie_api/aie.hpp"
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

void aie_vadd_window(input_window<int32> *in0, input_window<int32> *in1, output_window<int32> *out){
    for (unsigned int i=0; i< 2048/8; i++) {
        auto a = window_readincr_v<8>(in0);
        auto b = window_readincr_v<8>(in1);
        auto res = aie::add(a, b);
        window_writeincr(out, res);
    }
}
 ```

The `aie_vadd_window` kernel operates on window of data (buffers). This kernel uses the AI Engine API which is recommended for all kernel designs. It consists of one function called **void aie_vadd_window**. This function receives two pointer to instances of the *input_window<int32>* class **in0** and **in1** which represent ports of the vadd kernel where the source data for this computation will come from. The data in and out of this kernel is being moved from local memory. The function also has another argument which is a pointer to an instance of the  *output_window<int32>* class, **out**. This argument represent the output port of the Kernel. For more information on data streams for the AI Engine see [here](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Load-and-Store-From-Memory)


Within the function there is a loop to iterate over the data. First the vectors a and b are initialized by reading from memory 8 elements (256-bit wide).  The function *window_readincr_v<8>()* returns a vector of 8 lanes with the same datatype as the argument, int32 in this case. We perform the addition of vector a and b with `aie::add(a, b)`. By using the `auto` keyword, we let the compiler to define the appropriate datatype for us. The `window_writeincr(out, res)` writes the result to **out** and increases the pointer by the number of lanes on `res`.  More information about the data memory interfaces of AI Engines can be found [here](https://docs.xilinx.com/r/en-US/am009-versal-ai-engine/AI-Engine-Interfaces)

More information can be found [here](https://www.xilinx.com/htmldocs/xilinx2022_2/aiengine_api/aie_api/doc/index.html). The full source code for this section can be found [here](https://github.com/Xilinx/xup_aie_training/blob/main/sources/vadd_lab/aie/src/vadd_window.cc)


## Adaptive Dataflow Graph

```c++
#include <adf.h>
#include "kernels.hpp"

#define STREAM

#ifdef STREAM
#define CONNECTION stream
#else
#define CONNECTION window<2048 * sizeof(int)>
#endif

using namespace adf;

class simpleGraph : public graph {
    private:
        kernel vadd;

    public:
        input_plio p_s0;
        input_plio p_s1;
        output_plio p_s2;

        simpleGraph() {
            // create kernel & define source code
#ifdef STREAM
            vadd = kernel::create(aie_vadd_stream);
            source(vadd) = "vadd_stream.cc";
#else
            vadd = kernel::create(aie_vadd_window);
            source(vadd) = "vadd_window.cc";
#endif
            // Define connection names and text file source/sink
            p_s0 = input_plio::create("StreamIn0", plio_32_bits, "data/input0.txt");
            p_s1 = input_plio::create("StreamIn1", plio_32_bits, "data/input1.txt");
            p_s2 = output_plio::create("StreamOut0", plio_32_bits, "output.txt");

            //connect ports and kernel
            connect<CONNECTION>(p_s0.out[0], vadd.in[0]);
            connect<CONNECTION>(p_s1.out[0], vadd.in[1]);
            connect<CONNECTION>(vadd.out[0], p_s2.in[0]);

            // Define kernel runtime ratio
            runtime<ratio>(vadd) = 1;
        };
};
```

This document file defines the ``simpleGraph`` class that is used to represent the design. This class inherits the methods of the ``graph`` class that comes from the ADF model.

The first thing that this class creates is the kernel for the vector add operation. The kernel class is explained [here](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/graph) in the user guide. The create function within the kernel class, `kernel::create()`, determines how many inputs and outputs the kernel has and determines its appropriate element type for these inputs and outputs. The `source()` function links the AIE kernel source code to this dataflow graph, the kernel's behavior. A source file constraint is required for each kernel being used.

Next the input and output ports to the kernel are specified. The **input_plio** and the **output_plio** classes represent I/O ports used to connect the AI Engine kernel to external platform ports. The create function in context of these classes take in the name of the port, the data width (which is chosen from predefined options from within these plio classes), and the source where the data is taken from for simulation.  Further information on PLIO can be found [here](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/input_plio/output_plio).

Next the connect function is called to connect the data streams to the source and sink ports of the vadd kernel. By default. the stream-based access model is used in this instance. This model on AI Engine supports two 32-bit input streams of data and two 32-bit output streams of data. In this instance we used two input streams and one output stream. For further information on the Stream-based Access and other models for data movement please refer to [this section](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Stream-Based-Access) of the user guide.

The final line introduce constraints to the kernel. The runtime ratio constrain allows you to specify a core usage fraction for a kernel. In this case one hundred percent of the AI Engine tile is permitted to be used for this kernel. More information on kernel constraints can be found [here](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Constraints). The full source code for this section can be found [here](https://github.com/Xilinx/xup_aie_training/blob/main/sources/vadd_lab/aie/src/graph.hpp).

Note that by commenting the line `#define STREAM`, the graph will use the window kernel. For a window kernel, we need to specify the size of the window to use (buffer size).

## Testbench

```c++
#include "graph.hpp"

simpleGraph vadd_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char** argv) {
    vadd_graph.init();
#ifdef STREAM
    vadd_graph.run(512);
#else
    vadd_graph.run(1);
#endif
    vadd_graph.end();
    return 0;
}
#endif
```    
This document defines how the kernel will be tested in simulation for the AIE portion of the design. It defines an instance of the simpleGraph called vadd_graph. The simpleGraph class was defined in **graph.hpp** mentioned above. The code itself first initialize the graph, which sets up the vadd kernel and its ports. Then it runs the kernel 512 times which processes 2,048 elements. Then finally it ends the graph. The init(), run(), and end() methods are all inherited from the graph class and not present in the simpleGraph declaration. For a window-based kernel, the testbench only run the graph once.

For further information please refer to the UG1079 section on [simulation](https://docs.xilinx.com/r/en-US/ug1079-ai-engine-kernel-coding/Kernel-Simulation). The full source code for this section can be found [here](https://github.com/Xilinx/xup_aie_training/blob/main/sources/vadd_lab/aie/src/graph.cpp).
 

## Test Vectors

The test vectors used for simulation are sourced from .txt files. The values in these files are generated using a python file found [here](https://github.com/Xilinx/xup_aie_training/blob/main/sources/vadd_lab/aie/data/gen_test_data.py). source.txt contains the input vectors and golden.txt contains the expected results which are used to verify the kernels functionality.

---------------------------------------
<p align="center">Copyright&copy; 2023 Advanced Micro Devices</p>
