import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    lines = f.readlines()

new_lines = []
skip_until = None

for i, line in enumerate(lines):
    if skip_until is not None:
        if skip_until in line:
            skip_until = None
        continue

    if 'private byte ReverseBits(byte b)' in line:
        skip_until = '}'
        continue

    if 'private void Step()' in line:
        new_lines.append(line)
        new_lines.append('    {\n')
        new_lines.append('      if (transmitCounter >= dataSize)\n')
        new_lines.append('      {\n')
        new_lines.append('        if (transmitCounter != 16)\n')
        new_lines.append('        {\n')
        new_lines.append('          rxBuffer.Enqueue(receiveData);\n')
        new_lines.append('          this.Log(LogLevel.Noisy, "SPI{0}: Enqueued to RX FIFO: 0x{1:X}", id, receiveData);\n')
        new_lines.append('        }\n')
        new_lines.append('        transmitCounter = 0;\n')
        new_lines.append('        receiveData = 0;\n')
        new_lines.append('        if (txBuffer.Count != 0)\n')
        new_lines.append('        {\n')
        new_lines.append('          txBuffer.TryDequeue(out transmitData);\n')
        new_lines.append('          if (RegisteredPeripheral != null)\n')
        new_lines.append('          {\n')
        new_lines.append('            externalReceiveData = RegisteredPeripheral.Transmit((byte)transmitData);\n')
        new_lines.append('            this.Log(LogLevel.Noisy, "SPI{0}: Transmitted 0x{1:X} to external, got 0x{2:X}", id, (byte)transmitData, externalReceiveData);\n')
        new_lines.append('          }\n')
        new_lines.append('        }\n')
        new_lines.append('        else\n')
        new_lines.append('        {\n')
        new_lines.append('          transmitData = 0;\n')
        new_lines.append('          SetMultiplePins(txPins, false);\n')
        new_lines.append('          SetMultiplePins(clockPins, false);\n')
        new_lines.append('          _executionThread.Stop();\n')
        new_lines.append('          running = false;\n')
        new_lines.append('          transmitCounter = 16;\n')
        new_lines.append('          return;\n')
        new_lines.append('        }\n')
        new_lines.append('      }\n')
        new_lines.append('\n')
        new_lines.append('      bool clockWasHigh = ReadMultiplePins(clockPins);\n')
        new_lines.append('\n')
        new_lines.append('      // SPI Mode 0: set data BEFORE raising clock so data is stable at rising edge\n')
        new_lines.append('      if (!clockWasHigh)\n')
        new_lines.append('      {\n')
        new_lines.append('        bool bitToSend = Convert.ToBoolean((transmitData >> (dataSize - 1 - transmitCounter)) & 1);\n')
        new_lines.append('        SetMultiplePins(txPins, bitToSend);\n')
        new_lines.append('      }\n')
        new_lines.append('\n')
        new_lines.append('      SetMultiplePins(clockPins, !clockWasHigh);\n')
        new_lines.append('      gpio.ReevaluatePio((uint)steps);\n')
        new_lines.append('\n')
        new_lines.append('      if (!clockWasHigh)\n')
        new_lines.append('      {\n')
        new_lines.append('        // Read data on rising edge\n')
        new_lines.append('        if (loopbackMode)\n')
        new_lines.append('        {\n')
        new_lines.append('          // In loopback mode, feed transmitted bit back to receiver\n')
        new_lines.append('          bool transmittedBit = Convert.ToBoolean((transmitData >> (dataSize - 1 - transmitCounter)) & 1);\n')
        new_lines.append('          receiveData = (ushort)((receiveData << 1) | Convert.ToUInt16(transmittedBit));\n')
        new_lines.append('        }\n')
        new_lines.append('        else\n')
        new_lines.append('        {\n')
        new_lines.append('          bool receivedBit;\n')
        new_lines.append('          if (RegisteredPeripheral != null)\n')
        new_lines.append('          {\n')
        new_lines.append('            receivedBit = Convert.ToBoolean((externalReceiveData >> (dataSize - 1 - transmitCounter)) & 1);\n')
        new_lines.append('          }\n')
        new_lines.append('          else\n')
        new_lines.append('          {\n')
        new_lines.append('            receivedBit = ReadMultiplePins(rxPins);\n')
        new_lines.append('          }\n')
        new_lines.append('          receiveData = (ushort)((receiveData << 1) | Convert.ToUInt16(receivedBit));\n')
        new_lines.append('        }\n')
        new_lines.append('        transmitCounter += 1;\n')
        new_lines.append('      }\n')
        new_lines.append('    }\n')

        # Now we need to skip the old Step implementation.
        # It ends at the next 'public uint ReadDoubleWord' or similar.
        skip_until = 'public uint ReadDoubleWord'
        continue

    new_lines.append(line)

with open(filepath, 'w') as f:
    f.writelines(new_lines)
