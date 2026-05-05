import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    lines = f.readlines()

new_lines = []
for line in lines:
    if 'externalReceiveData = RegisteredPeripheral.Transmit((byte)transmitData);' in line:
        new_lines.append(line)
        new_lines.append('            this.Log(LogLevel.Info, "SPI{0}: Transmitted 0x{1:X} to external, got 0x{2:X}", id, (byte)transmitData, externalReceiveData);\n')
        continue
    new_lines.append(line)

with open(filepath, 'w') as f:
    f.writelines(new_lines)
