RP2040 Datasheet

## **Chapter 5. Electrical and Mechanical**

Physical and electrical details of the RP2040 chip.

## **5.1. Package**

_Figure 166. Top down view (left, top) and side view (right, top), along with bottom view (left, bottom) of the RP2040 QFN-56 package_

**==> picture [425 x 392] intentionally omitted <==**

**----- Start of picture text -----**<br>
PIN 1<br>**----- End of picture text -----**<br>


##  **NOTE**

There is no standard size for the central GND pad (or ePad) with QFNs. However, the one on RP2040 is smaller than most. This means that standard 0.4mm QFN-56 footprints provided with CAD tools may need adjusting. This gives the opportunity to route between the central pad and the ones on the periphery, which can help with maintaining power and ground integrity on cheaper PCBs. See Minimal Design Example for an example.

5.1. Package

**607**

RP2040 Datasheet

##  **NOTE**

Leads have a matte Tin (Sn) finish. Annealing is done post-plating, baking at 150°C for 1 hour. Minimum thickness for lead plating is 8 micromns, and the intermediate layer material is CuFe2P (roughened Copper (Cu)).

## **5.1.1. Thermal characteristics**

The thermal characteristics of the package are shown in Table 611.

|_Table 611. Thermal_<br>_data for the RP2040_<br>_QFN 56 package._|**θJA (°C/W)**|**ψJT (°C/W)**|**ψJB (°C/W)**|**TJ (°C)**|**TT (°C)**|**θJC (°C/W)**|**θJB (°C/W)**|
|---|---|---|---|---|---|---|---|
||48.00|0.80|29.20|42.00|41.8|19.01|29.03|



## **5.1.2. Recommended PCB Footprint**

**==> picture [356 x 262] intentionally omitted <==**

**----- Start of picture text -----**<br>
Figure 167.<br>Recommended PCB 7.75<br>Footprint for the 6.00<br>RP2040 QFN-56 3.20<br>package<br>0.20<br>7.75 6.00 3.20 5.40<br>0.20<br>0.875 1.175<br>0.40<br>5.40<br>Dimensions in mm<br>**----- End of picture text -----**<br>


## **5.1.3. Package markings**

The RP2040 7×7 mm QFN-56 package is marked as seen in Figure 168, with specifications as shown in Table 612. Coordinate origin is bottom-left of the package.

5.1. Package

**608**

RP2040 Datasheet

_Figure 168. Package marking format_

_Table 612. Marking requirements and dimensions_

**==> picture [213 x 208] intentionally omitted <==**

|**Line**|**Step**|**Item**|**Coord. X**|**Coord. Y**|**Char. Height**|**Char. Width**|**Char. Space**|
|---|---|---|---|---|---|---|---|
|1|1|Pin 1 Dot|0.5|6|0.5|0.5||
|2|1|Logo|3.5|2.395|3.83|3.05||
|3|1|RP2-B2|0.555|1.585|0.61|0.37|0.09|
|3|2|YY/WW|4.235|1.585|0.61|0.37|0.09|
|4|1|XXXXXX.00|0.555|0.775|0.61|0.37|0.09|
|4|2|TTT<br>(optional)|5.155|0.775|0.61|0.37|0.09|



##  **NOTE**

At Line 3, Step 1, the "RP2-B2" marking denotes device name "RP2" and silicon revision "B2."

## **5.2. Storage conditions**

In order to preserve the shelf and floor life of bare RP2040 devices, the recommended storage conditions in line with J- STD (020E & 033D) for RP2040 (classified MSL1) should be kept under 30°C and 85% relative humidity.

## **5.3. Solder profile**

RP2040 is a Pb-free part, with a Tp value of 260°C.

All temperatures refer to the center of the package, measured on the package body surface that is facing up during assembly reflow (live-bug orientation). If parts are reflowed in other than the normal live-bug assembly reflow orientation (i.e., dead-bug), Tp shall be within ±2°C of the live-bug Tp and still meet the Tc requirements; otherwise, the profile shall be adjusted to achieve the latter.

5.2. Storage conditions

**609**

RP2040 Datasheet

_Figure 169. Classification profile (not to scale)_

**==> picture [425 x 371] intentionally omitted <==**

**----- Start of picture text -----**<br>
Supplier T p ≥ Tc User T p ≤ Tc<br>Tc<br>Tc -5°C<br>Supplier t p User t p<br>T<br>p t Tc -5°C<br>Max. Ramp Up Rate = 3°C/s p<br>Max. Ramp Down Rate = 6°C/s<br>TL<br>t<br>Tsmax Preheat Area<br>Tsmin<br>ts<br>25<br>Time 25°C to Peak<br>Time<br>Temperature<br>**----- End of picture text -----**<br>


##  **NOTE**

Reflow profiles in this document are for classification/preconditioning, and are not meant to specify board assembly profiles. Actual board assembly profiles should be developed based on specific process needs and board designs, and should not exceed the parameters in Table 613.

_Table 613. Solder profile values_

|**Profile feature**|**Value**|
|---|---|
|Temperature min (Tsmin)|150°C|
|Temperature max (Tsmax)|200°C|
|Time (ts) from (Tsminto Tsmax)|60 — 120 seconds|
|Ramp-up rate (TLto Tp)|3°C/second max.|
|Liquidous temperature (TL)|217°C|
|Time (tL) maintained above TL|60 to 150 seconds|
|Peak package body temperature (Tp)|260°C|
|Classification temperature (Tc)|260°C|
|Time (tp) within 5°C of the specified classification temperature (Tc)|30 seconds|
|Ramp-down rate (Tpto TL)|6°C/second max.|
|Time 25°C to peak temperature|8 minutes max.|



5.3. Solder profile

**610**

RP2040 Datasheet

## **5.4. Compliance**

RP2040 is compliant to Moisture Sensitivity Level 1.

RP2040 is compliant to the requirement of REACH Substances of Very High Concern (SVHC) that ECHA announced on 25 June 2020.

RP2040 is compliant to the requirement and standard of Controlled Environment-related Substance of RoHS directive (EU) 2011/65/EU and directive (EU) 2015/863.

Package Level reliability qualifications carried out on RP2040:

- [Temperature Cycling per JESD22-A104]

- [HAST per JESD22-A110]

- [HTSL per JESD22-A103]

##  **NOTE**

A tin whiskers test is not performed as RP2040 is a bottom only termination device (QFN package) which not applicable to JEDEC standard (JESD201A).

## **5.5. Pinout**

## **5.5.1. Pin Locations**

_Figure 170. RP2040 QFN-56 package pinout_

**==> picture [297 x 292] intentionally omitted <==**

5.4. Compliance

**611**

RP2040 Datasheet

## **5.5.2. Pin Definitions**

## **5.5.2.1. Pin Types**

In the following GPIO Pin table (Table 615), the pin types are defined as shown below.

_Table 614. Pin Types_

|**Pin Type**|**Direction**|**Description**|
|---|---|---|
|**Digital In**|Input only|**Standard Digital**. Programmable Pull-Up, Pull-Down, Slew Rate,<br>|
|**Digital IO**|Bi-directional|Schmitt Trigger and Drive Strength. Default Drive Strength is 4mA.|
|**Digital In (FT)**|Input only|**Fault Tolerant Digital**. These pins are described as Fault Tolerant,<br>which in this case means that very little current flows into the pin<br>whilst it is below 363V and IOVDD is 0V There is also enhanced ESD|
|**Digital IO (FT)**|Bi-directional|.    .<br>protection on these pins. Programmable Pull-Up, Pull-Down, Slew Rate,<br>Schmitt Trigger and Drive Strength. Default Drive Strength is 4mA.|
|**Digital IO / Analogue**|Bi-directional (digital),<br>Input (Analogue)|**Standard Digital and ADC input**. Programmable Pull-Up, Pull-Down,<br>Slew Rate, Schmitt Trigger and Drive Strength. Default Drive Strength<br>is 4mA.|
|**USB IO**|Bi-directional|These pins are for USB use, and contain internal pull-up and pull-down<br>resistors, as per the USB specification.**Note**that external 27Ω series<br>resistors are required for USB operation.|
|**Analogue (XOSC)**||Oscillator input pins for attaching a 12MHz crystal. Alternatively, XIN<br>may be driven by a square wave.|



## **5.5.2.2. Pin List**

_Table 615. GPIO pins_

|**Name**|**Number**|**Type**|**Power Domain**|**Reset State**|**Description**|
|---|---|---|---|---|---|
|_GPIO0_|2|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO1_|3|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO2_|4|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO3_|5|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO4_|6|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO5_|7|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO6_|8|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO7_|9|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO8_|11|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO9_|12|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO10_|13|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO11_|14|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO12_|15|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO13_|16|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO14_|17|Digital IO (FT)|IOVDD|Pull-Down|User IO|
|_GPIO15_|18|Digital IO (FT)|IOVDD|Pull-Down|User IO|



5.5. Pinout

**612**

RP2040 Datasheet

|_Table 616. QSPI pins_<br>_Table 617. Crystal_<br>_oscillator pins_<br>_Table 618. Serial wire_<br>_debug pins_<br>_Table 619._<br>_Miscellaneous pins_|**Name**|**Number**|**Number**|**Type**|**Type**|**Power Domain**|**Power Domain**|**Reset State**|**Reset State**|**Description**|
|---|---|---|---|---|---|---|---|---|---|---|
||_GPIO16_|27||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO17_|28||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO18_|29||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO19_|30||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO20_|31||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO21_|32||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO22_|34||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO23_|35||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO24_|36||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO25_|37||Digital IO (FT)||IOVDD||Pull-Down||User IO|
||_GPIO26 / ADC0_|38||Digital IO /<br>Analogue||IOVDD /<br>ADC_AVDD||Pull-Down||User IO or ADC<br>input|
||_GPIO27 / ADC1_|39||Digital IO /<br>Analogue||IOVDD /<br>ADC_AVDD||Pull-Down||User IO or ADC<br>input|
||_GPIO28 / ADC2_|40||Digital IO /<br>Analogue||IOVDD /<br>ADC_AVDD||Pull-Down||User IO or ADC<br>input|
||_GPIO29 / ADC3_|41||Digital IO /<br>Analogue||IOVDD /<br>ADC_AVDD||Pull-Down||User IO or ADC<br>input|
||||||||||||
||**Name**|**Number**||**Type**||**Power Domain**||**Reset State**||**Description**|
||_QSPI_SD3_|51||Digital IO||IOVDD||||QSPI data|
||_QSPI_SCLK_|52||Digital IO||IOVDD||Pull-Down||QSPI clock|
||_QSPI_SD0_|53||Digital IO||IOVDD||||QSPI data|
||_QSPI_SD2_|54||Digital IO||IOVDD||||QSPI data|
||_QSPI_SD1_|55||Digital IO||IOVDD||||QSPI data|
||_QSPI_CSn_|56||Digital IO||IOVDD||Pull-Up||QSPI chip select|
||||||||||||
||**Name**||**Number**||**Type**||**Power Domain**||**Description**||
||_XIN_||20||Analogue (XOSC)||IOVDD||Crystal oscillator. XIN<br>may also be driven by<br>a square wave.||
||_XOUT_||21||Analogue (XOSC)||IOVDD||Crystal oscillator.||
||||||||||||
||**Name**|**Number**||**Type**||**Power Domain**||**Reset State**||**Description**|
||_SWCLK_|24||Digital In (FT)||IOVDD||Pull-Up||Debug clock|
||_SWD_|25||Digital IO (FT)||IOVDD||Pull-Up||Debug data|
||||||||||||
||**Name**|**Number**||**Type**||**Power Domain**||**Reset State**||**Description**|
||_RUN_|26||Digital In (FT)||IOVDD||Pull-Up||Chip enable /<br>reset|



5.5. Pinout

**613**

RP2040 Datasheet

|_Table 620. USB pins_<br>_Table 621. Power_<br>_supply pins_|**Name**|**Number**|**Number**|**Type**|**Type**|**Power Domain**|**Power Domain**|**Reset State**|**Reset State**|**Description**|
|---|---|---|---|---|---|---|---|---|---|---|
||_TESTEN_|19||Digital In||IOVDD||Pull-Down||Test enable<br>(connect to Gnd)|
||||||||||||
||**Name**||**Number**||**Type**||**Power Domain**||**Description**||
||_USB_DP_||47||USB IO||USB_VDD||USB Data +ve. 27Ω<br>series resistor<br>required for USB<br>operation||
||_USB_DM_||46||USB IO||USB_VDD||USB Data -ve. 27Ω<br>series resistor<br>required for USB<br>operation||
||||||||||||
||**Name**|||**Number(s)**||||**Description**|||
||_IOVDD_|||1, 10, 22, 33, 42, 49||||IO supply|||
||_DVDD_|||23, 50||||Core supply|||
||_VREG_VIN_|||44||||Voltage regulator input supply|||
||_VREG_VOUT_|||45||||Voltage regulator output|||
||_USB_VDD_|||48||||USB supply|||
||_ADC_AVDD_|||43||||ADC supply|||
||_GND_|||57||||Common ground connection via<br>central pad|||



## **5.5.3. Pin Specifications**

The following electrical specifications are obtained from characterisation over the specified temperature and voltage ranges, as well as process variation, unless the specification is marked as 'Simulated'. In this case, the data is for information purposes only, and is not guaranteed.

## **5.5.3.1. Absolute Maximum Ratings**

_Table 622. Absolute maximum ratings for digital IO (Standard and Fault Tolerant)_

|**Parameter**|**Symbol**|**Minimum**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|---|
|_I/O Supply Voltage_|IOVDD|-0.5|3.63|V||
|_Voltage at IO_|VPIN|-0.5|IOVDD + 0.5|V||



## **5.5.3.2. ESD Performance**

|_Table 623. ESD_<br>_performance for all_<br>_pins, unless otherwise_<br>_stated_|**Parameter**|**Symbol**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|---|
||_Human Body Model_|HBM|2|kV|Compliant with JEDEC<br>specification JS-001-<br>2012 (April 2012)|



5.5. Pinout

**614**

RP2040 Datasheet

|**Parameter**|**Symbol**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|
|_Human Body Model_<br>**_Digital (FT) pins only_**|HBM|4|kV|Compliant with JEDEC<br>specification JS-001-<br>2012 (April 2012)|
|_Charged Device Model_|CDM|500|V|Compliant with<br>JESD22-C101E<br>(December 2009)|



## **5.5.3.3. Thermal Performance**

_Table 624. Thermal Performance_

|**Parameter**|**Symbol**|**Minimum**|**Typical**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|---|---|
|_Case_<br>_Temperature_|TC|-40||85|°C||



## **5.5.3.4. IO Electrical Characteristics**

_Table 625. Digital IO characteristics - Standard and FT unless otherwise stated_

|**Parameter**|**Symbol**|**Minimum**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|---|
|_Pin Input Leakage_<br>_Current_|IIN||1|μA||
|_Input Voltage High_<br>_@ IOVDD=1.8V_|VIH|0.65 * IOVDD|IOVDD + 0.3|V||
|_Input Voltage High_<br>_@ IOVDD=2.5V_|VIH|1.7|IOVDD + 0.3|V||
|_Input Voltage High_<br>_@ IOVDD=3.3V_|VIH|2|IOVDD + 0.3|V||
|_Input Voltage Low_<br>_@ IOVDD=1.8V_|VIL|-0.3|0.35 * IOVDD|V||
|_Input Voltage Low_<br>_@ IOVDD=2.5V_|VIL|-0.3|0.7|V||
|_Input Voltage Low_<br>_@ IOVDD=3.3V_|VIL|-0.3|0.8|V||
|_Input Hysteresis_<br>_Voltage @_<br>_IOVDD=1.8V_|VHYS|0.1 * IOVDD||V|Schmitt Trigger<br>enabled|
|_Input Hysteresis_<br>_Voltage @_<br>_IOVDD=2.5V_|VHYS|0.2||V|Schmitt Trigger<br>enabled|
|_Input Hysteresis_<br>_Voltage @_<br>_IOVDD=3.3V_|VHYS|0.2||V|Schmitt Trigger<br>enabled|
|_Output Voltage_<br>_High @_<br>_IOVDD=1.8V_|VOH|1.24|IOVDD|V|IOH= 2, 4, 8 or<br>12mA depending<br>on setting|



5.5. Pinout

**615**

RP2040 Datasheet

|_Table 626. USB IO_<br>_characteristics_|**Parameter**|**Symbol**|**Minimum**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|---|---|
||_Output Voltage_<br>_High @_<br>_IOVDD=2.5V_|VOH|1.78|IOVDD|V|IOH= 2, 4, 8 or<br>12mA depending<br>on setting|
||_Output Voltage_<br>_High @_<br>_IOVDD=3.3V_|VOH|2.62|IOVDD|V|IOH= 2, 4, 8 or<br>12mA depending<br>on setting|
||_Output Voltage_<br>_Low @_<br>_IOVDD=1.8V_|VOL|0|0.3|V|IOL= 2, 4, 8 or<br>12mA depending<br>on setting|
||_Output Voltage_<br>_Low @_<br>_IOVDD=2.5V_|VOL|0|0.4|V|IOL= 2, 4, 8 or<br>12mA depending<br>on setting|
||_Output Voltage_<br>_Low @_<br>_IOVDD=3.3V_|VOL|0|0.5|V|IOL= 2, 4, 8 or<br>12mA depending<br>on setting|
||_Pull-Up Resistance_|RPU|50|80|kΩ||
||_Pull-Down_<br>_Resistance_|RPD|50|80|kΩ||
||_Maximum Total_<br>_IOVDD current_|IIOVDD_MAX||50|mA|Sum of all current<br>being sourced by<br>GPIO and QSPI<br>pins|
||_Maximum Total_<br>_VSS current due to_<br>_IO (IOVSS)_|IIOVSS_MAX||50|mA|Sum of all current<br>being sunk into<br>GPIO and QSPI<br>pins|
||||||||
||**Parameter**|**Symbol**|**Minimum**|**Maximum**|**Units**|**Comment**|
||_Pin Input Leakage_<br>_Current_|IIN||1|μA||
||_Single Ended Input_<br>_Voltage High_|VIHSE|2||V||
||_Single Ended Input_<br>_Voltage Low_|VILSE||0.8|V||
||_Differential Input_<br>_Voltage High_|VIHDIFF|0.2||V||
||_Differential Input_<br>_Voltage Low_|VILDIFF||-0.2|V||
||_Output Voltage_<br>_High_|VOH|2.8|USB_VDD|V||
||_Output Voltage_<br>_Low_|VOL|0|0.3|V||
||_Pull-Up Resistance_<br>_- RPU2_|RPU2|0.873|1.548|kΩ||



5.5. Pinout

**616**

RP2040 Datasheet

|**Parameter**|**Symbol**|**Minimum**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|---|
|_Pull-Up Resistance_<br>_- RPU1&2_|RPU1&2|1.398|3.063|kΩ||
|_Pull-Down_<br>_Resistance_|RPD|14.25|15.75|kΩ||



_Table 627. ADC characteristics_

_Table 628. Oscillator pin characteristics when using a Square Wave input_

|**Parameter**|**Symbol**|**Minimum**|**Maximum**|**Units**|**Comment**|
|---|---|---|---|---|---|
|_ADC Input Voltage_<br>_Range_|VPIN_ADC|0|ADC_AVDD|V||
|_Effective Number_<br>_of Bits_|ENOB|8.7||bits|SeeSection 4.9.3|
|_Resolved Bits_|||12|bits||
|_ADC Input_<br>_Impedance_|RIN_ADC|100||kΩ||
|||||||
|**Parameter**|**Symbol**|**Minimum**|**Maximum**|**Units**|**Comment**|
|_Input Voltage High_|VIH|0.65*IOVDD|IOVDD + 0.3|V|XIN only. XOUT<br>floating|
|_Input Voltage Low_|VIL|0|0.35*IOVDD|V|XIN only. XOUT<br>floating|



See Section 2.16 for more details on the Oscillator, and Minimal Design Example for information on crystal usage.

## **5.5.3.5. Interpreting GPIO output voltage specifications**

The GPIOs on RP2040 have four different output drive strengths, which are nominally called 2, 4, 8 and 12mA modes. These are not hard limits, nor do they mean that they will always be sourcing (or sinking) the selected amount of milliamps. The amount of current a GPIO sources or sinks is dependent on the load attached to it. It will attempt to drive the output to the IOVDD level (or 0V in the case of a logic 0), but the amount of current it is able to source is limited, which will be dependent on the selected drive strength. Therefore the higher the current load is, the lower the voltage will be at the pin. At some point, the GPIO will be sourcing so much current, that the voltage is so low, it won’t be recognised as a logic 1 by the input of a connected device. The purpose of the output specifications in Table 625 are to try and quantify how much lower the voltage can be expected to be, when drawing specified amounts of current from the pin.

The Output High Voltage (VOH) is defined as the lowest voltage the output pin can be when driven to a logic 1 with a particular selected drive strength; e.g., 4mA being sourced by the pin whilst in 4mA drive strength mode. The Output Low Voltage is similar, but with a logic 0 being driven.

In addition to this, the sum of all the IO currents being sourced (i.e. when outputs are being driven high) from the IOVDD bank (essentially the GPIO and QSPI pins), must not exceed IIOVDD_MAX. Similarly, the sum of all the IO currents being sunk (i.e. when the outputs are being driven low) must not exceed IIOVSS_MAX.

5.5. Pinout

**617**

RP2040 Datasheet

_Figure 171. Typical Current vs Voltage curves of a GPIO output._

**==> picture [319 x 329] intentionally omitted <==**

Figure 171 shows the effect on the output voltage as the current load on the pin increases. You can clearly see the effect of the different drive strengths; the higher the drive strength, the closer the output voltage is to IOVDD (or 0V) for a given current. The minimum VOH and maximum VOL limits are shown in red. You can see that at the specified current for each drive strength, the voltage is well within the allowed limits, meaning that this particular device could drive a lot more current and still be within VOH/VOL specification. This is a typical part at room temperature, there will be a spread of other devices which will have voltages much closer to this limit. Of course, if your application doesn’t need such tightly controlled voltages, then you can source or sink more current from the GPIO than the selected drive strength setting, but experimentation will be required to determine if it indeed safe to do so in your application, as it will be outside the scope of this specification.

## **5.5.3.6. Pin IO Delays**

These delays include PIO’s input/output mapping logic, IO muxing, and the actual pad delays into a nominal load of 5 pF. Min/max is over the extremes of process variation, voltage (1.1 V +- 10%) and temperature (-40 C to 125 C).

These delays assume an IOVDD of 1.8 V, with PADS_VSEL set. At IOVDD = 3.3 V, the delay is significantly lower, and the range is smaller.

The flops themselves have a typical setup time of 10.6 ps and hold time of 2.2 ps. The IO delays between flops and pads must be taken into account.

For minimum and maximum output delays, from CLK_SYS arriving at any flop in PIO to the data being valid at a particular GPIO pad see Table 629.

||GPIO pad seeTable 629.|||
|---|---|---|---|
|_Table 629. Pin_<br>_minimum and_<br>_maximum delays from_<br>_flop to pad, in_<br>_nanoseconds._|**Pad output**|**Min delay (ns)**|**Max delay (ns)**|
||GPIO0|2.27|7.10|
||GPIO1|2.31|7.07|



5.5. Pinout

**618**

RP2040 Datasheet

|**Pad output**|**Min delay (ns)**|**Max delay (ns)**|
|---|---|---|
|GPIO2|2.33|7.08|
|GPIO3|2.24|7.00|
|GPIO4|2.30|7.07|
|GPIO5|2.34|7.10|
|GPIO6|2.32|7.10|
|GPIO7|2.39|7.09|
|GPIO8|2.34|7.09|
|GPIO9|2.38|7.08|
|GPIO10|2.33|7.07|
|GPIO11|2.36|7.08|
|GPIO12|2.35|7.04|
|GPIO13|2.31|7.08|
|GPIO14|2.38|7.06|
|GPIO15|2.33|7.05|
|GPIO16|2.34|7.09|
|GPIO17|2.37|7.09|
|GPIO18|2.37|7.04|
|GPIO19|2.27|7.10|
|GPIO20|2.38|7.09|
|GPIO21|2.05|7.10|
|GPIO22|2.34|7.07|
|GPIO23|2.16|7.05|
|GPIO24|2.12|7.06|
|GPIO25|2.26|7.10|
|GPIO26|2.32|7.09|
|GPIO27|2.26|7.08|
|GPIO28|2.34|7.09|
|GPIO29|2.30|7.07|



For minimum and maximum input delays, from pad input to the input synchroniser, see Table 630.

|_Table 630. Pin_<br>_minimum and_<br>_maximum delays from_<br>_pad input to input_<br>_synchroniser, in_<br>_nanoseconds._|**Pad output**|**Min delay (ns)**|**Max delay (ns)**|
|---|---|---|---|
||GPIO1|1.89|5.22|
||GPIO2|1.84|5.25|
||GPIO3|1.83|5.24|
||GPIO4|1.90|5.17|
||GPIO5|1.90|5.14|



5.5. Pinout

**619**

RP2040 Datasheet

|**Pad output**|**Min delay (ns)**|**Max delay (ns)**|
|---|---|---|
|GPIO6|1.91|5.19|
|GPIO7|1.91|5.14|
|GPIO8|1.95|5.14|
|GPIO9|1.96|5.12|
|GPIO10|1.95|5.11|
|GPIO11|1.92|5.16|
|GPIO12|1.92|5.15|
|GPIO13|1.94|5.16|
|GPIO14|1.90|5.18|
|GPIO15|1.92|5.15|
|GPIO16|1.95|5.13|
|GPIO17|1.95|5.12|
|GPIO18|1.95|5.10|
|GPIO19|1.95|5.12|
|GPIO21|2.07|4.98|
|GPIO23|1.98|5.06|
|GPIO24|1.97|5.07|
|GPIO25|1.97|5.08|
|GPIO26|1.96|5.12|
|GPIO27|1.94|5.13|
|GPIO28|1.95|5.13|
|GPIO29|1.99|5.10|



For minimum and maximum input delays over all corners, from pad input to state machine IN data flops (synchronisers bypassed) see Table 631.

|_Table 631. Pin_<br>_minimum and_<br>_maximum delays from_<br>_pad input to state_<br>_machine IN data flops_<br>_(synchronisers_<br>_bypassed), in_<br>_nanoseconds._|**Pad input**|**Min delay (ns)**|**Max delay (ns)**|
|---|---|---|---|
||GPIO1|2.22|5.45|
||GPIO2|2.25|5.49|
||GPIO3|2.23|5.18|
||GPIO4|2.24|5.41|
||GPIO5|2.30|5.65|
||GPIO6|2.25|5.48|
||GPIO7|2.26|5.50|
||GPIO8|2.30|5.51|
||GPIO9|2.25|5.68|
||GPIO10|2.34|5.71|
||GPIO11|2.28|5.47|



5.5. Pinout

**620**

RP2040 Datasheet

|**Pad input**|**Min delay (ns)**|**Max delay (ns)**|
|---|---|---|
|GPIO12|2.29|5.40|
|GPIO13|2.25|5.47|
|GPIO14|2.24|5.41|
|GPIO15|2.23|5.47|
|GPIO16|2.30|5.42|
|GPIO17|2.28|5.44|
|GPIO18|2.28|5.34|
|GPIO19|2.30|5.50|
|GPIO21|2.16|5.79|
|GPIO23|2.33|5.53|
|GPIO24|2.28|5.60|
|GPIO25|2.29|5.53|
|GPIO26|2.28|5.38|
|GPIO27|2.27|5.39|
|GPIO28|2.24|5.28|
|GPIO29|2.33|5.47|



## **5.5.3.6.1. Effects of IOVDD**

All of the IO delays given above assume IOVDD = 1.8V. Increasing IOVDD to 3.3V reduces the pad delays quite significantly, and the pad delay is a large fraction of the delays reported above. See Table 632 for a summary of best and worst pad delays at 1.8V and 3.3V.

_Table 632. Best and worst pad delays at 1.8V and 3.3V._

|**Path type**|**IOVDD**|**Min delay (ns)**|**Max delay (ns)**|
|---|---|---|---|
|Output|1.8V|1.54|3.65|
|Output|3.3V|1.11|2.14|
|Input|1.8V|0.63|1.06|
|Input|3.3V|0.47|0.76|



Changing IOVDD does not affect any logic in the core domain, so these differences can be added to the IO delay tables above to estimate the IO delay ranges at IOVDD = 3.3V (see Table 633).

_Table 633. IO delay ranges at IOVDD = 3.3V._

|**Path group**|**IOVDD**|**Min delay (ns)**|**Max delay (ns)**|
|---|---|---|---|
|Output|1.8V|2.12|7.10|
|Output|3.3V|1.69|5.59|
|Input to sync|1.8V|1.83|5.25|
|Input to sync|3.3V|1.67|4.95|
|Input to SM|1.8V|2.16|5.79|
|Input to SM|3.3V|2.00|5.49|



5.5. Pinout

**621**

RP2040 Datasheet

## **5.6. Power Supplies**

_Table 634. Power Supply Specifications_

|**Power Supply**|**Supplies**|**Min**|**Typ**|**Max**|**Units**|
|---|---|---|---|---|---|
|IOVDDa|Digital IO|1.62|1.8 / 3.3|3.63|V|
|DVDDb|Digital core|1.05|1.1|1.16|V|
|VREG_VIN|Voltage regulator|1.62|1.8 / 3.3|3.63|V|
|USB_VDD|USB PHY|3.135|3.3|3.63|V|
|ADC_AVDDc|ADC|1.62|3.3|3.63|V|



a If IOVDD <2.5V, GPIO VOLTAGE_SELECT registers should be adjusted accordingly. See Section 2.9 for details.

b Short term transients should be within +/-100mV. If using 200MHz for clk_sys as described in Section 2.15.3, set DVDD to 1.15V.

c ADC performance will be compromised at voltages below 2.97V

## **5.7. Power Consumption**

## **5.7.1. Peripheral power consumption**

Baseline readings are taken with only clock sources and essential peripherals (BUSCTRL, BUSFAB, VREG, Resets, ROM, SRAMs) active in the WAKE_EN0/WAKE_EN1 registers. Clocks are set to default clock settings. Each peripheral is activated in turn by enabling all clock sources for the peripheral in the WAKE_EN0/WAKE_EN1 registers. Current consumption is the increase in current when the peripheral clocks are enabled.

_Table 635. Baseline power consumption_

|**Peripheral**|**Typical DVDD Current Consumption (μA/MHz)**|
|---|---|
|_DMA_|2.6|
|_I2C0_|3.9|
|_I2C1_|3.8|
|_IO + Pads_|23.6|
|_PIO0_|12.3|
|_PIO1_|12.5|
|_PWM_|5.0|
|_RTC_|1.1|
|_SIO_|1.9|
|_SPI0_|1.7|
|_SPI1_|1.8|
|_Timer_|1.2|
|_UART0_|3.5|
|_UART1_|3.7|
|_Watchdog_|1.0|
|_XIP_|37.6|



5.6. Power Supplies

**622**

RP2040 Datasheet

Because of fixed external reference clocks of 48 MHz, as well as the variable system clock input, ADC and USBCTRL power consumption does not vary linearly with system clock (as it does for other peripherals which only have system and/or peripheral clock inputs). Absolute DVDD current consumption of the ADC and USBCTRL blocks at standard clocks (system clock of 125 MHz) is given below:

|_Table 636. Baseline_<br>_power consumption_<br>_for ADC and USBCTRL_|**Peripheral**|**Typical DVDD Current Consumption (μA/MHz)**|
|---|---|---|
||_ADC_|0.1|
||_USBCTRL_|1.3|



## **5.7.2. Power consumption for typical user cases**

The following data shows the current consumption of various power supplies on 3 each of typical (tt), fast (ff) and slow (ss) corner RP2040 devices, with four different software use-cases.

##  **NOTE**

For power consumption of the Raspberry Pi Pico, please see the **Raspberry Pi Pico Datasheet** .

Firstly, 'Popcorn' (Media player demo) using the VGA, SD Card, and Audio board. This demo uses VGA video, I2S audio and 4-bit SD Card access, with a system clock frequency of 48MHz.

##  **NOTE**

For more details of the VGA board see the **Hardware design with RP2040** book.

Secondly, the BOOTSEL mode of RP2040. These measurements are made both with and without USB activity on the bus, using a Raspberry Pi 4 as a host.

The third use-case uses the hello_dormant binary which puts RP2040 into a low power state, DORMANT mode.

The final use-case uses the hello_sleep binary code which puts RP2040 into a low power state, SLEEP mode.

Table 637 has two columns per power supply, 'Typical Average Current' and 'Maximum Average Current'. The former is the current averaged over several seconds that you might expect a typical RP2040 to consume at room temperature and nominal voltage (e.g., DVDD=1.1V, IOVDD=3.3V, etc). The 'Maximum Average Current' is the maximum current consumption (again averaged over several seconds) you might expect to see on a worst-case RP2040 device, across the temperature extremes, and maximum voltage (e.g., DVDD=1.21V, etc).

##  **NOTE**

The 'Popcorn' consumption measurements depend on the video being displayed at the time. The 'Typical' values are obtained over several seconds of video, with varied colour and intensity. The 'Maximum' values are measured during periods of white video, when the required current is at its highest.

_Table 637. Power Consumption_

|**Software Use-**<br>**case**|**Typical**<br>**Average DVDD**<br>**Current**|**Max. Average**<br>**DVDD current**|**Typical**<br>**Average**<br>**IOVDD Current**|**Max. Average**<br>**IOVDD current**|**Typical**<br>**Average**<br>**USB_VDD**<br>**Current**|**Max. Average**<br>**USB_VDD**<br>**current**|**Units**|
|---|---|---|---|---|---|---|---|
|_Popcorn_|10.9|16.6|24.8|35.5|-|-|mA|
|_BOOTSEL_<br>_mode - Active_|9.4|14.7|1.2|4.3|1.4|2.0|mA|
|_BOOTSEL_<br>_mode - Idle_|9.0|14.3|1.2|4.3|0.2|0.6|mA|
|_Dormant_|0.18|4.2|-|-|-|-|mA|



5.7. Power Consumption

**623**

RP2040 Datasheet

|**Software Use-**<br>**case**|**Typical**<br>**Average DVDD**<br>**Current**|**Max. Average**<br>**DVDD current**|**Typical**<br>**Average**<br>**IOVDD Current**|**Max. Average**<br>**IOVDD current**|**Typical**<br>**Average**<br>**USB_VDD**<br>**Current**|**Max. Average**<br>**USB_VDD**<br>**current**|**Units**|
|---|---|---|---|---|---|---|---|
|_Sleep_|0.39|4.5|-|-|-|-|mA|



## **5.7.2.1. Power Consumption versus frequency**

To give an indication of the relationship between the core frequency that RP2040 is operating at, and the current consumed by the DVDD supply, Figure 172 shows the measured results of a typical RP2040 device, continuously running FFT calculations on both cores, at various core clock frequencies. Figure 172 also shows the effects of case temperature, and DVDD voltage upon the current consumption.

_Figure 172. DVDD Current vs Core Frequency of a typical RP2040 device, whilst running FFT calculations_

**==> picture [319 x 171] intentionally omitted <==**

5.7. Power Consumption

**624**
