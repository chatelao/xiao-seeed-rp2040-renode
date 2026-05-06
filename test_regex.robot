*** Settings ***
Resource  ${RENODEKEYWORDS}

*** Test Cases ***
Test Wait
    Create Machine
    Start Emulation
    ${res}=  Wait For Line On Uart  UART Bidirectional Communication Ready
    Log  ${res}
    ${type}=  Evaluate  "type: " + str(type($res))
    Log  ${type}
    ${keys}=  Evaluate  list($res.keys())
    Log  ${keys}
    ${groups}=  Evaluate  list($res['groups']) if 'groups' in $res else ($res['Groups'] if 'Groups' in $res else 'NONE')
    Log  groups: ${groups}

*** Keywords ***
Create Machine
    Execute Command  $global.TEST_FILE = @/app/.pio/build/seeed-xiao-rp2040/firmware.elf
    Execute Command  include @/app/test/renode-config/run_xiao.resc
    Create Terminal Tester  sysbus.uart0
