import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    content = f.read()

# Fix the bit selection for external peripheral
bad_line = 'receivedBit = Convert.ToBoolean((externalReceiveData >> (7 - transmitCounter)) & 1);'
good_line = 'receivedBit = Convert.ToBoolean((externalReceiveData >> (dataSize - 1 - transmitCounter)) & 1);'

content = content.replace(bad_line, good_line)

with open(filepath, 'w') as f:
    f.write(content)
