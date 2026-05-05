import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    content = f.read()

# Remove the return and fix the logic to match original flow
old_block = """        else
        {
          transmitData = 0;
          SetMultiplePins(txPins, false);
          SetMultiplePins(clockPins, false);
          _executionThread.Stop();
          running = false;
          transmitCounter = 16;
        }
        return;
      }"""

new_block = """        else
        {
          transmitData = 0;
          SetMultiplePins(txPins, false);
          SetMultiplePins(clockPins, false);
          _executionThread.Stop();
          running = false;
          transmitCounter = 16;
          return;
        }
      }"""

# Wait, if I keep the return in the 'else' block, it's fine.
# But I must NOT have it if txBuffer.Count != 0.

content = content.replace(old_block, new_block)

with open(filepath, 'w') as f:
    f.write(content)
