Getting started with Raspberry Pi Pico-series

## **Chapter 3. Install the Raspberry Pi Pico VS Code Extension**

The Raspberry Pi Pico VS Code extension helps you create, develop, run, and debug projects in Visual Studio Code. It includes a project generator with many templating options, automatic toolchain management, one click project compilation, and offline documentation of the Pico SDK.

The VS Code extension supports all Raspberry Pi Pico-series devices.

## **3.1. Install Dependencies**

## **3.1.1. Raspberry Pi OS and Windows**

No dependencies needed.

## **3.1.2. Linux**

Most Linux distributions come preconfigured with all of the dependencies needed to run the extension. However, some distributions may require additional dependencies. The extension requires the following:

- [Python 3.9 or later]

- [Git]

- [Tar]

- [a native C and C++ compiler (the extension supports GCC)]

You can install these with:

$ sudo apt install python3 git tar build-essential

## **3.1.3. macOS**

To install all requirements for the extension on macOS, run the following command:

$ xcode-select --install

This installs the following dependencies:

- [Git]

- [Tar]

- [A native C and C++ compiler (the extension supports GCC and Clang)]

3.1. Install Dependencies

**6**

Getting started with Raspberry Pi Pico-series

## **3.2. Install the Extension**

You can find the extension in the VS Code Extensions Marketplace. Search for the **Raspberry Pi Pico** extension, published by **Raspberry Pi** . Click the **Install** button to add it to VS Code.

_Figure 1. Debugging in VS Code._

**==> picture [425 x 266] intentionally omitted <==**

You can find the store entry at https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico.

You can find the extension source code and release downloads at https://github.com/raspberrypi/pico-vscode.

When installation completes, check the Activity sidebar (by default, on the left side of VS Code). If installation was successful, a new sidebar section appears with a Raspberry Pi Pico icon, labelled "Raspberry Pi Pico Project".

3.2. Install the Extension

**7**
