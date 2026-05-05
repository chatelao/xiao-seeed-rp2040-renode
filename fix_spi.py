import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    lines = f.readlines()

new_lines = []
for line in lines:
    if 'public void OnGPIO(int number, bool value)' in line:
        new_lines.append(line)
        new_lines.append('    {\n')
        new_lines.append('        if (RegisteredPeripheral != null && RegisteredPeripheral is IGPIOReceiver receiver)\n')
        new_lines.append('        {\n')
        new_lines.append('            receiver.OnGPIO(number, value);\n')
        new_lines.append('        }\n')
        new_lines.append('    }\n')
        # Skip the next line which is the old empty body
        continue
    if line.strip() == '{}' and len(new_lines) > 0 and 'public void OnGPIO' in new_lines[-2]:
        continue

    # Also fix the bit-bang received data logic to use transmitCounter correctly
    if 'receivedBit = Convert.ToBoolean((externalReceiveData >> (dataSize - 1 - transmitCounter)) & 1);' in line:
        # Actually this logic was already correct if we want MSB first.
        # But wait, PL022 sends MSB first. transmitCounter goes 0 to 7.
        # 0: bit 7
        # 1: bit 6
        # ...
        # 7: bit 0
        # This matches (8 - 1 - counter).
        new_lines.append(line)
        continue

    new_lines.append(line)

# Wait, I need to make sure I don't double the OnGPIO or mess up the curly braces.
# Actually I'll just use replace_with_git_merge_diff for precision.
