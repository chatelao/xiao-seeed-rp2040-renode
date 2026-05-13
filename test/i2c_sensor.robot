*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Setup      Reset Emulation
Resource        ${RENODEKEYWORDS}

*** Variables ***
${UART}         sysbus.uart0
${RESC}         ${CURDIR}/../examples/i2c_sensor/i2c_sensor.resc
${FIRMWARE}     ${CURDIR}/../examples/i2c_sensor/.pio/build/seeed-xiao-rp2040/firmware.elf

*** Test Cases ***
Verify I2C Sensor Example
    [Documentation]    Verifies that the BMP280 sensor ID is read correctly over I2C.
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}

    Wait For Line On Uart     I2C Sensor Example Started  timeout=30
    Wait For Line On Uart     Scanning I2C for BMP280 (0x76)...  timeout=10
    Wait For Line On Uart     Found BMP280. Chip ID: 0x58  timeout=20
    Wait For Line On Uart     I2C Sensor Success!  timeout=10
