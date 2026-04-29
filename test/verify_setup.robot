*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Library         Process
Library         String
Library         Collections
Library         OperatingSystem

*** Variables ***
${RESC}         test/xiao_rp2040.resc
${UART_LOG}     renode_uart0.log
${RENODE}       test/renode/renode

*** Keywords ***
Setup
    Log To Console    Building firmware...
    ${result}=        Run Process    pio    run
    Should Be Equal As Integers    ${result.rc}    0
    Log To Console    Firmware built successfully.

Teardown
    Terminate All Processes

*** Test Cases ***
Verify UART Output
    Log To Console    Starting Renode simulation...

    # We don't use 'start' in -e because it's already in the resc file
    ${renode_proc}=    Start Process    ${RENODE}    --disable-gui    --plain    -e    include @${RESC}    alias=renode    stdout=renode_out.log    stderr=renode_err.log

    # Wait for some time to let it run and produce output
    Log To Console    Waiting for output in ${UART_LOG}...

    # Wait until the file exists and has content, or timeout
    Wait Until Created    ${UART_LOG}    timeout=15s

    Sleep    10s

    ${output}=    Get File    ${UART_LOG}
    Log To Console    UART Output: ${output}

    ${mon_out}=   Get File    renode_out.log
    Log To Console    Monitor Output: ${mon_out}

    ${mon_err}=   Get File    renode_err.log
    Log To Console    Monitor Error: ${mon_err}

    Should Contain    ${output}    Hello, XIAO RP2040!

    Terminate Process    ${renode_proc}
