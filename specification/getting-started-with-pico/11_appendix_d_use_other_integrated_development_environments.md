Getting started with Raspberry Pi Pico-series

## **Appendix D: Use other Integrated Development Environments**

The recommended Integrated Development Environment (IDE) is Visual Studio Code. However other environments can be used with Raspberry Pi microcontrollers and Raspberry Pi Pico-series.

## **Use Eclipse**

Eclipse is a multiplatform Integrated Development environment (IDE) available for Linux, macOS, and Windows. The latest version works well on the Raspberry Pi 4, 400, and 5 (4GB and up) running a 64-bit OS. The following instructions describe how to set up Eclipse on a Linux device for to develop on Pico-series devices. Instructions for other systems will be broadly similar, although the details of connecting to Pico-series devices vary.

## **Setting up Eclipse for Pico on a Linux machine**

Prerequisites:

- [Device running a recent version of Linux with at least 4GB of RAM]

- [64-bit operating system.]

- [CMake 3.11 or newer]

If using a Raspberry Pi, you should enable the standard UART by adding the following to config.txt

## enable_uart=1

You should also install OpenOCD and the SWD debug system. See [debug_probe_section] for instructions on how to do this.

## **Installing Eclipse and Eclipse plugins**

Install the latest version of Eclipse IDE for Embedded C/C++ Developers using the standard instructions. If you are running on an ARM platform, you will need to install an AArch64 (64-bit ARM) version of Eclipse. All versions can be found on the Eclipse website. https://www.eclipse.org/downloads/packages

Download the correct file for your system, and extract it. You can then run it by going to the place where it was extracted and running the 'eclipse' executable.

$ ./eclipse

The Embedded CDT version of Eclipse includes the C/C++ development kit and the Embedded development kit, so has everything you need to develop for Pico-series devices.

## **Using pico-examples**

The standard build system for the Pico environment is CMake. However Eclipse does not use CMake as it has its own build system, so we need to convert the pico-examples CMake build to an Eclipse project.

Use Eclipse

**41**

Getting started with Raspberry Pi Pico-series

1. At the same level as the pico-examples folder, create a new folder, for example pico-examples-eclipse

2. Change directory to that folder

3. Set the path to the PICO_SDK_PATH

$ export PICO_SDK_PATH=<wherever>

4. Run the following command:

$ cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../pico-examples

##  **IMPORTANT**

The SDK builds binaries for the Raspberry Pi Pico by default. To build a binary for a different board, pass the -DPICO_BOARD=<board> option to CMake, replacing the <board> placeholder with the name of the board you’d like to target. To build a binary for Pico 2, pass -DPICO_BOARD=pico2. To build a binary for Pico W, pass -DPICO_BOARD=pico_w. To specify a Wi-Fi network and password that your Pico W should connect to, pass -DWIFI_SSID="Your Network" -DWIFI_PASSWORD="Your Password".

This will create the Eclipse project files in our pico-examples-eclipse folder, using the source from the original CMake tree.

You can now load your new project files into Eclipse using the Open project From File System option in the File menu.

## **Building**

Right click on the project in the project explorer, and select Build. This will build all the examples.

## **OpenOCD**

This example uses the OpenOCD system to communicate with a Raspberry Pi microcontroller. You will need to have provided the 2-wire debug connections from the host device to the microcontroller prior to running the code. On a Raspberry Pi, this can be done via GPIO connections, but on a laptop or desktop device, you need to use extra hardware for this connection. One way is to use the Debug Probe.

Once OpenOCD is installed and the correct connection made, Eclipse needs to be set up to talk to OpenOCD when programs are run. OpenOCD provides a GDB interface to Eclipse, and it is that interface that is used when debugging.

To set up the OpenOCD system, select Preferences from the Window menu.

Click on MCU arrow to expand the options and click on Global OpenOCD path.

For the executable, type in “openocd”. For the folder, select the location in the file system where you have cloned the Pico OpenOCD fork from github.

Use Eclipse

**42**

Getting started with Raspberry Pi Pico-series

_Figure 13. Setting the OCD executable name and path in Eclipse._

**==> picture [319 x 207] intentionally omitted <==**

## **Creating a Run configuration**

In order to run or debug code in Eclipse you need to set up a Run Configuration. This sets up all the information needed to identify the code to run, any parameters, the debugger, source paths and SVD information.

From the Eclipse Run menu, select Run Configurations. To create a debugger configuration, select GDB OpenOCD Debugging option, then select the New Configuration button.

_Figure 14. Creating a new Run/Debug configuration in Eclipse._

**==> picture [319 x 207] intentionally omitted <==**

## **Setting up the application to run**

Because the pico-examples build creates lots of different application executables, you need to select which specific one is to be run or debugged.

On the Main tab of the Run configuration page, use the Browse option to select the C/C++ applications you wish to run.

The Eclipse build will have created the executables in sub folders of the Eclipse project folder. In our example case this is

…/pico-examples-eclipse/<name of example folder>/<optional name of example subfolder>/executable.elf

So for example, if we running the LED blink example, this can be found at:

Use Eclipse

**43**

Getting started with Raspberry Pi Pico-series

…/pico-examples-eclipse/blink/blink.elf

_Figure 15. Setting the executable to debug in Eclipse._

**==> picture [319 x 228] intentionally omitted <==**

## **Setting up the debugger**

Let’s set up OpenOCD to talk to the Raspberry Pi microcontroller.

Set openocd in the boxes labelled **Executable** and **Actual Executable** . We also need to set up OpenOCD to use the Pico specific configuration, so in the Config options sections add the following. You will need to change the path to point to the location where the Pico version of OpenOCD is installed.

- -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg

Replace rp2040.cfg with rp2350.cfg if you are using a RP2350-based device.

All other OpenOCD settings should be set to the default values.

The actual debugger used is GDB. This talks to the OpenOCD debugger for the actual communications with the Raspberry Pi microcontroller, but provides a standard interface to the IDE.

The particular version of GDB used is gdb-multiarch, so enter this in the fields labelled **Executable Name** and **Actual Executable** .

Use Eclipse

**44**

Getting started with Raspberry Pi Pico-series

_Figure 16. Setting up the Debugger and OpenOCD in Eclipse._

**==> picture [319 x 261] intentionally omitted <==**

## **Setting up the SVD plugin**

SVD provides a mechanism to view and set peripheral registers on the Pico board. An SVD file provides register locations and descriptions, and the SVD plugin for Eclipse integrates that functionality in to the Eclipse IDE. The SVD plugin comes as part of the Embedded development plugins.

Select the SVD path tab on the Launch configuration, and enter the location on the file system where the SVD file is located. This is usually found in the pico-sdk source tree.

## E.g.

…/pico-sdk/src/rp2040/hardware_regs/RP2040.svd or …/pico-sdk/src/rp2350/hardware_regs/RP2350.svd

_Figure 17. Setting the SVD path in Eclipse._

**==> picture [319 x 261] intentionally omitted <==**

Use Eclipse

**45**

Getting started with Raspberry Pi Pico-series

## **Running the Debugger**

Once the Run configuration is complete and saved, you can launch immediately using the Run button at the bottom right of the dialog, or simply Apply the changes and Close the dialog. You can then run the application using the Run Menu Debug option.

This will set Eclipse in to debug perspective, which will display a multitude of different debug and source code windows, along with the very useful Peripherals view which uses the SVD data to provide access to peripheral registers. From this point on this is a standard Eclipse debugging session.

_Figure 18. The Eclipse debugger running, showing some of the debugging window available._

**==> picture [319 x 170] intentionally omitted <==**

## **Use CLion**

CLion is a multiplatform Integrated Development environment (IDE) from JetBrains, available for Linux, Windows and Mac. This is a commercial IDE often the choice of professional developers (or those who love JetBrains IDEs) although there are free or reduce price licenses available. It _will_ run on a Raspberry Pi, however the performance is not ideal, so it is expected you would be using CLion on your desktop or laptop.

Whilst setting up projects, development and building are a breeze, setting up debug is still not very mainstream at the moment, so be warned.

## **Setting up CLion**

If you are planning to use CLion we assume you either have it installed or can install it from https://www.jetbrains.com/ clion/

## **Setting up a project**

Here we are using pico-examples as the example project.

To open the pico-examples project, select Open… from the File menu, and then navigate to and select the pico-examples directory you checked out, and press OK.

Once open you’ll see something like Figure 19.

Use CLion

**46**

Getting started with Raspberry Pi Pico-series

_Figure 19. A newly opened CLion picoexamples project._

**==> picture [319 x 226] intentionally omitted <==**

Notice at the bottom that CLion attempted to load the CMake project, but there was an error; namely that we hadn’t specified PICO_SDK_PATH

## **Configuring CMake Profiles**

Select Settings… from the File menu, and then navigate to and select 'CMake' under Build, Execution, Deployment.

You can set the environment variable PICO_SDK_PATH under Environment: as in Figure 20, or you can set it as -DPICO_SDK_PATH=xxx under CMake options:. These are just like the environment variables or command line args when calling cmake from the command line, so this is where you’d specify CMake settings such as PICO_BOARD, PICO_TOOLCHAIN_PATH etc.

_Figure 20. Configuring a CMake profile in CLion._

**==> picture [319 x 195] intentionally omitted <==**

Use CLion

**47**

Getting started with Raspberry Pi Pico-series

##  **IMPORTANT**

The SDK builds binaries for the Raspberry Pi Pico by default. To build a binary for a different board, pass the -DPICO_BOARD=<board> option to CMake, replacing the <board> placeholder with the name of the board you’d like to target. To build a binary for Pico 2, pass -DPICO_BOARD=pico2. To build a binary for Pico W, pass -DPICO_BOARD=pico_w. To specify a Wi-Fi network and password that your Pico W should connect to, pass -DWIFI_SSID="Your Network" -DWIFI_PASSWORD="Your Password".

You can have as many CMake profiles as you like with different settings. You probably want to add a Release build by hitting the + button, and then filling in the PICO_SDK_PATH again, or by hitting the copy button two to the right, and fixing the name and settings (see Figure 21)

_Figure 21. Configuring a second CMake Profile in CLion._

**==> picture [319 x 194] intentionally omitted <==**

After pressing OK, you’ll see something like Figure 22. Note that there are two tabs for the two profiles (Debug and Release) at the bottom of the window. In this case Release is selected, and you can see that the CMake setup was successful.

_Figure 22. Configuring a second CMake profile in CLion._

**==> picture [319 x 226] intentionally omitted <==**

## **Running a build**

Now we can choose to build one or more targets. For example you can navigate to the drop down selector in the middle of the toolbar, and select or starting typing hello_usb; then press the tool icon to its left to build (see Figure 23).

Use CLion

**48**

Getting started with Raspberry Pi Pico-series

Alternatively you can do a full build of all targets or other types of build from the Build menu.

_Figure 23._ hello_usb _successfully built._

**==> picture [319 x 232] intentionally omitted <==**

Note that the drop down selector lets you choose both the target you want to build and a CMake profile to use (in this case one of Debug or Release)

Another thing you’ll notice Figure 23 shows is that in the bottom status bar, you can see hello_usb and Debug again. These are showing you the target and CMake profile being used to control syntax highlighting etc. in the editor (This was auto selected when you chose hello_usb before). You can visually see in the stdio.c file that has been opened by the user, that PICO_STDIO_USB is set, but PICO_STDIO_UART is not (which are part of the configuration of hello_usb). Build time per binary configuration of libraries is heavily used within the SDK, so this is a very nice feature.

## **Build Artifacts**

The build artifacts are located under cmake-build-<profile> under the project root (see Figure 24). In this case this is the cmake-build-debug directory.

The UF2 file can be copied onto a Raspberry Pi microcontroller in BOOTSEL mode, or the ELF can be used for debugging.

_Figure 24. Locating the_ hello_usb _build artifacts_

**==> picture [319 x 232] intentionally omitted <==**

Use CLion

**49**

Getting started with Raspberry Pi Pico-series

## **Other Environments**

There are too development environments available to describe all of them here. You can use many of them with the SDK. In general, IDEs require the following features to support Pico-series devices:

- [CMake integration]

- [GDB support with remote options]

- [SVD. Not essential but makes reading peripheral status much easier]

- [Optional Arm embedded development plugin. These types of plugin often make support much easier.]

Other Environments

**50**
