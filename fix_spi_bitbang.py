import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    content = f.read()

# Fix 1: Bit-reverse the transmitted byte to external peripheral and the received byte back.
# PL022 sends MSB first, and our bit-banging Step() sends from (dataSize - 1) down to 0.
# However, ISPIPeripheral.Transmit(byte) expects a standard byte.
# Since we are sending MSB at bit (dataSize-1) which is usually 7, it SHOULD work if we send the whole byte.
# But let's look at how receiveData is reconstructed:
# receiveData = (ushort)((receiveData << 1) | Convert.ToUInt16(receivedBit));
# This builds MSB to LSB.
# externalReceiveData >> (7 - transmitCounter) means:
# counter=0: bit 7
# counter=1: bit 6
# ...
# This matches the MSB-first shifting.

# The issue is likely that transmitData as a ushort needs to be shifted/masked correctly for Transmit(byte).
# And we need to make sure we don't call Transmit for every bit if transmitCounter stays 0?
# Wait, transmitCounter is incremented at the end of Step().

old_block = """        if (transmitCounter == 0)
        {
          this.Log(LogLevel.Info, "SPI{0}: Starting frame with data 0x{1:X}, transmitCounter {2}", id, transmitData, transmitCounter);
          if (RegisteredPeripheral != null)
          {
            byte dataToTransmit = (byte)transmitData;
            if (dataSize < 8)
            {
               dataToTransmit &= (byte)((1 << dataSize) - 1);
            }

            externalReceiveData = RegisteredPeripheral.Transmit(dataToTransmit);

            this.Log(LogLevel.Info, "SPI{0}: Transmitted 0x{1:X} to external, got 0x{2:X}", id, dataToTransmit, externalReceiveData);
          }
        }"""

new_block = """        if (transmitCounter == 0)
        {
          if (RegisteredPeripheral != null)
          {
            byte dataToTransmit = (byte)transmitData;
            // The controller sends MSB first. If dataSize is 8, bit 7 is sent first.
            // ISPIPeripheral.Transmit(byte) expects bit 7 to be the first bit.
            // So dataToTransmit is already correct.

            externalReceiveData = RegisteredPeripheral.Transmit(dataToTransmit);
            this.Log(LogLevel.Info, "SPI{0}: Transmitted 0x{1:X} to external, got 0x{2:X}", id, dataToTransmit, externalReceiveData);
          }
        }"""

content = content.replace(old_block, new_block)

# Fix 2: Ensure bit reconstruction matches.
# receiveData = (ushort)((receiveData << 1) | Convert.ToUInt16(receivedBit));
# This means bit received at counter=0 becomes bit 7 of the final byte.
# So receivedBit should be bit 7 of externalReceiveData when counter=0.
# Formula (7 - transmitCounter) is correct.
# Wait, I already changed it to (7 - transmitCounter) using sed.

with open(filepath, 'w') as f:
    f.write(content)
