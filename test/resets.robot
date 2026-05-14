*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Setup      Reset Emulation
Resource        ${RENODEKEYWORDS}

*** Variables ***
${UART}         sysbus.uart0
${RESC}         ${CURDIR}/renode-config/run_xiao.resc

*** Test Cases ***
Verify RESETS Peripheral
    [Documentation]     Verifies that the RESETS peripheral correctly reflects RESET and RESET_DONE states.
    [Tags]              resets  rp2040

    Execute Command     $global.TEST_FILE = @${FIRMWARE}
    Execute Command     include @${RESC}
    Create Terminal Tester    ${UART}

    # Wait for UART to be ready
    Wait For Line On Uart     UART Bidirectional Communication Ready    timeout=10

    # Send 'r' command to test RESETS
    Write Char On Uart        r

    # Verify output
    Wait For Line On Uart     RESETS Test: INIT_R=0x[0-9A-F]{8} INIT_D=0x[0-9A-F]{8} CLR_R=0x00000000 CLR_D=0x01FFFFFF SET_R=0x01FFFFFF SET_D=0x00000000    timeout=5  treatAsRegex=true
