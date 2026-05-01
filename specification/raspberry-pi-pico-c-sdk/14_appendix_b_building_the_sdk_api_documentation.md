Raspberry Pi Pico-series C/C++ SDK

## **Appendix B: Building the SDK API documentation**

The SDK documentation can be viewed online, but is also part of the SDK itself and can be built directly from the command line. If you haven’t already checked out the SDK repository you should do so,

$ cd ~/ $ mkdir pico $ cd pico $ git clone https://github.com/raspberrypi/pico-sdk.git --branch master $ cd pico-sdk $ git submodule update --init $ cd .. $ git clone https://github.com/raspberrypi/pico-examples.git --branch master

Install doxygen if you don’t already have it,

$ sudo apt install doxygen

Then afterwards you can go ahead and build the documentation for all platforms:

$ cd pico-sdk $ mkdir build $ cd build $ cmake -DPICO_EXAMPLES_PATH=../../pico-examples -DPICO_NO_PICOTOOL=1 -DPICO_PLATFORM=combined -docs .. $ make docs

The API documentation will be built and can be found in the pico-sdk/build/docs/doxygen/html directory, see Figure 30.

##  **TIP**

If you prefer to build documentation for a single platform only, then replace -DPICO_PLATFORM=combined-docs with -DPICO_PLATFORM=rp2040 or -DPICO_PLATFORM=rp2350 in the above, using a fresh build directory.

Appendix B: Building the SDK API documentation

**682**

Raspberry Pi Pico-series C/C++ SDK

_Figure 30. The SDK API documentation_

**==> picture [319 x 212] intentionally omitted <==**

Appendix B: Building the SDK API documentation

**683**
