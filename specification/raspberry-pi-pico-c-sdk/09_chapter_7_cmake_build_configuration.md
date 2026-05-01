Raspberry Pi Pico-series C/C++ SDK

## **Chapter 7. CMake build configuration**

Use CMake cache variables to customize SDK builds.

## **7.1. Full List of SDK Configuration Variables**

_Table 37. SDK CMake Configuration Variables_

|**Name / Description**|**Default**|
|---|---|
|**PICO_ALLOW_EXAMPLE_KEYS**<br>Don’t raise a warning when using default signing/encryption keys|0|
|**PICO_BARE_METAL**<br>Flag to exclude anything except base headers from the build|0|
|**PICO_BOARD**<br>Board name being built for. This may be specified in the user environment<br>(seeSection 7.2)|pico or pico2|
|**PICO_BOARD_CMAKE_DIRS**<br>List of directories to look for <PICO_BOARD>.cmake in. This may be specified<br>in the user environment||
|**PICO_BOARD_HEADER_DIRS**<br>List of directories to look for <PICO_BOARD>.h in. This may be specified the<br>user environment||
|**PICO_BTSTACK_PATH**<br>Path to BTstack. Can be passed to CMake or set in your environment if you do<br>not wish to use the version included with the SDK|<PICO_SDK_PATH>/lib/btstack|
|**PICO_CLIB**<br>The C library to use e.g. newlib/picolibc/llvm_libc (seeSection 7.3)|based on PICO_COMPILER|
|**PICO_CMAKE_PRELOAD_PLATFORM_FILE**<br>Custom CMake file to use to set up the platform environment||
|**PICO_COMPILER**<br>Specifies the compiler family to use (seeSection 7.3)|PICO_DEFAULT_COMPILER which is<br>set based on PICO_PLATFORM|
|**PICO_CONFIG_HEADER_FILES**<br>List of extra header files to include from pico/config.h for all platforms||
|**PICO_COPY_TO_RAM**<br>Option to default all binaries to copy code from flash to SRAM before running<br>(seeSection 7.4)|0|
|**PICO_CXX_ENABLE_CXA_ATEXIT**<br>Enable cxa-atexit|0|
|**PICO_CXX_ENABLE_EXCEPTIONS**<br>Enable CXX exception handling|0|
|**PICO_CXX_ENABLE_RTTI**<br>Enable CXX rtti|0|



7.1. Full List of SDK Configuration Variables

**566**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_CYW43_DRIVER_PATH**<br>Path to cyw43-driver. Can be passed to CMake or set in your environment if<br>you do not wish to use the version included with the SDK|<PICO_SDK_PATH>/lib/cyw43-driver|
|**PICO_DEBUG_INFO_IN_RELEASE**<br>Include debug information in release builds (seeSection 7.3)|1|
|**PICO_DEFAULT_BOARD_host**<br>The default PICO_BOARD when PICO_PLATFORM is host (seeSection 7.2)|none|
|**PICO_DEFAULT_BOARD_rp2040**<br>The default PICO_BOARD when PICO_PLATFORM is rp2040 (seeSection 7.2)|pico|
|**PICO_DEFAULT_BOARD_rp2350**<br>The default PICO_BOARD when PICO_PLATFORM is rp2350 (seeSection 7.2)|pico2|
|**PICO_DEFAULT_BOARD_rp2350-arm-s**<br>The default PICO_BOARD when PICO_PLATFORM is rp2350-arm-s (see<br>Section 7.2)|pico2|
|**PICO_DEFAULT_BOARD_rp2350-riscv**<br>The default PICO_BOARD when PICO_PLATFORM is rp2350-riscv (seeSection<br>7.2)|pico2|
|**PICO_DEFAULT_BOOT_STAGE2**<br>Simpler alternative to specifying PICO_DEFAULT_BOOT_STAGE2_FILE where<br>the latter is set to<br>src/rp2_common/boot_stage2/{PICO_DEFAULT_BOOT_STAGE2}.S|compile_time_choice|
|**PICO_DEFAULT_BOOT_STAGE2_FILE**<br>Default boot stage 2 file to use unless overridden by pico_set_boot_stage2 on<br>the TARGET; this setting is useful when explicitly setting the default build from<br>a per board CMake file||
|**PICO_DEFAULT_PIOASM_OUTPUT_FORMAT**<br>Default output format used by pioasm when using pico_generate_pio_header|c-sdk|
|**PICO_DEFAULT_PLATFORM**<br>The default for PICO_PLATFORM if not specified (seeSection 7.2)|rp2040|
|**PICO_DEFAULT_RP2350_PLATFORM**<br>Default actual platform to build for if rp2350 is specified for PICO_PLATFORM<br>e.g. rp2350-arm-s/rp2350-riscv (seeSection 7.2)|rp2350-arm-s|
|**PICO_DEFAULT_UART_BAUD_RATE**<br>Define the default UART baudrate|115200|
|**PICO_DEOPTIMIZED_DEBUG**<br>Disable all compiler optimization in debug builds (seeSection 7.3)|0|
|**PICO_GCC_TRIPLE**<br>List of GCC_TRIPLES — usually only one — to try when searching for a<br>compiler. This may be specified the user environment (seeSection 7.3)|PICO_DEFAULT_GCC_TRIPLE which is<br>set based on PICO_COMPILER|
|**PICO_HOST_CONFIG_HEADER_FILES**<br>List of extra header files to include from pico/config.h for the host platform<br>only||
|**PICO_LWIP_PATH**<br>Path to lwIP. Can be passed to CMake or set in your environment if you do not<br>wish to use the version included with the SDK|<PICO_SDK_PATH>/lib/lwip|



7.1. Full List of SDK Configuration Variables

**567**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_MBEDTLS_PATH**<br>Path to Mbed TLS. Can be passed to CMake or set in your environment if you<br>do not wish to use the version included with the SDK|<PICO_SDK_PATH>/lib/mbedtls|
|**PICO_NO_CMSE**<br>Disable CMSE compiler extensions (seeSection 7.3)|0|
|**PICO_NO_COPRO_DIS**<br>Disable disassembly listing postprocessing that disassembles RP2350<br>coprocessor instructions|0|
|**PICO_NO_FLASH**<br>Option to default all binaries to not use flash i.e. run from SRAM (seeSection<br>7.4)|0|
|**PICO_NO_GC_SECTIONS**<br>Disable-ffunction-sections -fdata-sectionsand--gc-sections|0|
|**PICO_NO_HARDWARE**<br>Option as to whether the build is not targeting an RP2040 or RP2350 device|1 when PICO_PLATFORM is host, 0<br>otherwise|
|**PICO_NO_PICOTOOL**<br>Disable use/requirement for picotool meaning that UF2 output and<br>signing/hashing and coprocoessor disassembly will all be unavailable|0|
|**PICO_NO_TARGET_NAME**<br>Don’t define PICO_TARGET_NAME|0|
|**PICO_NO_UF2**<br>Disable UF2 output|0|
|**PICO_ON_DEVICE**<br>Option as to whether the build is targeting an RP2040 or RP2350 device|0 when PICO_PLATFORM is host, 1<br>otherwise|
|**PICO_PLATFORM**<br>Platform to build for e.g. rp2040/rp2350/rp2350-arm-s/rp2350-riscv/host.<br>This may be specified in the user environment (seeSection 7.2)|based on PICO_BOARD or<br>environment value|
|**PICO_RP2040_CONFIG_HEADER_FILES**<br>List of extra header files to include from pico/config.h for the rp2040 platform<br>only||
|**PICO_RP2350_ARM_S_CONFIG_HEADER_FILES**<br>List of extra header files to include from pico/config.h for the rp2350-arm-s<br>platform only||
|**PICO_RP2350_RISCV_CONFIG_HEADER_FILES**<br>List of extra header files to include from pico/config.h for the riscv platform<br>only||
|**PICO_SDK_VERSION_MAJOR**<br>SDK major version number|Current SDK major version|
|**PICO_SDK_VERSION_MINOR**<br>SDK minor version number|Current SDK minor version|
|**PICO_SDK_VERSION_PRE_RELEASE_ID**<br>Optional SDK pre-release version identifier|Current SDK pre-release identifier|
|**PICO_SDK_VERSION_REVISION**<br>SDK version revision|Current SDK revision|



7.1. Full List of SDK Configuration Variables

**568**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_SDK_VERSION_STRING**<br>SDK version string|Current SDK version string|
|**PICO_STDIO_RTT**<br>Option to globally enable stdio RTT for all targets by default|0|
|**PICO_STDIO_SEMIHOSTING**<br>Option to globally enable stdio semi-hosting for all targets by default|0|
|**PICO_STDIO_UART**<br>Option to globally enable stdio UART for all targets by default|1|
|**PICO_STDIO_USB**<br>Option to globally enable stdio USB for all targets by default|0|
|**PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS**<br>Maximum number of milliseconds to wait during initialization for a CDC<br>connection from the host (negative means indefinite) during initialization|0|
|**PICO_TINYUSB_PATH**<br>Path to TinyUSB. Can be passed to CMake or set in your environment if you do<br>not wish to use the version included with the SDK|<PICO_SDK_PATH>/lib/tinyusb|
|**PICO_TOOLCHAIN_DIR**<br>Path to search for toolchain CMake files (seeSection 7.3)|<sdk_path>/preload/toolchains|
|**PICO_TOOLCHAIN_PATH**<br>Path to search for compiler (seeSection 7.3)|none (i.e. search system paths)|
|**PICO_USE_DEFAULT_MAX_PAGE_SIZE**<br>Don’t shrink linker max page to 4096|0|



## **7.2. Platform and Board Configuration**

Passing PICO_BOARD=my_board_name to the CMake build (or specifying it in your environment) will cause the header my_board_name.h to be included by all other SDK headers in order to provide #defines particular to the board you are using.

You may also wish to specify your own board configuration in which case you can set PICO_BOARD_HEADER_DIRS in the environment or CMake to a semicolon separated list of paths to search for my_board_name.h.

On previous versions of the SDK there was generally little need for setting PICO_PLATFORM as the default value rp2040 selected RP2040 - the one and only RP-series microcontroller at the time.

SDK version 2.0.0 now supports the following RP-series microcontroller platforms along with the pre-existing value host that can be used to build code for testing.

## rp2040

Building for RP2040

rp2350-arm-s

Building for RP2350 on Arm processors; the "s" stands for secure, and means the binary runs directly from the bootrom, when the processor is still in secure mode.

rp2350-riscv

Building for RP2350 on RISC-V processors.

Individual manufactured boards will usually support only one of either RP2040 or RP2350. To avoid having to specify PICO_PLATFORM in addition to PICO_BOARD, specifying the latter can now automatically set the former (or vice versa).

The following steps are applied in order, with the results from the previous step being used in the next:

7.2. Platform and Board Configuration

**569**

Raspberry Pi Pico-series C/C++ SDK

1. If neither PICO_BOARD or PICO_PLATFORM is specified, PICO_PLATFORM defaults to PICO_DEFAULT_PLATFORM which itself defaults to rp2040

2. If PICO_PLATFORM is specified and not PICO_BOARD, then PICO_BOARD is defaulted based on the value of PICO_PLATFORM:

   - [pico][ for ][PICO_PLATFORM=rp2040]

   - [pico2][ for ][PICO_PLATFORM=rp2350-arm-s][ or ][PICO_PLATFORM=rp2350-riscv]

3. If PICO_BOARD is specified but not PICO_PLATFORM, PICO_PLATFORM will be set if a value for it is specified in the board header.

Because most RP2350 boards allow both Arm and RISC-V development, rp2350 is also a valid value for PICO_PLATFORM, and is often specified by a board header in step 3 above, but is always replaced with the value of PICO_DEFAULT_RP2350_PLATFORM to allow the user their own preference. PICO_DEFAULT_RP2350_PLATFORM defaults to rp2350-arm-s if not otherwise specified.

##  **NOTE**

Both PICO_PLATFORM and PICO_BOARD are **latched** if they have been specified via the environment, on the first CMake configuration; i.e. the value from the environment will not be used when configuring CMake subsequently in the same existing build directory.

## **7.3. Compiler and Toolchain Configuration**

The SDK supports building for Arm Cortex-M0 plus processors on RP2040 and for both Arm Cortex-M33 processors and RISC-V Hazard3 processors on RP2350.

The SDK also supports building with either GCC or LLVM (clang) on Arm. See Section 2.10 for more details of supported compilers.

## **7.3.1. Variables**

The following variables are used to find and configure the right compiler.

## **7.3.1.1. PICO_COMPILER**

This is usually defaulted for you to a GCC compiler based on PICO_PLATFORM. However, you can select one of the following values

- [pico_arm_gcc][ - Selects ][pico_arm_cortex_m0plus_gcc][ on RP2040 and ][pico_arm_cortex_m33_gcc][ on RP2350]

- [pico_arm_cortex_m0plus_gcc][ - Configures GCC to build for Arm Cortex-M0 plus]

- [pico_arm_cortex_m33_gcc][ - Configures GCC to build for Arm Cortex-M33]

- [pico_arm_clang][ - Selects ][pico_arm_cortex_m0plus_clang][ on RP2040 and ][pico_arm_cortex_m33_clang][ on RP2350]

- [pico_arm_cortex_m0plus_clang][ - Configures LLVM/clang to build for Arm Cortex-M0 plus]

- [pico_arm_cortex_m33_clang][ - Configures LLVM/clang to build for Arm Cortex-M33]

- [pico_riscv_gcc][ - Configures GCC to build for RISC-V Hazard3]

- [pico_riscv_gcc_zcb_zcmp][ - Configures GCC to build for RISC-V Hazard3 using ][zcb][ and ][zcmp][ extensions that aren’t] supported by all compilers

7.3. Compiler and Toolchain Configuration

**570**

Raspberry Pi Pico-series C/C++ SDK

## **7.3.1.2. PICO_GCC_TRIPLE**

This specifies one or more compiler "triples" to try when looking for a GCC compiler.

On Arm this defaults to arm-none-eabi.

On RISC-V this defaults to riscv32-unknown-elf;riscv32-corev-elf i.e. the two most common options are supported.

## **7.3.1.3. PICO_TOOLCHAIN_PATH**

Armed with PICO_COMPILER and PICO_GCC_TRIPLE (if using GCC) the SDK will then search for a compiler. By default, it searches the path, but PICO_TOOLCHAIN_PATH may be set to specify the root directory of a compiler toolchain install.

## **7.3.1.4. PICO_CLIB**

Most programs for the SDK require a C-library. Generally your installed compiler will include the toolchain. In this case, the SDK will try to detect either of the following runtimes, as which one is used effects how the SDK interacts with it.

- [newlib]

- [picolibc]

- [llvm-libc]

The SDK sets PICO_CLIB to one of these values, however you can set it yourself first if you want to force a choice.

## **7.4. Binary Type configuration**

These variables control how executables for RP-series microcontroller are laid out in memory. The default is for the code and data to be entirely stored in flash with writable data (and some specifically marked) methods to copy into RAM at startup.

|RAM at startup.|||
|---|---|---|
|Variable name|Values|Result|
|PICO_DEFAULT_BINARY_TYPE|default|Stores binaries in flash storage. Runs binaries from flash storage.|
||no_flash|Stores binaries in memory. Runs binaries from memory. Does not<br>require any flash storage. Note: You must reload this type of binary<br>after every reboot via UF2 file or debugger.|
||copy_to_ram|Stores binaries in flash, but copies them to memory (RAM) before<br>executing.|
||blocked_ram||
|PICO_NO_FLASH|0/1|Equivalent toPICO_DEFAULT_BINARY_TYPE=no_flashif=1.|
|PICO_COPY_TO_RAM|0/1|Equivalent toPICO_DEFAULT_BINARY_TYPE=copy_to_ramif=1.|
|PICO_USE_BLOCKED_RAM|0/1|Equivalent toPICO_DEFAULT_BINARY_TYPE=blocked_ramif=1.|



7.4. Binary Type configuration

**571**

Raspberry Pi Pico-series C/C++ SDK

##  **TIP**

You can set the binary type for each executable target (as created by add_executable) by calling pico_set_binary_type(target type) using the same type as PICO_DEFAULT_BINARY_TYPE.

7.4. Binary Type configuration

**572**
