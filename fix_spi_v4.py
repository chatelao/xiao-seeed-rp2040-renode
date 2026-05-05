import sys

filepath = 'test/renode-config/emulation/peripherals/spi/rp2040_spi.cs'
with open(filepath, 'r') as f:
    content = f.read()

# Fix the double body for OnGPIO
bad_ongpio = """    public void OnGPIO(int number, bool value)
    {
        if (RegisteredPeripheral != null && RegisteredPeripheral is IGPIOReceiver receiver)
        {
            receiver.OnGPIO(number, value);
        }
    }
    {

    }"""

good_ongpio = """    public void OnGPIO(int number, bool value)
    {
        if (RegisteredPeripheral != null && RegisteredPeripheral is IGPIOReceiver receiver)
        {
            receiver.OnGPIO(number, value);
        }
    }"""

content = content.replace(bad_ongpio, good_ongpio)

with open(filepath, 'w') as f:
    f.write(content)
