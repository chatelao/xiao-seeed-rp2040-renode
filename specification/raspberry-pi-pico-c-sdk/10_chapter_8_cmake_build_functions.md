Raspberry Pi Pico-series C/C++ SDK

## **Chapter 8. CMake build functions**

Use CMake functions to customize SDK builds.

## **8.1. SDK CMake Functions**

## **8.1.1. Boot Stage 2**

CMake functions to create stage 2 bootloaders

pico_clone_default_boot_stage2(NAME)

Clone the default boot stage 2 target.

pico_define_boot_stage2(NAME SOURCES) Define a boot stage 2 target.

## **8.1.2. Pico Binary Info**

CMake functions to add binary info to the output binary

pico_set_program_description(TARGET description)

Set the program description for the target

pico_set_program_name(TARGET name)

Set the program name for the target

pico_set_program_url(TARGET url)

Set the program URL for the target

pico_set_program_version(TARGET version) Set the program version for the target

## **8.1.3. Pico BTstack**

CMake functions to configure the bluetooth stack

pico_btstack_make_gatt_header(TARGET_LIB TARGET_TYPE GATT_FILE) Make a GATT header file from a BTstack GATT file.

## **8.1.4. Pico CYW43 Driver**

CMake functions to configure the CYW43 driver

pico_configure_ip4_address(TARGET_LIB TARGET_TYPE DEF_NAME IP_ADDRESS_STR)

Set an ip address in a compile definition

pico_use_wifi_firmware_partition(TARGET [NO_EMBEDDED_PT])

Use a partition for the Wi-Fi firmware

8.1. SDK CMake Functions

**573**

Raspberry Pi Pico-series C/C++ SDK

## **8.1.5. Pico LwIP**

CMake functions to configure LwIP

pico_set_lwip_httpd_content(TARGET_LIB TARGET_TYPE HTTPD_FILES…)

Compile the http content into a source file for lwip.

## **8.1.6. Pico PIO**

CMake functions to generate PIO headers

pico_generate_pio_header(TARGET PIO_FILES… [OUTPUT_FORMAT <format>] [OUTPUT_DIR <dir>]) Generate pio header and include it in the build

## **8.1.7. Pico Runtime**

CMake functions to configure the runtime environment

pico_minimize_runtime(TARGET [INCLUDE …] [EXCLUDE …]) Minimize the runtime components for the target

## **8.1.8. Pico Standard Link**

CMake functions to configure the linker

pico_add_link_depend(TARGET dependency)

Add a link time dependency to the target

pico_check_linker_script(LDSCRIPT)

Check the linker script for compatibility

pico_set_binary_type(TARGET TYPE)

Set the binary type for the target

pico_set_linker_script(TARGET LDSCRIPT) Set the linker script for the target

## **8.1.9. Pico Standard I/O**

CMake functions to configure the standard I/O library

pico_enable_stdio_rtt(TARGET ENABLED)

Enable stdio RTT for the target

pico_enable_stdio_semihosting(TARGET ENABLED) Enable stdio semi-hosting for the target

pico_enable_stdio_uart(TARGET ENABLED) Enable stdio UART for the target

pico_enable_stdio_usb(TARGET ENABLED) Enable stdio USB for the target

8.1. SDK CMake Functions

**574**

Raspberry Pi Pico-series C/C++ SDK

## **8.1.10. Other**

Other CMake functions

pico_add_bin_output(TARGET)

Generate a bin file for the target

pico_add_dis_output(TARGET)

Generate a disassembly file for the target

pico_add_hex_output(TARGET)

Generate a hex file for the target

pico_add_uf2_output(TARGET)

Add a UF2 output using picotool

pico_add_extra_outputs(TARGET)

Perform post-build actions for the target

pico_embed_pt_in_binary(TARGET PTFILE)

Embed a partition table in the binary

pico_encrypt_binary(TARGET AESFILE IVFILE [SIGFILE <file>] [EMBED] [MBEDTLS] [OTP_KEY_PAGE <page>])

Encrypt the taget binary

pico_hash_binary(TARGET)

Hash the target binary.

pico_sign_binary(TARGET [SIGFILE])

Sign the target binary with the given PEM signature.

pico_ensure_load_map(TARGET)

Ensure a load map is added to the target.

pico_load_map_clear_sram(TARGET)

Adds a load map entry to clear all of SRAM

pico_package_uf2_output(TARGET [PACKADDR])

Package a UF2 output to be written to the PACKADDR address.

pico_set_binary_version(TARGET [MAJOR <version>] [MINOR <version>] [ROLLBACK <version>] [ROWS <rows…>])

Add a version item to the metadata block

pico_set_otp_key_output_file(TARGET OTPFILE)

Output the public key hash and other necessary rows to an otp JSON file.

pico_set_uf2_family(TARGET FAMILY)

Set the UF2 family to use when creating the UF2.

## **8.2. Alphabetical List of SDK CMake Functions**

## **8.2.1. pico_add_bin_output**

pico_add_bin_output(TARGET)

Generate a bin file for the target

8.2. Alphabetical List of SDK CMake Functions

**575**

Raspberry Pi Pico-series C/C++ SDK

## **8.2.2. pico_add_dis_output**

pico_add_dis_output(TARGET)

Generate a disassembly file for the target

## **8.2.3. pico_add_extra_outputs**

pico_add_extra_outputs(TARGET)

Perform picotool processing and add disassembly, hex, bin, map, and uf2 outputs for the target

## **8.2.4. pico_add_hex_output**

pico_add_hex_output(TARGET)

Generate a hex file for the target

## **8.2.5. pico_add_link_depend**

pico_add_link_depend(TARGET dependency)

Add a link time dependency to the target

## **Parameters**

> dependency The dependency to add

## **8.2.6. pico_add_uf2_output**

pico_add_uf2_output(TARGET)

Add a UF2 output using picotool - must be called after all required properties have been set

## **8.2.7. pico_btstack_make_gatt_header**

pico_btstack_make_gatt_header(TARGET_LIB TARGET_TYPE GATT_FILE)

Make a GATT header file from a BTstack GATT file.

Pass the target library name, library type, and path to the GATT input file. To add additional directories to the gatt import path, add them to the end of the argument list.

## **Parameters**

> TARGET_LIB The target library name

> TARGET_TYPE The target library type

> GATT_FILE The path to the GATT input file

## **8.2.8. pico_check_linker_script**

pico_check_linker_script(LDSCRIPT)

Checks the linker script for compatibility with the current SDK version, and if not, raises warnings and enables

8.2. Alphabetical List of SDK CMake Functions

**576**

Raspberry Pi Pico-series C/C++ SDK

workarounds to maintain compatibility where possible.

## **Parameters**

> LDSCRIPT Full path to the linker script to check

## **8.2.9. pico_clone_default_boot_stage2**

pico_clone_default_boot_stage2(NAME)

Create a new boot stage 2 target using the default implementation for the current build (PICO_BOARD derived)

## **Parameters**

> NAME The name of the new boot stage 2 target

## **8.2.10. pico_configure_ip4_address**

pico_configure_ip4_address(TARGET_LIB TARGET_TYPE DEF_NAME IP_ADDRESS_STR)

Set an ip address in a compile definition This can be used to set the following compile definitions CYW43_DEFAULT_IP_STA_ADDRESS CYW43_DEFAULT_IP_STA_GATEWAY CYW43_DEFAULT_IP_AP_ADDRESS CYW43_DEFAULT_IP_AP_GATEWAY CYW43_DEFAULT_IP_MASK CYW43_DEFAULT_IP_DNS e.g. pico_configure_ip4_address(picow_tcpip_server_background PRIVATE CYW43_DEFAULT_IP_STA_ADDRESS "10.3.15.204")

## **Parameters**

> TARGET_LIB The target library to set the ip address for

> TARGET_TYPE The type of target library

> DEF_NAME The name of the compile definition to set

> IP_ADDRESS_STR The ip address to set

## **8.2.11. pico_define_boot_stage2**

pico_define_boot_stage2(NAME SOURCES)

Define a boot stage 2 target.

By convention the first source file name without extension is used for the binary info name

## **Parameters**

> NAME The name of the boot stage 2 target

> SOURCES The source files to link into the boot stage 2

## **8.2.12. pico_embed_pt_in_binary**

pico_embed_pt_in_binary(TARGET PTFILE)

Create the specified partition table from JSON, and embed it in the block loop.

8.2. Alphabetical List of SDK CMake Functions

**577**

Raspberry Pi Pico-series C/C++ SDK

This sets the target property PICOTOOL_EMBED_PT to PTFILE.

## **Parameters**

> PTFILE The partition table JSON file to use

## **8.2.13. pico_enable_stdio_rtt**

pico_enable_stdio_rtt(TARGET ENABLED)

Enable stdio RTT for the target

## **Parameters**

> ENABLED Whether to enable stdio RTT

## **8.2.14. pico_enable_stdio_semihosting**

pico_enable_stdio_semihosting(TARGET ENABLED)

Enable stdio semi-hosting for the target

## **Parameters**

> ENABLED Whether to enable stdio semi-hosting

## **8.2.15. pico_enable_stdio_uart**

pico_enable_stdio_uart(TARGET ENABLED)

Enable stdio UART for the target

## **Parameters**

> ENABLED Whether to enable stdio UART

## **8.2.16. pico_enable_stdio_usb**

pico_enable_stdio_usb(TARGET ENABLED)

Enable stdio USB for the target

## **Parameters**

> ENABLED Whether to enable stdio USB

## **8.2.17. pico_encrypt_binary**

pico_encrypt_binary(TARGET AESFILE IVFILE [SIGFILE <file>] [EMBED] [MBEDTLS] [OTP_KEY_PAGE <page>])

Encrypt the target binary with the given AES key (should be a binary file containing 128 bytes of a random key share, or 32 bytes of a random key), and sign the encrypted binary.

Salts the public IV with the provided IVFILE (should be a binary file containing 16 bytes of a random IV), to give the IV used by the encryption.

This sets the target properties PICOTOOL_AESFILE to AESFILE, PICOTOOL_IVFILE to IVFILE, and PICOTOOL_ENC_SIGFILE to SIGFILE if specified, else PICOTOOL_SIGFILE.

8.2. Alphabetical List of SDK CMake Functions

**578**

Raspberry Pi Pico-series C/C++ SDK

Optionally, use EMBED to embed a decryption stage into the encrypted binary. This sets the target property PICOTOOL_EMBED_DECRYPTION to TRUE.

Optionally, use MBEDTLS to to use the MbedTLS based decryption stage - this is faster, but offers no security against power or timing sniffing attacks, and takes up more code size. This sets the target property PICOTOOL_USE_MBEDTLS_DECRYPTION to TRUE.

Optionally, use OTP_KEY_PAGE to specify the OTP page storing the AES key. This sets the target property PICOTOOL_OTP_KEY_PAGE to OTP_KEY_PAGE.

## **Parameters**

> AESFILE The AES key file to use

> IVFILE The IV file to use

> SIGFILE The PEM signature file to use

> EMBED Embed a decryption stage into the encrypted binary

> MBEDTLS Use MbedTLS based decryption stage (faster, but less secure)

> OTP_KEY_PAGE The OTP page storing the AES key

## **8.2.18. pico_ensure_load_map**

pico_ensure_load_map(TARGET)

Ensure a load map is added to the target. This can be used to ensure a load map is present, so the bootrom knows where to load the binary if it’s stored in a different location (e.g. a packaged binary).

This sets the target property PICOTOOL_LOAD_MAP to true.

## **8.2.19. pico_generate_pio_header**

pico_generate_pio_header(TARGET PIO_FILES… [OUTPUT_FORMAT <format>] [OUTPUT_DIR <dir>])

Generate pio header and include it in the build

## **Parameters**

> PIO_FILES The PIO files to generate the header for

> OUTPUT_FORMAT The output format to use for the pio header

> OUTPUT_DIR The directory to output the pio header to

## **8.2.20. pico_hash_binary**

pico_hash_binary(TARGET)

Hash the target binary.

This sets the target property PICOTOOL_HASH_OUTPUT to true.

## **8.2.21. pico_load_map_clear_sram**

pico_load_map_clear_sram(TARGET)

Adds an entry to the load map to instruct the bootrom to clear all of SRAM before loading the binary.

8.2. Alphabetical List of SDK CMake Functions

**579**

Raspberry Pi Pico-series C/C++ SDK

This appends the --clear argument to the target property PICOTOOL_EXTRA_PROCESS_ARGS.

## **8.2.22. pico_minimize_runtime**

pico_minimize_runtime(TARGET [INCLUDE …] [EXCLUDE …])

Minimize the runtime components for the target

INCLUDE/EXCLUDE can contain any of the following (all defaulting to not included)

DEFAULT_ALARM_POOL - default alarm pool setup PRINTF - full printf support PRINTF_MINIMAL - printf support without the following PRINTF_FLOAT - to control float support if printf is enabled PRINTF_EXPONENTIAL - to control exponential support if printf is enabled PRINTF_LONG_LONG - to control long long support if printf is enabled PRINTF_PTRDIFF_T - to control ptrdiff_t support if printf is enabled FLOAT - support for single-precision floating point DOUBLE - support for double-precision floating point

FPGA_CHECK - checks for FPGA which allows Raspberry Pi to run your binary on FPGA PANIC - default panic impl which brings in stdio

AUTO_INIT_MUTEX - auto init mutexes, without this you get no printf mutex either CRT0_FAR_CALLS - use blx not bl for calls from crt0 to user overridable functions;

## **Parameters**

> INCLUDE The items to include

> EXCLUDE The items to exclude

## **8.2.23. pico_package_uf2_output**

pico_package_uf2_output(TARGET [PACKADDR])

Package a UF2 output to be written to the PACKADDR address. This can be used with a no_flash binary to write the UF2 to flash when dragging & dropping, and it will be copied to SRAM by the bootrom before execution.

This sets the target property PICOTOOL_UF2_PACKAGE_ADDR to PACKADDR and calls pico_ensure_load_map(TARGET).

**Parameters**

> PACKADDR The address to package the UF2 to, defaults to start of flash

## **8.2.24. pico_set_binary_type**

pico_set_binary_type(TARGET TYPE)

Set the binary type for the target

## **Parameters**

> TYPE The binary type to set

## **8.2.25. pico_set_binary_version**

pico_set_binary_version(TARGET [MAJOR <version>] [MINOR <version>] [ROLLBACK <version>] [ROWS <rows…>])

8.2. Alphabetical List of SDK CMake Functions

**580**

Raspberry Pi Pico-series C/C++ SDK

Adds a version item to the metadata block, with the given major, minor and rollback version, along with the rollback rows.

These are appended as arguments to the target property PICOTOOL_EXTRA_PROCESS_ARGS if setting the rollback version, or set as compile definitions if only setting the major/minor versions.

## **Parameters**

> MAJOR The major version to set

> MINOR The minor version to set

> ROLLBACK The rollback version to set

> ROWS The OTP rows to use for the rollback version

## **8.2.26. pico_set_linker_script**

pico_set_linker_script(TARGET LDSCRIPT)

Set the linker script for the target

## **Parameters**

> LDSCRIPT Full path to the linker script to set

## **8.2.27. pico_set_lwip_httpd_content**

pico_set_lwip_httpd_content(TARGET_LIB TARGET_TYPE HTTPD_FILES…)

Compile the http content into a source file "pico_fsdata.inc" in a format suitable for the lwip httpd server. Pass the target library name, library type, and the list of httpd content files to compile.

## **Parameters**

> TARGET_LIB The target library name

> TARGET_TYPE The type of the target library

> HTTPD_FILES The list of httpd content files to compile

## **8.2.28. pico_set_otp_key_output_file**

pico_set_otp_key_output_file(TARGET OTPFILE)

Output the public key hash and other necessary rows to an otp JSON file.

This sets the target property PICOTOOL_OTP_FILE to OTPFILE.

## **Parameters**

> OTPFILE The OTP file to output the public key hash and other necessary rows to

## **8.2.29. pico_set_program_description**

pico_set_program_description(TARGET description)

Set the program description for the target

## **Parameters**

> description The program description to set

8.2. Alphabetical List of SDK CMake Functions

**581**

Raspberry Pi Pico-series C/C++ SDK

## **8.2.30. pico_set_program_name**

pico_set_program_name(TARGET name)

Set the program name for the target

## **Parameters**

> name The program name to set

## **8.2.31. pico_set_program_url**

pico_set_program_url(TARGET url)

Set the program URL for the target

## **Parameters**

> url The program URL to set

## **8.2.32. pico_set_program_version**

pico_set_program_version(TARGET version)

Set the program version for the target

## **Parameters**

> version The program version to set

## **8.2.33. pico_set_uf2_family**

pico_set_uf2_family(TARGET FAMILY)

Set the UF2 family to use when creating the UF2.

This sets the target property PICOTOOL_UF2_FAMILY to FAMILY.

## **Parameters**

> FAMILY The UF2 family to use

## **8.2.34. pico_sign_binary**

pico_sign_binary(TARGET [SIGFILE])

Sign the target binary with the given PEM signature.

This sets the target properties PICOTOOL_SIGN_OUTPUT to true, PICOTOOL_SIGFILE to SIGFILE (if specified), and PICOTOOL_OTP_FILE to ${TARGET}.otp.json (if not already set).

To specify a common SIGFILE for multiple targets, the SIGFILE property can be set for a given scope, and then the SIGFILE argument is optional.

## **Parameters**

> SIGFILE The PEM signature file to use

8.2. Alphabetical List of SDK CMake Functions

**582**

Raspberry Pi Pico-series C/C++ SDK

## **8.2.35. pico_use_wifi_firmware_partition**

pico_use_wifi_firmware_partition(TARGET [NO_EMBEDDED_PT])

Use a partition for the Wi-Fi firmware

This will read the CYW43 firmware from a partition with the ID 0x776966696669726d, instead of embedding the firmware blob in the binary. By default it will also embed a compatible partition table in the binary, but this can be disabled by passing the NO_EMBEDDED_PT argument (for example, if you need to chain into the binary, it can’t contain a partition table).

This will create additional UF2 files for the CYW43 firmware - both a regular version, and a TBYB version to use if you’re updating it using the TBYB feature (see section 5.1.17 of the RP2350 datasheet). You will need to flash your chosen version to each new device once, after loading the partition table. For example using picotool:

picotool load TARGET.uf2 picotool reboot -u picotool load -ux TARGET_wifi_firmware.uf2

Then on subsequent updates, you can just flash the TARGET.uf2 file to the device.

## **Parameters**

> NO_EMBEDDED_PT If set, will not embed a partition table in the binary

8.2. Alphabetical List of SDK CMake Functions

**583**
