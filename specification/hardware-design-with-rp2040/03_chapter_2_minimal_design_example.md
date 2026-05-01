Hardware design with RP2040

## **Chapter 2. Minimal design example**

_Figure 2. KiCad 3D rendering of the minimal design example_

**==> picture [319 x 199] intentionally omitted <==**

This minimal design example is intended to demonstrate how you can get started with your own RP2040 based PCB designs. It consists of very nearly the minimum amount of circuitry required to make a functional design that can run your code. Schematics and layout files are available for KiCad at https://datasheets.raspberrypi.com/rp2040/MinimalKiCAD.zip. KiCad is a free, open source suite of tools for designing PCBs and can be found at https://kicad.org/.

This example PCB has two copper layers, and has components on the top side only (this makes it cheaper and easier to assemble). It also uses small SMD (surface-mount devices) components. The relatively large minimum track width, clearances and hole sizes should make this design easily and cheaply manufacturable from a range of PCB suppliers. The board is nominally 1mm thick, but it could be manufactured with a thicker PCB, for example 1.6mm is very common, but you might run into difficulties with the USB characteristic impedance (discussed below).

Whilst it might be seen as beneficial to use large, easily hand-solderable components for such an example design, the reality is that RP2040 is a 56 pin, 7×7mm QFN (Quad Flat No-leads) package with a small pitch (0.4mm pin-to-pin spacing). This requires a considerable amount of skill and experience to hand solder successfully. We therefore consider it best to have the PCBs machine assembled, however, if you are able to wield a soldering iron deftly enough to solder a QFN package successfully, then the use of other small SMD components (such as 0402 capacitors) should present few problems.

Chapter 2. Minimal design example

**6**

Hardware design with RP2040

_Figure 3. Schematic section RP2040 connections_

**==> picture [319 x 262] intentionally omitted <==**

This design consists of four main elements: **power, flash storage, crystal oscillator, and I/Os (input/outputs)** , and we’ll consider each in turn below.

## **2.1. Power**

At its simplest, RP2040 requires two different voltage supplies, **3.3V** (for the I/O) and **1.1V** (for the chip’s digital core). Fortunately, there is an _internal low-dropout voltage regulator_ (LDO) built into the device, which converts 3.3V to 1.1V for us, so we don’t have to worry too much about the 1.1V supply.

## **2.1.1. Input supply**

_Figure 4. Schematic section showing the power input_

**==> picture [319 x 175] intentionally omitted <==**

The input power connection for this design is via the 5V VBUS pin of a Micro-USB connector (labelled **J1** in Figure 4). This is a common method of powering electronic devices, and it makes sense here, as RP2040 has USB functionality, which we will be wiring to the data pins of this connector. As we need only 3.3V for this design, we need to lower the incoming 5V USB supply, in this case, using a second, _external_ LDO voltage regulator. The _NCP1117_ ( **U1** ) chosen here has a fixed output of 3.3V, is widely available, and can provide up to 1A of current, which will be plenty for most designs.

2.1. Power

**7**

Hardware design with RP2040

A look at the datasheet for the NCP1117 tells us that this device requires a **10μF** capacitor on the input, and another on the output ( **C1** and **C4** ).

## **2.1.2. Decoupling capacitors**

_Figure 5. Schematic section showing the RP2040  power supply inputs, voltage regulator and decoupling capacitors_

**==> picture [319 x 100] intentionally omitted <==**

Another aspect of the power supply design are the decoupling capacitors required for RP2040. These provide two basic functions. Firstly, they filter out power supply noise, and secondly, provide a local supply of charge that the circuits inside RP2040 can use at short notice. This prevents the voltage level in the immediate vicinity from dropping too much when the current demand suddenly increases. Because, of this, it is **important to place decoupling close to the power pins** . Ordinarily, we recommend the use of a **100nF capacitor per power pin** , however, we deviate from this rule in a couple of instances.

_Figure 6. Section of layout showing RP2040 routing and decoupling_

**==> picture [319 x 203] intentionally omitted <==**

Firstly, in order to be able to have enough space for all of the chip pins to be able to be routed out, away from the device, we have to compromise with the amount of decoupling capacitors we can use. In this design, pins 48 and 49 of RP2040 share a single capacitor ( **C9** in Figure 6 and Figure 5), as there is not a lot of room on that side of the device. This could be overcome if we used more complex/expensive technology, such as smaller components, or a four layer PCB with components on both the top and bottom sides. _This is a design trade-off_ ; we have decreased the complexity and cost, at the expense of having less decoupling capacitance, and capacitors which are slightly further away from the chip than is optimal (this increases the inductance). This could have the effect of limiting the maximum speed the design could operate at, as the voltage supply could get too noisy and drop below the minimum allowed voltage; but for most applications, this trade-off should be acceptable.

Secondly, the internal voltage regulator has its own special requirements, as you can see below.

## **2.1.3. Internal voltage regulator**

The internal voltage regulator produces a 1.1V supply from an input of 3.3V. We simply connect the VREG_OUT pin to the DVDD pins. The regulator does have some special requirements when it comes to decoupling capacitors. **We must**

2.1. Power

**8**

Hardware design with RP2040

**place 1μF capacitors close to both the input (VREG_IN) and the output (VREG_OUT)** , in order to provide a stable 1.1V supply. The voltage regulator also has restrictions on the amount of ESR (equivalent series resistance) of these capacitors, but in practice, by using physically small ceramic chip capacitors, these requirements will almost certainly be met. In this design, capacitors **C8** and **C10** (Figure 5) are ceramic capacitors of 0402 size.

For more details on the on-chip voltage regulator see on-chip voltage regulator

## **2.2. Flash storage**

_Figure 7. Schematic section showing the flash memory and USB_BOOT circuitry_

**==> picture [319 x 208] intentionally omitted <==**

In order to be able to store program code which RP2040 can boot and run from, we need to use a flash memory, specifically, a quad SPI flash memory. The device chosen here is an _W25Q128JVS_ device ( **U2** in the Figure 7), which is a 128Mbit chip (16MB). This is the largest memory size that RP2040 can support. If your particular application doesn’t need as much storage, then a smaller, cheaper memory could be used instead.

For more details on selecting a flash device, see Section 4.10 in the RP2040 Datasheet.

As this databus can be quite high frequency and is regularly in use, the **QSPI pins of RP2040 should be wired directly to the flash, using short connections** to maintain the signal integrity, and to also reduce crosstalk in surrounding circuits. Crosstalk is where signals on one circuit net can induce unwanted voltages on a neighbouring circuit, potentially causing errors to occur.

The **QSPI_SS signal is a special case** . It is connected to the flash directly, but it also has two resistors connected to it. The first ( **R2** ) is a pull-up to the 3.3V supply. The flash memory requires the chip-select input to be at the same voltage as its own 3.3V supply pin as the device is powered up, otherwise, it does not function correctly. When the RP2040 is powered up, its QSPI_SS pin will automatically default to a pull-up, but there is a short period of time during switch-on where the state of the QSPI_SS pin cannot be guaranteed. The addition of a pull-up resistor ensures that this requirement will always be satisfied. **R2** is marked as _DNF_ (Do Not Fit) on the schematic, as we have found that with this particular flash device, the external pull-up is unnecessary. However, if a different flash is used, it may become important to be able to insert a **10kΩ** resistor here, so it has been included just in case. The second resistor ( **R1** ) is a **1kΩ** resistor, connected to a header ( **J2** ) labelled _'USB_BOOT'_ . This is because the QSPI_SS pin is used as a _'boot strap'_ ; RP2040 checks the value of this I/O during the boot sequence, and if it is found to be a logic 0, then RP2040 reverts to the BOOTSEL mode, where RP2040 presents itself as a USB mass storage device, and code can be copied directly to it. If we simply _place a jumper wire between the pins of J2_ , we pull QSPI_SS pin to ground, and if the device is then subsequently reset (e.g. by toggling the RUN pin), RP2040 will restart in BOOTSEL mode instead of attempting to run the contents of the flash.

Both **R1** and **R2** should be placed close to the flash chip, so we avoid additional lengths of copper tracks which could affect the signal.

2.2. Flash storage

**9**

Hardware design with RP2040

## **2.3. Crystal oscillator**

_Figure 8. Schematic section showing the crystal oscillator and load capacitors_

**==> picture [319 x 193] intentionally omitted <==**

Strictly speaking, RP2040 does not actually require an external clock source, as it has its own internal oscillator. However, as the frequency of this internal oscillator is not well defined or controlled, varying from chip to chip, as well as with different supply voltages and temperatures, it is recommended to use a stable external frequency source. Applications which rely on exact frequencies are not possible without an external frequency source, USB being a prime example.

Providing an external frequency source can be done in one of two ways: either by providing a **clock source with a CMOS output** (square wave of IOVDD voltage) into the XIN pin, or by using a **12MHz crystal** connected between XIN and XOUT. Using a crystal is the preferred option here, as they are both relatively cheap and very accurate.

The chosen crystal for this design is an _ABM8-272-T3_ ( **Y1** in Figure 8). This is the same 12MHz crystal used on the Raspberry Pi Pico. We highly recommend using this crystal along with the accompanying circuitry to ensure that the clock starts quickly under all conditions without damaging the crystal itself. The crystal has a 30ppm frequency tolerance, which should be good enough for most applications. Along with a frequency tolerance of +/-30ppm, it has a maximum ESR of **50Ω** , and a load capacitance of **10pF** , both of which had a bearing on the choice of accompanying components.

For a crystal to oscillate at the desired frequency, the manufacturer specifies the _load capacitance_ that it needs for it to do so, and in this case, it is **10pF** . This load capacitance is achieved by placing two capacitors of equal value, one on each side of the crystal to ground ( **C2** and **C3** ). From the crystal’s point of view, these capacitors are connected in _series_ between its two terminals. Basic circuit theory tells us that they combine to give a capacitance of **(C2*C3)/(C2+C3)** , and as **C2=C3** , then it is simply **C2/2** . In this example, we’ve used **15pF** capacitors, so the series combination is **7.5pF** . In addition to this intentional load capacitance, we must also add a value for the unintentional extra capacitance, or parasitic capacitance, that we get from the PCB tracks and the XIN and XOUT pins of RP2040. We’ll assume a value of **3pF** for this, and as this capacitance is in _parallel_ to **C2** and **C3** , we simply add this to give us a _total load capacitance_ of **10.5pF** , which is close enough to the target of **10pF** . As you can see, the parasitic capacitance of the PCB traces are a factor, and we therefore need to keep them small so we don’t upset the crystal and stop it oscillating as intended. Try and keep the layout as short as possible.

The second consideration is the _maximum ESR_ (equivalent series resistance) of the crystal. We’ve opted for a device with a maximum of **50Ω** , as we’ve found that this, along with a **1kΩ** series resistor ( **R5** ), is a good value to prevent the crystal being over-driven and being damaged **when using an IOVDD level of 3.3V** . However, if IOVDD is less than 3.3V, then the drive current of the XIN/XOUT pins is reduced, and you will find that the amplitude of the crystal is lower, or may not even oscillate at all. In this case, a smaller value of the series resitor will need to be used. _Any deviation from the crystal circuit shown here, or with an IOVDD level other than 3.3V, will require extensive testing to ensure that the crystal oscillates under all conditions, and starts-up sufficiently quickly as not to cause problems with your application._

2.3. Crystal oscillator

**10**

Hardware design with RP2040

## **2.3.1. Recommended crystal**

For original designs using RP2040 we recommend using the Abracon ABM8-272-T3. For example, in addition to the minimal design example, see the Pico board schematic in Appendix B of the Raspberry Pi Pico Datasheet and the Pico design files.

For the best performance and stability across typical operating temperature ranges, use the Abracon ABM8-272-T3. You can source the ABM8-272-T3 directly from Abracon or from an authorised reseller. Pico has been specifically tuned for the ABM8-272-T3, which has the following specifications:

_Table 1. Key Crystal Specifications._

|**Parameters**|**Minimum**|**Typical**|**Maximum**|**Units**|**Notes**|
|---|---|---|---|---|---|
|Center Frequency|12.000|12.000|12.000|MHz||
|Operation Mode|Fundamental-AT|Fundamental-AT|Fundamental-AT|||
|Operating Temperature|-40||+85|ºC||
|Storage Temperature|-55||+125|ºC||
|Frequency Tolerance (25ºC)|-30||+30|ppm||
|Frequency Stability (25ºC)|-30||+30|ppm||
|Equivalent Series Resistance (R1)|||50|Ω||
|Shunt Capacitance (C0)|||3.0|pF||
|Load Capacitance (CL)|10|10|10|pF||
|Drive Level||10|200|µW||
|Aging|-5||+5|ppm|@25±3°C, 1st year|
|Insulation Resistance|500|||MΩ|@100Vdc±15V|



Even if you use a crystal with similar specifications, you will need to test the circuit over a range of temperatures to ensure stability.

The crystal oscillator is powered from the VDDIO voltage. As a result, the Abracon crystal and that particular damping resistor are tuned for 3.3V operation. If you use a different IO voltage, you will need to re-tune.

Any changes to crystal parameters risk instability across any components connected to the crystal circuit.

If you can’t source the recommended crystal directly from Abracon or a reseller, contact applications@raspberrypi.com.

## **2.4. I/Os**

## **2.4.1. USB**

_Figure 9. Schematic section showing the USB pins of RP2040 and series termination_

**==> picture [319 x 95] intentionally omitted <==**

The RP2040 provides two pins to be used for _full speed_ (FS) or _low speed_ (LS) USB, either as a _host or device_ , depending on the software used. As we’ve already discussed, RP2040 can also boot as a USB mass storage device, so wiring up

2.4. I/Os

**11**

Hardware design with RP2040

these pins to the USB connector ( **J1** in Figure 4) makes sense. The USB_DP and USB_DM pins on RP2040 do not require any additional pull-ups or pull-downs (required to indicate speed, FS or LS, or whether it is a host or device), as these are built in to the I/Os. However, these I/Os do **require 27Ω series termination resistors (R3 and R4 in Figure 9), placed close to the chip** , in order to meet the USB impedance specification.

Even though RP2040 is limited to full speed data rate (12Mbps), we should try and makes sure that the _characteristic impedance_ of the transmission lines (the copper tracks connecting the chip to the connector) are close to the USB specification of **90Ω** (measured differentially). On a **1mm** thick board such as this, if we use **0.8mm** wide tracks on USB_DP and USB_DM, with a gap of **0.15mm** between them, we should get a differential characteristic impedance of around **90Ω** . This is to ensure that the signals can travel along these transmission lines as cleanly as possible, minimising voltage reflections which can reduce the integrity of the signal. In order for these transmission lines to work properly, we _need to make sure that directly below these lines is a ground_ . A solid, uninterrupted area of ground copper, stretching the entire length of the track. On this design, almost the entirety of the bottom copper layer is devoted to ground, and particular care was taken to ensure that the USB tracks pass over nothing but ground. If a PCB _thicker than 1mm_ is chosen for your build, then we have two options. We could re-engineer the USB transmission lines to compensate for the greater distance between the track and ground underneath (which could be a physical impossibility), or we could ignore it, and hope for the best. USB FS can be quite forgiving, but your mileage may vary. It is likely to work in many applications, but it’s probably not going to be compliant to the USB standard.

## **2.4.2. I/O headers**

_Figure 10. Schematic section showing the 2.54mm I/O headers_

**==> picture [319 x 261] intentionally omitted <==**

In addition to the USB connector already mentioned, there are a pair of _2×18-way 2.54mm headers_ ( **J3** and **J4** in Figure 10), one on each side of the board, to which the rest of the I/O have been connected. As this is a general purpose design, with no particular application in mind, the I/O have been made available to be connected as the user wishes. The inner row of pins on each header are the I/Os, and the outer row are all connected to ground. It is good practice to include many grounds on I/O connectors. This helps to maintain a low impedance ground, and also to provide plenty of potential return paths for currents travelling to and from the I/O connections. This is important to minimise electromagnetic interference which can be caused by the return currents of quickly switching signals taking long, looping paths to complete the circuit.

Both headers are on the same 2.54mm grid, which makes connecting this board to other things, such as breadboards, easier. You might want to consider fitting only a single row 18-way header instead of the 2×18-way, dispensing with the outer row of ground connections, to make it more convenient to fit to a breadboard.

2.4. I/Os

**12**

Hardware design with RP2040

## **2.5. Schematic**

The complete schematic is shown below. As previously mentioned, the design files are available in KiCad format.

_Figure 11. Complete schematic of the minimal board_

**==> picture [425 x 295] intentionally omitted <==**

## **2.6. Supported flash chips**

The initial flash probe sequence, used by the bootrom to extract the second stage from flash, uses an 03h serial read command, with 24-bit addressing, and a serial clock of approximately 1MHz. It repeatedly cycles through the four combinations of clock polarity and clock phase, looking for a valid second stage CRC32 checksum.

As the second stage is then free to configure execute-in-place using the same 03h serial read command, RP2040 can perform cached flash execute-in-place with **any** chip supporting 03h serial read with 24-bit addressing, which includes most 25-series flash devices. The SDK provides an example second stage for CPOL=0 CPHA=0, at https://github.com/ raspberrypi/pico-sdk/blob/master/src/rp2040/boot_stage2/boot2_generic_03h.S. To support flash programming using the routines in the bootrom, the device must also respond to the following commands:

- [02h][ 256-byte page program]

- [05h][ status register read]

- [06h][ set write enable latch]

- [20h][ 4kB sector erase]

RP2040 also supports a wide variety of dual-SPI and QSPI access modes. For example, https://github.com/raspberrypi/ pico-sdk/blob/master/src/rp2040/boot_stage2/boot2_w25q080.S configures a Winbond W25Q-series device for quadIO continuous read mode, where RP2040 sends quad-IO addresses (without a command prefix) and the flash responds with quad-IO data.

Some caution is needed with flash XIP modes where the flash device stops responding to standard serial commands, like the Winbond continuous read mode mentioned above. This can cause issues when RP2040 is reset, but the flash device is not power-cycled, because the flash will then not respond to the bootrom’s flash probe sequence. Before issuing the 03h serial read, the bootrom always issues the following fixed sequence, which is a best-effort sequence for

2.5. Schematic

**13**

Hardware design with RP2040

discontinuing XIP on a range of flash devices:

- CSn=1, IO[3:0]=4’b0000 (via pull downs to avoid contention), issue ×32 clocks

- CSn=0, IO[3:0]=4’b1111 (via pull ups to avoid contention), issue ×32 clocks

- [CSn=1]

- CSn=0, MOSI=1’b1 (driven low-Z, all other I/Os Hi-Z), issue ×16 clocks

If your chosen device does not respond to this sequence when in its continuous read mode, then it must be kept in a state where each transfer is prefixed by a serial command, otherwise RP2040 will not be able to recover following an internal reset.

## **2.7. Making a PCB**

The minimal design example, see Chapter 2, was deliberately designed with two copper layers, and with components on the top side only. The design rules are relaxed, to allow low cost PCB fabrication. This particular design has been verified to work with Eurocircuits (https://www.eurocircuits.com/) standard PCB pool, though there should be few problems having it manufactured by other PCB prototyping manufacturers.

2.7. Making a PCB

**14**
