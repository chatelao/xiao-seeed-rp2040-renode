*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Setup      Reset Emulation
Resource        ${RENODEKEYWORDS}

*** Variables ***
${UART}         sysbus.uart0
${RESC}         ${CURDIR}/../examples/dma_pacing/dma_pacing.resc
${FIRMWARE}     ${CURDIR}/../examples/dma_pacing/.pio/build/seeed-xiao-rp2040/firmware.elf

*** Test Cases ***
Verify DMA Pacing Example
    [Documentation]    Verifies that the paced DMA transfer completes successfully.
    Execute Command           $global.TEST_FILE = @${FIRMWARE}
    Execute Script            ${RESC}
    Create Terminal Tester    ${UART}

    Wait For Line On Uart     DMA Pacing Example Started  timeout=30
    Wait For Line On Uart     Starting paced transfer on channel  timeout=10
    Wait For Line On Uart     DMA Pacing Success: PACED DMA EXAMPLE  timeout=30
