*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Variables ***
${UART}                       sysbus.uart0
${ADC}                        sysbus.adc
${FIRMWARE}                   ${CURDIR}/../.pio/build/seeed-xiao-rp2040/firmware.elf
${RESC}                       ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Should Detect ADC Conversion Error
    [Documentation]           Verifies that changing AINSEL during sampling sets the ERR and ERR_STICKY bits.
    [Timeout]                 1200 seconds
    Create Machine
    Start Emulation
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Send 'X' to trigger error detection test in firmware
    Write Char On Uart        X
    Wait For Line On Uart     ADC Error Test: ERR=1 ERR_STICKY=1

Should Pack Error Bit In FIFO
    [Documentation]           Verifies that bit 15 is set in FIFO data when a conversion error occurred and FIFO error reporting is enabled.
    [Timeout]                 1200 seconds
    Create Machine
    Start Emulation
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Send 'Y' to trigger FIFO error test in firmware
    Write Char On Uart        Y
    # VAL=0x8... (bit 15 set)
    Wait For Line On Uart     ADC FIFO Error Test: VAL=0x8

Should Maintain READY Flag During Delay
    [Documentation]           Verifies that the READY flag is high when ADC is in Waiting or Delay state.
    [Timeout]                 1200 seconds
    Create Machine
    Start Emulation
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Set a long delay (high DIV)
    # DIV = 0x4000.00 (integral 0x4000 = 16384)
    # Total cycles between samples = 16384 + 1 = 16385
    # Sampling takes 96 cycles.
    Execute Command           ${ADC} WriteDoubleWord 0x10 0x400000

    # Start many (bit 3) and enable (bit 0)
    Execute Command           ${ADC} WriteDoubleWord 0x0 0x9

    # Wait a bit and check READY flag
    # It should be high most of the time (16385 - 96 cycles)
    Sleep                     1s
    ${cs}=                    Execute Command    ${ADC} ReadDoubleWord 0x0
    # bit 8 is READY
    ${ready_bit}=             Evaluate    (${cs} >> 8) & 1
    Should Be Equal As Integers    ${ready_bit}    1

Should Align DMARequest With Threshold
    [Documentation]           Verifies that DMARequest is asserted only when FIFO level reaches the threshold.
    [Timeout]                 1200 seconds
    Create Machine
    Start Emulation
    Wait For Line On Uart     UART Bidirectional Communication Ready

    # Set threshold to 4 (FCS.THRESH is bits 24:27)
    Execute Command           ${ADC} WriteDoubleWord 0x08 0x04000000
    # Enable FIFO (bit 0) and DREQ (bit 3)
    Execute Command           ${ADC} WriteDoubleWord 0x08 0x04000009

    # Check DMARequest is deasserted (FIFO level 0)
    ${dreq}=                  Execute Command    ${ADC} ReadDMARequest
    Should Contain            ${dreq}    False

    # Trigger 3 samples manually (START_ONCE is bit 2)
    # We need EN=1 (bit 0) too.
    Execute Command           ${ADC} WriteDoubleWord 0x0 0x5
    Execute Command           ${ADC} WriteDoubleWord 0x0 0x5
    Execute Command           ${ADC} WriteDoubleWord 0x0 0x5

    # Check DMARequest is still deasserted
    ${dreq}=                  Execute Command    ${ADC} ReadDMARequest
    Should Contain            ${dreq}    False

    # Trigger 4th sample
    Execute Command           ${ADC} WriteDoubleWord 0x0 0x5

    # Check DMARequest is now asserted
    Sleep                     1s
    ${dreq}=                  Execute Command    ${ADC} ReadDMARequest
    Should Contain            ${dreq}    True

*** Keywords ***
Create Machine
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Command           include @${RESC}
    Create Terminal Tester    ${UART}
