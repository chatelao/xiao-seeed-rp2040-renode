import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    lines = f.readlines()

# Add extra fields
new_lines = []
for line in lines:
    if 'private ushort receiveData;' in line:
        new_lines.append(line)
        new_lines.append('    private byte externalReceiveData;\n')
        continue
    new_lines.append(line)

lines = new_lines
new_lines = []

# Update OnGPIO
for line in lines:
    if 'public void OnGPIO(int number, bool value)' in line:
        new_lines.append(line)
        new_lines.append('    {\n')
        new_lines.append('        if (RegisteredPeripheral != null && RegisteredPeripheral is IGPIOReceiver receiver)\n')
        new_lines.append('        {\n')
        new_lines.append('            receiver.OnGPIO(number, value);\n')
        new_lines.append('        }\n')
        new_lines.append('    }\n')
        continue
    if line.strip() == '{}' and len(new_lines) > 0 and 'public void OnGPIO' in new_lines[-2]:
        continue
    new_lines.append(line)

lines = new_lines
new_lines = []

# Add FinishTransmission
for line in lines:
    if 'public void WriteDoubleWord(long offset, uint value)' in line:
        new_lines.append('    public void FinishTransmission()\n')
        new_lines.append('    {\n')
        new_lines.append('      if (RegisteredPeripheral != null)\n')
        new_lines.append('      {\n')
        new_lines.append('        RegisteredPeripheral.FinishTransmission();\n')
        new_lines.append('      }\n')
        new_lines.append('    }\n\n')
    new_lines.append(line)

lines = new_lines
new_lines = []

# Replace Step
skip = False
for line in lines:
    if 'private void Step()' in line:
        new_lines.append('    private void Step()\n')
        new_lines.append('    {\n')
        new_lines.append('      if (transmitCounter >= dataSize)\n')
        new_lines.append('      {\n')
        new_lines.append('        if (transmitCounter != 16)\n')
        new_lines.append('        {\n')
        new_lines.append('          rxBuffer.Enqueue(receiveData);\n')
        new_lines.append('        }\n')
        new_lines.append('        transmitCounter = 0;\n')
        new_lines.append('        receiveData = 0;\n')
        new_lines.append('        if (txBuffer.Count != 0)\n')
        new_lines.append('        {\n')
        new_lines.append('          txBuffer.TryDequeue(out transmitData);\n')
        new_lines.append('          if (RegisteredPeripheral != null)\n')
        new_lines.append('          {\n')
        new_lines.append('            externalReceiveData = RegisteredPeripheral.Transmit((byte)transmitData);\n')
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
        new_lines.append('        }\n')
        new_lines.append('        return;\n')
        new_lines.append('      }\n')
        new_lines.append('\n')
        new_lines.append('      bool clockWasHigh = ReadMultiplePins(clockPins);\n')
        new_lines.append('\n')
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
        new_lines.append('        if (loopbackMode)\n')
        new_lines.append('        {\n')
        new_lines.append('          bool bit = Convert.ToBoolean((transmitData >> (dataSize - 1 - transmitCounter)) & 1);\n')
        new_lines.append('          receiveData = (ushort)((receiveData << 1) | Convert.ToUInt16(bit));\n')
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
        skip = True
        continue

    if skip:
        if 'public uint ReadDoubleWord' in line:
            skip = False
            new_lines.append(line)
        continue

    new_lines.append(line)

with open(filepath, 'w') as f:
    f.writelines(new_lines)
