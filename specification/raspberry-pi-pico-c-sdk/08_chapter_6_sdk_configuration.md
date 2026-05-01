Raspberry Pi Pico-series C/C++ SDK

## **Chapter 6. SDK configuration**

SDK configuration is the process of customising the SDK code for your particular build/application. As the parts of the SDK that you use are recompiled as part of your build, configuration options can be chosen at compile time resulting in smaller and more efficient customized versions of the code.

This chapter will show what configuration parameters are available, and how they can be changed.

SDK configuration parameters are passed as C preprocessor definitions to the build. The most common way to override them is to specify them in your CMakeLists.txt when you define your executable or library:

e.g.

add_executable(my_program main.c) ... target_compile_definitions(my_program PRIVATE PICO_STACK_SIZE=4096 )

or if you are creating a library, and you want to add compile definitions whenever your library is included:

add_library(my_library INTERFACE) ... target_compile_definitions(my_library INTERFACE PICO_STDIO_DEFAULT_CRLF=0 PICO_DEFAULT_UART=1 )

The definitions can also be specified in header files, as is commonly done for board configuration (see Chapter 9).

For example,. the Pimoroni Tiny2040 board header configures the following to specify appropriate board settings for the default I2C channel exposed on that board.

_// --- I2C --#ifndef PICO_DEFAULT_I2C #define PICO_DEFAULT_I2C 1 #endif #ifndef PICO_DEFAULT_I2C_SDA_PIN #define PICO_DEFAULT_I2C_SDA_PIN 2 #endif #ifndef PICO_DEFAULT_I2C_SCL_PIN #define PICO_DEFAULT_I2C_SCL_PIN 3 #endif_

##  **NOTE**

The #ifdef still allows the values to be by the build (i.e. in CMakeLists.txt)

If you would rather set values in your own header file rather than via CMake, then you must make sure the header is included by all compilation (including the SDK sources). Using a custom PICO_BOARD header is one way of doing this, but a more advanced way is to have the SDK include your header via pico/config.h which itself is included by every SDK source file.

Chapter 6. SDK configuration

**549**

Raspberry Pi Pico-series C/C++ SDK

This can be done by adding the following before the pico_sdk_init() in your CMakeLists.txt:

list(APPEND PICO_CONFIG_HEADER_FILES path/to/your/header.h)

## **6.1. Full List of SDK Configuration Defines**

_Table 36. SDK and Board Configuration Defines_

|**Name / Description**|**Default**|
|---|---|
|**CYW43_DEFAULT_PIN_WL_CLOCK**<br>gpio pin for the spi clock line to the cyw43 chip||
|**CYW43_DEFAULT_PIN_WL_CS**<br>gpio pin for the spi chip select to the cyw43 chip||
|**CYW43_DEFAULT_PIN_WL_DATA_IN**<br>gpio pin for spi data in from the cyw43 chip||
|**CYW43_DEFAULT_PIN_WL_DATA_OUT**<br>gpio pin for spi data out to the cyw43 chip||
|**CYW43_DEFAULT_PIN_WL_HOST_WAKE**<br>gpio (irq) pin for the irq line from the cyw43 chip||
|**CYW43_DEFAULT_PIN_WL_REG_ON**<br>gpio pin to power up the cyw43 chip||
|**CYW43_FIRMWARE_PARTITION_ID**<br>ID of Wi-Fi firmware partition which must match the ID used in the partition<br>table JSON|0x776966696669726d|
|**CYW43_LWIP_DEFAULT**<br>Sets the default value of CYW43_LWIP if it’s undefined. CYW43_LWIP defines<br>if cyw43-driver uses LwIP. The default behavior - if it’s not defined anywhere -<br>is to set it to 1 and cyw43-driver will use lwIP requiring you to provide an<br>lwipopts.h header file. You can set CYW43_LWIP_DEFAULT to change the<br>default to 0 and avoid using lwIP if CYW43_LWIP is undefined||
|**CYW43_NO_DEFAULT_TASK_STACK**<br>Disables the default static allocation of the CYW43 FreeRTOS task stack|0|
|**CYW43_PIN_WL_DYNAMIC**<br>flag to indicate if cyw43 SPI pins can be changed at runtime||
|**CYW43_PIO_CLOCK_DIV_DYNAMIC**<br>Enable runtime configuration of the clock divider for communication with the<br>wireless chip|0|
|**CYW43_PIO_CLOCK_DIV_FRAC8**<br>Fractional part of the clock divider for communication with the wireless chip 0-<br>255|0|
|**CYW43_PIO_CLOCK_DIV_INT**<br>Integer part of the clock divider for communication with the wireless chip|2|
|**CYW43_TASK_PRIORITY**<br>Priority for the CYW43 FreeRTOS task|tskIDLE_PRIORITY + 4|
|**CYW43_TASK_STACK_SIZE**<br>Stack size for the CYW43 FreeRTOS task in 4-byte words|1024|



6.1. Full List of SDK Configuration Defines

**550**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**GPIO_IRQ_CALLBACK_ORDER_PRIORITY**<br>IRQ priority order of the default IRQ callback|PICO_SHARED_IRQ_HANDLER_LOWE<br>ST_ORDER_PRIORITY|
|**GPIO_RAW_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY**<br>IRQ priority order of raw IRQ handlers if the priority is not specified|PICO_SHARED_IRQ_HANDLER_DEFAU<br>LT_ORDER_PRIORITY|
|**PARAM_ASSERTIONS_DISABLE_ALL**<br>Global assert disable|0|
|**PARAM_ASSERTIONS_ENABLED_ADDRESS_ALIAS**<br>Enable/disable assertions in memory address aliasing macros|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_ADC**<br>Enable/disable assertions in the hardware_adc module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_BOOT_LOCK**<br>Enable/disable assertions in the hardware_boot_lock module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_CLOCKS**<br>Enable/disable assertions in the hardware_clocks module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_DMA**<br>Enable/disable hardware_dma assertions|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_EXCEPTION**<br>Enable/disable assertions in the hardware_exception module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_FLASH**<br>Enable/disable assertions in the hardware_flash module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_GPIO**<br>Enable/disable assertions in the hardware_gpio module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_I2C**<br>Enable/disable assertions in the hardware_i2c module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_INTERP**<br>Enable/disable assertions in the hardware_interp module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_IRQ**<br>Enable/disable assertions in the hardware_irq module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_PIO**<br>Enable/disable assertions in the hardware_pio module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_POWMAN**<br>Enable/disable hardware_powman assertions|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_PWM**<br>Enable/disable assertions in the hardware_pwm module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_RESETS**<br>Enable/disable assertions in the hardware_resets module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_SHA256**<br>Enable/disable hardware_sha256 assertions|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_SPI**<br>Enable/disable assertions in the hardware_spi module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_SYNC**<br>Enable/disable assertions in the hardware_sync module|0|



6.1. Full List of SDK Configuration Defines

**551**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_TICKS**<br>Enable/disable assertions in the hardware_ticks module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_TIMER**<br>Enable/disable assertions in the hardware_timer module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_UART**<br>Enable/disable assertions in the hardware_uart module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_WATCHDOG**<br>Enable/disable assertions in the hardware_watchdog module|0|
|**PARAM_ASSERTIONS_ENABLED_HARDWARE_XIP_CACHE**<br>Enable/disable assertions in the hardware_xip_cache module|0|
|**PARAM_ASSERTIONS_ENABLED_LOCK_CORE**<br>Enable/disable assertions in the lock core|0|
|**PARAM_ASSERTIONS_ENABLED_PHEAP**<br>Enable/disable assertions in the pheap module|0|
|**PARAM_ASSERTIONS_ENABLED_PICO_CYW43_ARCH**<br>Enable/disable assertions in the pico_cyw43_arch module|0|
|**PARAM_ASSERTIONS_ENABLED_PICO_MULTICORE**<br>Enable/disable assertions in the pico_multicore module|0|
|**PARAM_ASSERTIONS_ENABLED_PICO_TIME**<br>Enable/disable assertions in the pico_time module|0|
|**PARAM_ASSERTIONS_ENABLED_PIO_INSTRUCTIONS**<br>Enable/disable assertions in the PIO instructions|0|
|**PARAM_ASSERTIONS_ENABLE_ALL**<br>Global assert enable|0|
|**PICO_ADC_CLKDIV_ROUND_NEAREST**<br>True if floating point ADC clock divisors should be rounded to the nearest<br>possible clock divisor rather than rounding down|PICO_CLKDIV_ROUND_NEAREST|
|**PICO_BOOTROM_LOCKING_ENABLED**<br>Enable/disable locking for bootrom functions that use shared resources. If<br>this flag is enabled bootrom lock checking is turned on and BOOT locks are<br>taken around the relevant bootrom functions|1|
|**PICO_BOOTSEL_VIA_DOUBLE_RESET_ACTIVITY_LED**<br>Optionally define a pin to use as bootloader activity LED when BOOTSEL mode<br>is entered via reset double tap||
|**PICO_BOOTSEL_VIA_DOUBLE_RESET_ACTIVITY_LED_ACTIVE_LOW**<br>Whether pin used as bootloader activity LED when BOOTSEL mode is entered<br>via reset double tap is active low. Not supported on RP2040|0|
|**PICO_BOOTSEL_VIA_DOUBLE_RESET_INTERFACE_DISABLE_MASK**<br>Optionally disable either the mass storage interface (bit 0) or the PICOBOOT<br>interface (bit 1) when entering BOOTSEL mode via double reset|0|
|**PICO_BOOTSEL_VIA_DOUBLE_RESET_TIMEOUT_MS**<br>Window of opportunity for a second press of a reset button to enter BOOTSEL<br>mode (milliseconds)|200|



6.1. Full List of SDK Configuration Defines

**552**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_BOOT_STAGE2_CHOOSE_AT25SF128A**<br>Select boot2_at25sf128a as the boot stage 2 when no boot stage 2 selection<br>is made by the CMake build. This define applies to compilation of the boot<br>stage 2 not the main application|0|
|**PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H**<br>Select boot2_generic_03h as the boot stage 2 when no boot stage 2 selection<br>is made by the CMake build. This define applies to compilation of the boot<br>stage 2 not the main application|1|
|**PICO_BOOT_STAGE2_CHOOSE_IS25LP080**<br>Select boot2_is25lp080 as the boot stage 2 when no boot stage 2 selection is<br>made by the CMake build. This define applies to compilation of the boot stage<br>2 not the main application|0|
|**PICO_BOOT_STAGE2_CHOOSE_W25Q080**<br>Select boot2_w25q080 as the boot stage 2 when no boot stage 2 selection is<br>made by the CMake build. This define applies to compilation of the boot stage<br>2 not the main application|0|
|**PICO_BOOT_STAGE2_CHOOSE_W25X10CL**<br>Select boot2_w25x10cl as the boot stage 2 when no boot stage 2 selection is<br>made by the CMake build. This define applies to compilation of the boot stage<br>2 not the main application|0|
|**PICO_BTSTACK_CYW43_MAX_HCI_PROCESS_LOOP_COUNT**<br>limit the max number of iterations of the hci processing loop||
|**PICO_BUILD_BOOT_STAGE2_NAME**<br>Name of the boot stage 2 if selected in the build system. This define applies to<br>compilation of the boot stage 2 not the main application||
|**PICO_CLKDIV_ROUND_NEAREST**<br>True if floating point clock divisors should be rounded to the nearest possible<br>clock divisor by default rather than rounding down|1|
|**PICO_CLOCK_ADJUST_PERI_CLOCK_WITH_SYS_CLOCK**<br>When the SYS clock PLL is changed keep the peripheral clock attached to it|0|
|**PICO_CLOCK_GPIO_CLKDIV_ROUND_NEAREST**<br>True if floating point GPIO clock divisors should be rounded to the nearest<br>possible clock divisor rather than rounding down|PICO_CLKDIV_ROUND_NEAREST|
|**PICO_CMSIS_RENAME_EXCEPTIONS**<br>Whether to rename SDK exceptions such as isr_nmi to their CMSIS equivalent<br>i.e. NMI_Handler|1|
|**PICO_COLORED_STATUS_LED_AVAILABLE**<br>Indicate whether a colored status LED is available|1 if PICO_DEFAULT_WS2812_PIN is<br>defined; may be set by the user to 0 to<br>not use the colored status LED even if<br>available|
|**PICO_COLORED_STATUS_LED_USES_WRGB**<br>Indicate if the colored status LED supports WRGB|0|
|**PICO_COLORED_STATUS_LED_WS2812_FREQ**<br>Frequency per bit for the WS2812 colored status LED|800000|



6.1. Full List of SDK Configuration Defines

**553**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_CONFIG_HEADER**<br>Unquoted path to header include in place of the default pico/config.h which<br>may be desirable for build systems which can’t easily generate the<br>config_autogen header||
|**PICO_CONFIG_RTOS_ADAPTER_HEADER**<br>Unquoted path to header include in the default pico/config.h for RTOS<br>integration defines that must be included in all sources||
|**PICO_CORE1_STACK_SIZE**<br>Minimum amount of stack space reserved in the linker script for core 1 - if<br>zero then no space is reserved and the user must provide their own stack|PICO_STACK_SIZE (0x800)|
|**PICO_CRT0_ALLOCATE_SPACERS**<br>Set spacer sections as allocatable. This makes them appear in print-memory-<br>usage but is incompatible with linker scripts that do not KEEP the sections|1|
|**PICO_CRT0_NEAR_CALLS**<br>Whether calls from crt0 into the binary are near (<16M away) - ignored for<br>PICO_COPY_TO_RAM|0|
|**PICO_CRT0_NO_RESET_SECTION**<br>Omit .reset section contents containing the startup code. This must be set if<br>you want to replace the startup code while still keeping the rest of pico_crt0 as<br>the reset section define here is not garbage collected|0|
|**PICO_CYW43_ARCH_DEBUG_ENABLED**<br>Enable/disable some debugging output in the pico_cyw43_arch module|1 in debug builds|
|**PICO_CYW43_ARCH_DEFAULT_COUNTRY_CODE**<br>Default country code for the cyw43 wireless driver|CYW43_COUNTRY_WORLDWIDE|
|**PICO_CYW43_LOGGING_ENABLED**<br>Enable/disable CYW43_PRINTF used for logging in cyw43 components. Has<br>no effect if CYW43_PRINTF is defined by the user|1|
|**PICO_DEBUG_MALLOC**<br>Enable/disable debug printf from malloc|0|
|**PICO_DEBUG_MALLOC_LOW_WATER**<br>Define the lower bound for allocation addresses to be printed by<br>PICO_DEBUG_MALLOC|0|
|**PICO_DEBUG_PIN_BASE**<br>First pin to use for debug output (if enabled)|19|
|**PICO_DEBUG_PIN_COUNT**<br>Number of pins to use for debug output (if enabled)|3|
|**PICO_DEFAULT_COLORED_STATUS_LED_ON_COLOR**<br>the default pixel color value of the colored status LED when it is on||
|**PICO_DEFAULT_I2C**<br>Define the default I2C for a board|Usually provided via board header|
|**PICO_DEFAULT_I2C_SCL_PIN**<br>Define the default I2C SCL pin|Usually provided via board header|
|**PICO_DEFAULT_I2C_SDA_PIN**<br>Define the default I2C SDA pin|Usually provided via board header|



6.1. Full List of SDK Configuration Defines

**554**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_DEFAULT_IRQ_PRIORITY**<br>Define the default IRQ priority|0x80|
|**PICO_DEFAULT_LED_PIN**<br>Optionally define a pin that drives a regular LED on the board|Usually provided via board header|
|**PICO_DEFAULT_LED_PIN_INVERTED**<br>1 if LED is inverted or 0 if not|0|
|**PICO_DEFAULT_SPI**<br>Define the default SPI for a board|Usually provided via board header|
|**PICO_DEFAULT_SPI_CSN_PIN**<br>Define the default SPI CSN pin|Usually provided via board header|
|**PICO_DEFAULT_SPI_RX_PIN**<br>Define the default SPI RX pin|Usually provided via board header|
|**PICO_DEFAULT_SPI_SCK_PIN**<br>Define the default SPI SCK pin|Usually provided via board header|
|**PICO_DEFAULT_SPI_TX_PIN**<br>Define the default SPI TX pin|Usually provided via board header|
|**PICO_DEFAULT_TIMER**<br>Timer instance number to use for RP2040-period hardware_timer APIs that<br>assumed a single timer instance|0|
|**PICO_DEFAULT_UART**<br>Define the default UART used for printf etc|Usually provided via board header|
|**PICO_DEFAULT_UART_BAUD_RATE**<br>Define the default UART baudrate|115200|
|**PICO_DEFAULT_UART_RX_PIN**<br>Define the default UART RX pin|Usually provided via board header|
|**PICO_DEFAULT_UART_TX_PIN**<br>Define the default UART TX pin|Usually provided via board header|
|**PICO_DEFAULT_WS2812_PIN**<br>Optionally define a pin that controls data to a WS2812 compatible LED on the<br>board||
|**PICO_DEFAULT_WS2812_POWER_PIN**<br>Optionally define a pin that controls power to a WS2812 compatible LED on<br>the board||
|**PICO_DISABLE_SHARED_IRQ_HANDLERS**<br>Disable shared IRQ handlers|0|
|**PICO_DIVIDER_CALL_IDIV0**<br>Whether 32 bit division by zero should call __aeabi_idiv0|1|
|**PICO_DIVIDER_CALL_LDIV0**<br>Whether 64 bit division by zero should call __aeabi_ldiv0|1|
|**PICO_DIVIDER_DISABLE_INTERRUPTS**<br>Disable interrupts around division such that divider state need not be<br>saved/restored in exception handlers|0|
|**PICO_DIVIDER_IN_RAM**<br>Whether divider functions should be placed in RAM|0|



6.1. Full List of SDK Configuration Defines

**555**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_DOUBLE_SUPPORT_ROM_V1**<br>Include double support code for RP2040 B0 when that chip revision is<br>supported|1|
|**PICO_EMBED_XIP_SETUP**<br>Embed custom XIP setup (boot2) in an RP2350 binary|0|
|**PICO_FLASH_ASSERT_ON_UNSAFE**<br>Assert in debug mode rather than returning an error if flash_safe_execute<br>cannot guarantee safety to catch bugs early|1|
|**PICO_FLASH_ASSUME_CORE0_SAFE**<br>Assume that core 0 will never be accessing flash and so doesn’t need to be<br>considered during flash_safe_execute|0|
|**PICO_FLASH_ASSUME_CORE1_SAFE**<br>Assume that core 1 will never be accessing flash and so doesn’t need to be<br>considered during flash_safe_execute|0|
|**PICO_FLASH_BANK_STORAGE_OFFSET**<br>Offset in flash of the Bluetooth flash storage|PICO_FLASH_SIZE_BYTES -<br>FLASH_SECTOR_SIZE -<br>PICO_FLASH_BANK_TOTAL_SIZE on<br>RP2350 otherwise<br>PICO_FLASH_SIZE_BYTES -<br>PICO_FLASH_BANK_TOTAL_SIZE|
|**PICO_FLASH_BANK_TOTAL_SIZE**<br>Total size of the Bluetooth flash storage. Must be an even multiple of<br>FLASH_SECTOR_SIZE|FLASH_SECTOR_SIZE * 2|
|**PICO_FLASH_SAFE_EXECUTE_PICO_SUPPORT_MULTICORE_LOCKOUT**<br>Support using multicore_lockout functions to make the other core safe during<br>flash_safe_execute|1 when using pico_multicore|
|**PICO_FLASH_SAFE_EXECUTE_SUPPORT_FREERTOS_SMP**<br>Support using FreeRTOS SMP to make the other core safe during<br>flash_safe_execute|1 when using FreeRTOS SMP|
|**PICO_FLASH_SIZE_BYTES**<br>size of primary flash in bytes|Usually provided via board header|
|**PICO_FLASH_SPI_CLKDIV**<br>Clock divider from clk_sys to use for serial flash communications in boot<br>stage 2. On RP2040 this must be a multiple of 2. This define applies to<br>compilation of the boot stage 2 not the main application|varies; often specified in board header|
|**PICO_FLASH_SPI_RXDELAY**<br>Receive delay in 1/2 clock cycles to use for serial flash communications in<br>boot stage 2. This define applies to compilation of the boot stage 2 not the<br>main application|varies; often specified in board header|
|**PICO_FLOAT_IN_RAM**<br>Force placement of SDK provided single-precision floating point into RAM|0|
|**PICO_FLOAT_SUPPORT_ROM_V1**<br>Include float support code for RP2040 B0 when that chip revision is supported|1|
|**PICO_HEAP_SIZE**<br>Minimum amount of heap space reserved by the linker script|0x800|



6.1. Full List of SDK Configuration Defines

**556**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_INCLUDE_RTC_DATETIME**<br>Whether to include the datetime_t type used with the RP2040 RTC hardware|1 on RP2040|
|**PICO_MALLOC_PANIC**<br>Enable/disable panic when an allocation failure occurs|1|
|**PICO_MAX_SHARED_IRQ_HANDLERS**<br>Maximum number of shared IRQ handlers|4|
|**PICO_MBEDTLS_SHA256_ALT_USE_DMA**<br>Whether to use DMA for writing to hardware for the mbedtls SHA-256<br>hardware acceleration|1|
|**PICO_MINIMAL_STORED_VECTOR_TABLE**<br>Only store a very minimal vector table in the binary on Arm|0|
|**PICO_NO_BINARY_INFO**<br>Don’t include "binary info" in the output binary|0 except forPICO_PLATFORM host|
|**PICO_NO_FPGA_CHECK**<br>Remove the FPGA platform check for small code size reduction|1|
|**PICO_NO_RAM_VECTOR_TABLE**<br>Enable/disable the RAM vector table|0|
|**PICO_NO_SIM_CHECK**<br>Remove the SIM platform check for small code size reduction|1|
|**PICO_NUM_VTABLE_IRQS**<br>Number of IRQ handlers in the vector table - can be lowered to save space if<br>you aren’t using some higher IRQs|NUM_IRQS|
|**PICO_OPAQUE_ABSOLUTE_TIME_T**<br>Enable opaque type for absolute_time_t to help catch inadvertent confusing<br>uint64_t delays with absolute times|0|
|**PICO_PANIC_FUNCTION**<br>Name of a function to use in place of the stock panic function or empty string<br>to simply breakpoint on panic||
|**PICO_PHEAP_MAX_ENTRIES**<br>Maximum number of entries in the pheap|255|
|**PICO_PIO_CLKDIV_ROUND_NEAREST**<br>True if floating point PIO clock divisors should be rounded to the nearest<br>possible clock divisor rather than rounding down|PICO_CLKDIV_ROUND_NEAREST|
|**PICO_PIO_USE_GPIO_BASE**<br>Enable code for handling more than 32 PIO pins|true when supported and when the<br>device has more than 32 pins|
|**PICO_PIO_VERSION**<br>PIO hardware version|0 on RP2040 and 1 on RP2350|
|**PICO_PRINTF_ALWAYS_INCLUDED**<br>Whether to always include printf code even if only called weakly (by panic)|1 in debug build 0 otherwise|
|**PICO_PRINTF_DEFAULT_FLOAT_PRECISION**<br>Define default floating point precision|6|
|**PICO_PRINTF_FTOA_BUFFER_SIZE**<br>Define printf ftoa buffer size|32|



6.1. Full List of SDK Configuration Defines

**557**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_PRINTF_MAX_FLOAT**<br>Define the largest float suitable to print with %f|1e9|
|**PICO_PRINTF_NTOA_BUFFER_SIZE**<br>Define printf ntoa buffer size|32|
|**PICO_PRINTF_SUPPORT_EXPONENTIAL**<br>Enable exponential floating point printing|1|
|**PICO_PRINTF_SUPPORT_FLOAT**<br>Enable floating point printing|1|
|**PICO_PRINTF_SUPPORT_LONG_LONG**<br>Enable support for long long types (%llu or %p)|1|
|**PICO_PRINTF_SUPPORT_PTRDIFF_T**<br>Enable support for the ptrdiff_t type (%t)|1|
|**PICO_PWM_CLKDIV_ROUND_NEAREST**<br>True if floating point PWM clock divisors should be rounded to the nearest<br>possible clock divisor rather than rounding down|PICO_CLKDIV_ROUND_NEAREST|
|**PICO_QUEUE_MAX_LEVEL**<br>Maintain a field for the highest level that has been reached by a queue|0|
|**PICO_RAND_BUS_PERF_COUNTER_EVENT**<br>Bus performance counter event to use for sourcing entropy|arbiter_sram5_perf_event_access|
|**PICO_RAND_BUS_PERF_COUNTER_INDEX**<br>Bus performance counter index to use for sourcing entropy|Undefined meaning pick one that is<br>not counting any valid event already|
|**PICO_RAND_ENTROPY_SRC_BUS_PERF_COUNTER**<br>Enable/disable use of a bus performance counter as an entropy source|1 if no hardware TRNG|
|**PICO_RAND_ENTROPY_SRC_ROSC**<br>Enable/disable use of ROSC as an entropy source|1 if no hardware TRNG|
|**PICO_RAND_ENTROPY_SRC_TIME**<br>Enable/disable use of hardware timestamp as an entropy source|1|
|**PICO_RAND_ENTROPY_SRC_TRNG**<br>Enable/disable use of hardware TRNG as an entropy source|1 if hardware TRNG is available|
|**PICO_RAND_MIN_ROSC_BIT_SAMPLE_TIME_US**<br>Define a default minimum time between sampling the ROSC random bit|10|
|**PICO_RAND_RAM_HASH_END**<br>End of address in RAM (non-inclusive) to hash during pico_rand seed<br>initialization|SRAM_END|
|**PICO_RAND_RAM_HASH_START**<br>Start of address in RAM (inclusive) to hash during pico_rand seed initialization|PICO_RAND_RAM_HASH_END - 1024|
|**PICO_RAND_ROSC_BIT_SAMPLE_COUNT**<br>Number of samples to take of the ROSC random bit per random number<br>generation|1|
|**PICO_RAND_SEED_ENTROPY_SRC_BOARD_ID**<br>Enable/disable use of board id as part of the random seed|not<br>PICO_RAND_SEED_ENTROPY_SRC_B<br>OOT_RANDOM|



6.1. Full List of SDK Configuration Defines

**558**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_RAND_SEED_ENTROPY_SRC_BOOT_RANDOM**<br>Enable/disable use of the per boot random number as an entropy source for<br>the random seed|0 on RP2040 which has none|
|**PICO_RAND_SEED_ENTROPY_SRC_BUS_PERF_COUNTER**<br>Enable/disable use of a bus performance counter as an entropy source for the<br>random seed|PICO_RAND_ENTROPY_SRC_BUS_PE<br>RF_COUNTER|
|**PICO_RAND_SEED_ENTROPY_SRC_RAM_HASH**<br>Enable/disable use of a RAM hash as an entropy source for the random seed|1 if no hardware TRNG|
|**PICO_RAND_SEED_ENTROPY_SRC_ROSC**<br>Enable/disable use of ROSC as an entropy source for the random seed|PICO_RAND_ENTROPY_SRC_ROSC|
|**PICO_RAND_SEED_ENTROPY_SRC_TIME**<br>Enable/disable use of hardware timestamp as an entropy source for the<br>random seed|PICO_RAND_ENTROPY_SRC_TIME|
|**PICO_RAND_SEED_ENTROPY_SRC_TRNG**<br>Enable/disable use of hardware TRNG as an entropy source for the random<br>seed|PICO_RAND_ENTROPY_SRC_TRNG|
|**PICO_RP2040_B0_SUPPORTED**<br>Whether to include any specific software support for RP2040 B0 revision|1|
|**PICO_RP2040_B1_SUPPORTED**<br>Whether to include any specific software support for RP2040 B1 revision|1|
|**PICO_RP2040_B2_SUPPORTED**<br>Whether to include any specific software support for RP2040 B2 revision|1|
|**PICO_RP2350A**<br>Whether the current board has an RP2350 in an A (30 GPIO) package - set to 0<br>for RP2350 in a B (48 GPIO) package|Usually provided via board header|
|**PICO_RP2350_A2_SUPPORTED**<br>Whether to include any specific software support for RP2350 A2 revision|1|
|**PICO_RUNTIME_NO_INIT_BOOTROM_RESET**<br>Do not include SDK implementation ofruntime_init_bootrom_resetfunction|1 on RP2040|
|**PICO_RUNTIME_NO_INIT_CLOCKS**<br>Do not include SDK implementation ofruntime_init_clocksfunction|0|
|**PICO_RUNTIME_NO_INIT_DEFAULT_ALARM_POOL**<br>Do not include SDK implementation ofruntime_init_default_alarm_poolfunction|1 if<br>PICO_TIME_DEFAULT_ALARM_POOL_DISABLED<br>is|
|**PICO_RUNTIME_NO_INIT_EARLY_RESETS**<br>Do not include SDK implementation ofruntime_init_early_resetsfunction|1 on RP2040|
|**PICO_RUNTIME_NO_INIT_INSTALL_RAM_VECTOR_TABLE**<br>Do not include SDK implementation ofruntime_init_install_ram_vector_table<br>function|0 unless RISC-V or RAM binary|
|**PICO_RUNTIME_NO_INIT_MUTEX**<br>Do not include SDK implementation ofruntime_init_mutexfunction|0|
|**PICO_RUNTIME_NO_INIT_PER_CORE_BOOTROM_RESET**<br>Do not include SDK implementation ofruntime_init_per_core_bootrom_reset<br>function|1 on RP2040|



6.1. Full List of SDK Configuration Defines

**559**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_RUNTIME_NO_INIT_PER_CORE_ENABLE_COPROCESSORS**<br>Do not include SDK implementation of<br>runtime_init_per_core_enable_coprocessorsfunction|1 on RP2040 or RISC-V|
|**PICO_RUNTIME_NO_INIT_PER_CORE_INSTALL_STACK_GUARD**<br>Do not include SDK implementation of<br>runtime_init_per_core_install_stack_guardfunction|1 unlessPICO_USE_STACK_GUARDSis 1|
|**PICO_RUNTIME_NO_INIT_RP2040_GPIO_IE_DISABLE**<br>Do not include SDK implementation ofruntime_init_rp2040_gpio_ie_disable<br>function|0 on RP2040|
|**PICO_RUNTIME_NO_INIT_SPIN_LOCKS_RESET**<br>Do not include SDK implementation ofruntime_init_spin_locks_resetfunction|0|
|**PICO_RUNTIME_NO_INIT_USB_POWER_DOWN**<br>Do not include SDK implementation ofruntime_init_usb_power_downfunction|0|
|**PICO_RUNTIME_SKIP_INIT_BOOTROM_LOCKING_ENABLE**<br>Skip calling ofruntime_init_bootrom_locking_enablefunction during runtime init|0|
|**PICO_RUNTIME_SKIP_INIT_BOOTROM_RESET**<br>Skip calling ofruntime_init_bootrom_resetfunction during runtime init|1 on RP2040|
|**PICO_RUNTIME_SKIP_INIT_BOOT_LOCKS_RESET**<br>Skip calling ofruntime_init_boot_locks_resetfunction during runtime init|0|
|**PICO_RUNTIME_SKIP_INIT_CLOCKS**<br>Skip calling ofruntime_init_clocksfunction during runtime init|0|
|**PICO_RUNTIME_SKIP_INIT_DEFAULT_ALARM_POOL**<br>Skip calling ofruntime_init_default_alarm_poolfunction during runtime init|1 if<br>PICO_TIME_DEFAULT_ALARM_POOL_DISABLED<br>is 1|
|**PICO_RUNTIME_SKIP_INIT_EARLY_RESETS**<br>Skip calling ofruntime_init_early_resetsfunction during runtime init|1 on RP2040|
|**PICO_RUNTIME_SKIP_INIT_INSTALL_RAM_VECTOR_TABLE**<br>Skip calling ofruntime_init_install_ram_vector_tablefunction during runtime<br>init|0 unless RISC-V or RAM binary|
|**PICO_RUNTIME_SKIP_INIT_MUTEX**<br>Skip calling ofruntime_init_mutexfunction during runtime init|0|
|**PICO_RUNTIME_SKIP_INIT_PER_CORE_BOOTROM_RESET**<br>Skip calling ofruntime_init_per_core_bootrom_resetfunction during per-core init|1 on RP2040|
|**PICO_RUNTIME_SKIP_INIT_PER_CORE_ENABLE_COPROCESSORS**<br>Skip calling ofruntime_init_per_core_enable_coprocessorsfunction during per-<br>core init|1 on RP2040 or RISC-V|
|**PICO_RUNTIME_SKIP_INIT_PER_CORE_H3_IRQ_REGISTERS**<br>Skip calling ofruntime_init_per_core_h3_irq_registersfunction during per-core<br>init|1 on non RISC-V|
|**PICO_RUNTIME_SKIP_INIT_PER_CORE_INSTALL_STACK_GUARD**<br>Skip calling ofruntime_init_per_core_install_stack_guardfunction during<br>runtime init|1 unlessPICO_USE_STACK_GUARDSis 1|
|**PICO_RUNTIME_SKIP_INIT_RP2040_GPIO_IE_DISABLE**<br>Skip calling ofruntime_init_rp2040_gpio_ie_disablefunction during runtime init|0 on RP2040|



6.1. Full List of SDK Configuration Defines

**560**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_RUNTIME_SKIP_INIT_SPIN_LOCKS_RESET**<br>Skip calling ofruntime_init_spin_locks_resetfunction during runtime init|0|
|**PICO_RUNTIME_SKIP_INIT_USB_POWER_DOWN**<br>Skip calling ofruntime_init_usb_power_downfunction during runtime init|0|
|**PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY**<br>Set default shared IRQ order priority|0x80|
|**PICO_SPINLOCK_ID_ATOMIC**<br>Spinlock ID for atomics|13|
|**PICO_SPINLOCK_ID_CLAIM_FREE_FIRST**<br>Lowest Spinlock ID in the 'claim free' range|24|
|**PICO_SPINLOCK_ID_CLAIM_FREE_LAST**<br>Highest Spinlock ID in the 'claim free' range|31|
|**PICO_SPINLOCK_ID_HARDWARE_CLAIM**<br>Spinlock ID for Hardware claim protection|11|
|**PICO_SPINLOCK_ID_IRQ**<br>Spinlock ID for IRQ protection|9|
|**PICO_SPINLOCK_ID_OS1**<br>First Spinlock ID reserved for use by low level OS style software|14|
|**PICO_SPINLOCK_ID_OS2**<br>Second Spinlock ID reserved for use by low level OS style software|15|
|**PICO_SPINLOCK_ID_RAND**<br>Spinlock ID for Random Number Generator|12|
|**PICO_SPINLOCK_ID_STRIPED_FIRST**<br>Lowest Spinlock ID in the 'striped' range|16|
|**PICO_SPINLOCK_ID_STRIPED_LAST**<br>Highest Spinlock ID in the 'striped' range|23|
|**PICO_SPINLOCK_ID_TIMER**<br>Spinlock ID for Timer protection|10|
|**PICO_STACK_SIZE**<br>Minimum amount of stack space reserved in the linker script for each core.<br>See also PICO_CORE1_STACK_SIZE|0x800|
|**PICO_STATUS_LED_AVAILABLE**<br>Indicate whether a single-color status LED is available|1 if PICO_DEFAULT_LED_PIN or<br>CYW43_WL_GPIO_LED_PIN is defined;<br>may be set by the user to 0 to not use<br>either even if they are available|
|**PICO_STATUS_LED_VIA_COLORED_STATUS_LED**<br>Indicate if the colored status LED should be used for both status_led and<br>colored_status_led APIs|1 if<br>PICO_COLORED_STATUS_LED_AVAIL<br>ABLE is 1 and<br>PICO_STATUS_LED_AVAILABLE is 0|
|**PICO_STDIO_DEADLOCK_TIMEOUT_MS**<br>Time after which to assume stdio_usb is deadlocked by use in IRQ and give up|1000|
|**PICO_STDIO_DEFAULT_CRLF**<br>Default for CR/LF conversion enabled on all stdio outputs|1|



6.1. Full List of SDK Configuration Defines

**561**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_STDIO_ENABLE_CRLF_SUPPORT**<br>Enable/disable CR/LF output conversion support|1|
|**PICO_STDIO_RTT_DEFAULT_CRLF**<br>Default state of CR/LF translation for rtt output|PICO_STDIO_DEFAULT_CRLF|
|**PICO_STDIO_SEMIHOSTING_DEFAULT_CRLF**<br>Default state of CR/LF translation for semihosting output|PICO_STDIO_DEFAULT_CRLF|
|**PICO_STDIO_SHORT_CIRCUIT_CLIB_FUNCS**<br>Directly replace common stdio functions such as putchar from the C-library to<br>avoid pulling in lots of c library code for simple output|1|
|**PICO_STDIO_STACK_BUFFER_SIZE**<br>Define printf buffer size (on stack)… this is just a working buffer not a max<br>output size|128|
|**PICO_STDIO_UART_DEFAULT_CRLF**<br>Default state of CR/LF translation for UART output|PICO_STDIO_DEFAULT_CRLF|
|**PICO_STDIO_UART_SUPPORT_CHARS_AVAILABLE_CALLBACK**<br>Enable UART STDIO support for stdio_set_chars_available_callback. Can be<br>disabled to make use of the uart elsewhere|1|
|**PICO_STDIO_USB_CONNECTION_WITHOUT_DTR**<br>Disable use of DTR for connection checking meaning connection is assumed<br>to be valid|0|
|**PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS**<br>Maximum number of milliseconds to wait during initialization for a CDC<br>connection from the host (negative means indefinite) during initialization|0|
|**PICO_STDIO_USB_DEFAULT_CRLF**<br>Default state of CR/LF translation for USB output|PICO_STDIO_DEFAULT_CRLF|
|**PICO_STDIO_USB_DEINIT_DELAY_MS**<br>Number of milliseconds to wait before deinitializing stdio_usb|110|
|**PICO_STDIO_USB_DEVICE_SELF_POWERED**<br>Set USB device as self powered device|0|
|**PICO_STDIO_USB_ENABLE_IRQ_BACKGROUND_TASK**<br>Enable/disable the use of a background task to call tud_task()|1 if the application is not using<br>tinyUSB directly|
|**PICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE**<br>Enable/disable resetting into BOOTSEL mode if the host sets the baud rate to<br>a magic value (PICO_STDIO_USB_RESET_MAGIC_BAUD_RATE)|1 if application is not using TinyUSB<br>directly|
|**PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE**<br>Enable/disable resetting into BOOTSEL mode via an additional VENDOR USB<br>interface - enables picotool based reset|1 if application is not using TinyUSB<br>directly|
|**PICO_STDIO_USB_ENABLE_TINYUSB_INIT**<br>Enable/disable calling tinyUSB tusb_init() during initialization|1 if the application is not using<br>tinyUSB directly|
|**PICO_STDIO_USB_LOW_PRIORITY_IRQ**<br>Explicit User IRQ number to claim for tud_task() background execution instead<br>of letting the implementation pick a free one dynamically (deprecated)||



6.1. Full List of SDK Configuration Defines

**562**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_STDIO_USB_POST_CONNECT_WAIT_DELAY_MS**<br>Number of extra milliseconds to wait when using<br>PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS after a host CDC<br>connection is detected (some host terminals seem to sometimes lose<br>transmissions sent right after connection)|50|
|**PICO_STDIO_USB_RESET_BOOTSEL_ACTIVITY_LED**<br>Optionally define a pin to use as bootloader activity LED when BOOTSEL mode<br>is entered via USB (either VIA_BAUD_RATE or VIA_VENDOR_INTERFACE)||
|**PICO_STDIO_USB_RESET_BOOTSEL_ACTIVITY_LED_ACTIVE_LOW**<br>Whether pin to use as bootloader activity LED when BOOTSEL mode is entered<br>via USB (either VIA_BAUD_RATE or VIA_VENDOR_INTERFACE) is active low|0|
|**PICO_STDIO_USB_RESET_BOOTSEL_FIXED_ACTIVITY_LED**<br>Whether the pin specified by<br>PICO_STDIO_USB_RESET_BOOTSEL_ACTIVITY_LED is fixed or can be<br>modified by picotool over the VENDOR USB interface|0|
|**PICO_STDIO_USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK**<br>Optionally disable either the mass storage interface (bit 0) or the PICOBOOT<br>interface (bit 1) when entering BOOTSEL mode via USB (either<br>VIA_BAUD_RATE or VIA_VENDOR_INTERFACE)|0|
|**PICO_STDIO_USB_RESET_INTERFACE_SUPPORT_MS_OS_20_DESCRIPTOR**<br>If vendor reset interface is included add support for Microsoft OS 2.0<br>Descriptor|1|
|**PICO_STDIO_USB_RESET_INTERFACE_SUPPORT_RESET_TO_BOOTSEL**<br>If vendor reset interface is included allow rebooting to BOOTSEL mode|1|
|**PICO_STDIO_USB_RESET_INTERFACE_SUPPORT_RESET_TO_FLASH_BOOT**<br>If vendor reset interface is included allow rebooting with regular flash boot|1|
|**PICO_STDIO_USB_RESET_MAGIC_BAUD_RATE**<br>Baud rate that if selected causes a reset into BOOTSEL mode (if<br>PICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE is set)|1200|
|**PICO_STDIO_USB_RESET_RESET_TO_FLASH_DELAY_MS**<br>Delay in ms before rebooting via regular flash boot|100|
|**PICO_STDIO_USB_STDOUT_TIMEOUT_US**<br>Number of microseconds to be blocked trying to write USB output before<br>assuming the host has disappeared and discarding data|500000|
|**PICO_STDIO_USB_SUPPORT_CHARS_AVAILABLE_CALLBACK**<br>Enable USB STDIO support for stdio_set_chars_available_callback. Can be<br>disabled to make use of USB CDC RX callback elsewhere|1|
|**PICO_STDIO_USB_TASK_INTERVAL_US**<br>Period of microseconds between calling tud_task in the background|1000|
|**PICO_STDIO_USB_USE_DEFAULT_DESCRIPTORS**<br>Whetherpico_stdio_usbprovides the USB descriptors needed for USB<br>communication|1 if the application is not using<br>tinyUSB directly|
|**PICO_STDOUT_MUTEX**<br>Enable/disable mutex around stdout|1|
|**PICO_TIME_DEFAULT_ALARM_POOL_DISABLED**<br>Disable the default alarm pool|0|



6.1. Full List of SDK Configuration Defines

**563**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**PICO_TIME_DEFAULT_ALARM_POOL_HARDWARE_ALARM_NUM**<br>Select which HW alarm is used for the default alarm pool|3|
|**PICO_TIME_DEFAULT_ALARM_POOL_MAX_TIMERS**<br>Selects the maximum number of concurrent timers in the default alarm pool|16|
|**PICO_TIME_SLEEP_OVERHEAD_ADJUST_US**<br>How many microseconds to wake up early (and then busy_wait) to account for<br>timer overhead when sleeping in low power mode|6|
|**PICO_UART_DEFAULT_CRLF**<br>Enable/disable CR/LF translation on UART|0|
|**PICO_UART_ENABLE_CRLF_SUPPORT**<br>Enable/disable CR/LF translation support|1|
|**PICO_USE_FASTEST_SUPPORTED_CLOCK**<br>Use the fastest officially supported clock by default|0|
|**PICO_USE_GPIO_COPROCESSOR**<br>Enable/disable use of the GPIO coprocessor for GPIO access|1|
|**PICO_USE_MALLOC_MUTEX**<br>Whether to protect malloc etc with a mutex|1 with pico_multicore, 0 otherwise|
|**PICO_USE_STACK_GUARDS**<br>Enable/disable stack guards|0|
|**PICO_USE_SW_SPIN_LOCKS**<br>Use software implementation for spin locks|1 on RP2350 due to errata|
|**PICO_VTABLE_PER_CORE**<br>User is using separate vector tables per core|0|
|**PICO_XOSC_STARTUP_DELAY_MULTIPLIER**<br>Multiplier (from 1ms) for xosc startup delay to accommodate slow-starting<br>oscillators|6|
|**PLL_SYS_POSTDIV1**<br>System clock PLL post divider 1 setting|6 on RP2040 or 5 on RP2350|
|**PLL_SYS_POSTDIV2**<br>System clock PLL post divider 2 setting|2|
|**PLL_SYS_REFDIV**<br>PLL reference divider setting for PLL_SYS|1|
|**PLL_SYS_VCO_FREQ_HZ**<br>System clock PLL frequency|(1500 * MHZ)|
|**PLL_USB_POSTDIV1**<br>USB clock PLL post divider 1 setting|5|
|**PLL_USB_POSTDIV2**<br>USB clock PLL post divider 2 setting|5|
|**PLL_USB_REFDIV**<br>PLL reference divider setting for PLL_USB|1|
|**PLL_USB_VCO_FREQ_HZ**<br>USB clock PLL frequency|(1200 * MHZ)|
|**SYS_CLK_HZ**<br>System operating frequency in Hz|125000000|



6.1. Full List of SDK Configuration Defines

**564**

Raspberry Pi Pico-series C/C++ SDK

|**Name / Description**|**Default**|
|---|---|
|**SYS_CLK_HZ**<br>System operating frequency in Hz|150000000|
|**SYS_CLK_VREG_VOLTAGE_AUTO_ADJUST**<br>Should the regulator voltage be adjusted above<br>SYS_CLK_VREG_VOLTAGE_MIN when initializing the clocks|0|
|**SYS_CLK_VREG_VOLTAGE_AUTO_ADJUST_DELAY_US**<br>Number of microseconds to wait after updating regulator voltage due to<br>SYS_CLK_VREG_VOLTAGE_MIN to allow voltage to settle|1000|
|**SYS_CLK_VREG_VOLTAGE_MIN**<br>minimum voltage (see VREG_VOLTAGE_x_xx) for the voltage regulator to be<br>ensured during clock initialization if<br>SYS_CLK_VREG_VOLTAGE_AUTO_ADJUST is 1||
|**USB_CLK_HZ**<br>USB clock frequency. Must be 48MHz for the USB interface to operate<br>correctly|48000000|
|**USB_DPRAM_MAX**<br>Set amount of USB RAM used by USB system|4096|
|**XOSC_HZ**<br>Crystal oscillator frequency in Hz|12000000|



6.1. Full List of SDK Configuration Defines

**565**
