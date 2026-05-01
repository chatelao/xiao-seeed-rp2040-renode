Getting started with Raspberry Pi Pico-series

## **Chapter 4. Load and debug a project**

The VS Code extension can create projects based on the examples provided by Pico Examples. For an example, we’ll walk you through how to create a project that blinks the LED on your Pico-series device:

1. In the VS Code left sidebar, select the Raspberry Pi Pico icon, labelled "Raspberry Pi Pico Project".

2. Select **New Project from Examples** .

3. In the **Name** field, select the **blink** example.

4. Choose the board type that matches your device.

5. Specify a folder where the extension can generate files. VS Code will create the new project in a sub-folder of the selected folder.

6. Click **Create** to create the project.

The extension will now download the SDK and the toolchain, install them locally, and generate the new project. The first project may take 5-10 minutes to install the toolchain. VS Code will ask you whether you trust the authors because we’ve automatically generated the .vscode directory for you. Select **yes** .

_Figure 2. Creating a project in VS Code._

**==> picture [425 x 266] intentionally omitted <==**

##  **NOTE**

The CMake Tools extension may display some notifications at this point. Ignore and close them.

On the left **Explorer** sidebar in VS Code, you should now see a list of files.

Open blink.c to view the blink example source code in the main window.

The Raspberry Pi Pico extension adds some capabilities to the status bar at the bottom right of the screen.

## **Compile**

Compiles the sources and builds the target UF2 file. You can copy this binary onto your device to program it.

## **Run**

Finds a connected device, flashes the code into it, and runs that code.

Chapter 4. Load and debug a project

**8**

Getting started with Raspberry Pi Pico-series

The extension sidebar also contains some quick access functions. Click on the Pico icon in the side menu and you’ll see **Compile Project** .

Hit **Compile Project** and a terminal tab will open at the bottom of the screen displaying the compilation progress.

## **4.1. Compile and Run** blink

To run the blink example:

1. Hold down the BOOTSEL button on your Pico-series device while plugging it into your development device using a micro USB cable to force it into USB Mass Storage Mode.

2. Press the **Run** button in the status bar or the **Run project** button in the sidebar.

You should see the terminal tab at the bottom of the window open. It will display information concerning the upload of the code. Once the code uploads, the device will reboot, and you should see the following output:

The device was rebooted to start the application.

Your blink code is now running. If you look at your device, the LED should blink twice every second.

## **4.2. Make a Code Change and Re-run**

To check that everything is working correctly, click on the blink.c file in VS Code. Navigate to the definition of LED_DELAY_MS at the top of the code:

#ifndef LED_DELAY_MS #define LED_DELAY_MS 250 #endif LED_DELAY_MS

1. Change the 250ms (a quarter of a second) to 100 (a tenth of a second):

#ifndef LED_DELAY_MS #define LED_DELAY_MS 100 #endif LED_DELAY_MS

2. Disconnect your device, then reconnect while holding the BOOTSEL button.

3. Press the **Run** button in the status bar or the **Run project** button in the sidebar.

You should see the terminal tab at the bottom of the window open. It will display information concerning the upload of the code. Once the code uploads, the device will reboot, and you should see the following output:

The device was rebooted to start the application.

Your blink code is now running. If you look at your device, the LED should flash faster, five times every second.

4.1. Compile and Run blink

**9**

Getting started with Raspberry Pi Pico-series

## **4.3. Debug**

The Raspberry Pi Debug Probe is a debug solution for any Arm-based computer. You can use other debug hardware with Pico-series devices, but we recommend the Debug Probe to make configuration as simple as possible. If you’d like to use a Pico-series device as a Debug Probe, see Debug with a second Pico or Pico 2.

First, connect the Debug Probe to your Pico-series device through the debug connector on the board. Depending on which Pico device you have, different connectors will be required. For Pico, Pico W, and Pico 2, use a soldering iron to solder the Debug Probe connectors onto the board. For Pico H, Pico WH, and Pico with headers, the debug header is already added. Just connect the Debug Probe with the supplied cable.

_Figure 3. Debug Probe wiring_

**==> picture [425 x 416] intentionally omitted <==**

For more information, see the Debug Probe documentation.

Now, plug the Debug Probe USB into your computer. The Debug Probe does not power the Pico device, it must be powered separately.

To start the debugger:

1. Open the extension sidebar by clicking on the Pico icon.

2. Select **Debug Project** or press **F5** .

3. If prompted to select a debugger, choose Pico Debug (Cortex-Debug)

The debugger will automatically download the code to the device, insert a breakpoint at the beginning of your main

4.3. Debug

**10**

Getting started with Raspberry Pi Pico-series

function, and run until that breakpoint is hit.

_Figure 4. Debugging in VS Code._

**==> picture [425 x 266] intentionally omitted <==**

Once in debugging mode, the sidebar has a number of windows displaying useful information about the current state of the device. At the top, a small control bar contains buttons that control code execution. Hover over the buttons to identify them. To continue code execution click **Continue** ( **F5** ).

Your blink code is now running. If you look at your device, the LED should be blinking as before. Now press Restart ( **Ctrl+Shift+F5** ) to go back to the beginning of main.

Press **Step-over** ( **F10** ) once. The highlighted line, which indicates the next line to be executed, will advance to the pico_led_init function call. To step into this function, press **Step-into** ( **F11** ). The source window will update to indicate execution is now at the beginning of the function. You can either continue to step over code until the function returns to main, or select **Step-out** ( **Shift+F11** ) to finish executing the function.

After returning to the main function, check the **Local Variables** window to see that the value of rc is 0 (PICO_OK).

Press Restart ( **Ctrl+Shift+F5** ) again to go back to the beginning of main. Then move the cursor down to the pico_set_led line and press **F9** . When you create the breakpoint, you’ll see a red dot indicating the breakpoint location:

4.3. Debug

**11**

Getting started with Raspberry Pi Pico-series

_Figure 5. Debugging in VS Code._

**==> picture [425 x 266] intentionally omitted <==**

You can add and remove a breakpoint by clicking on the red dot.

Press **Continue** ( **F5** ); execution should halt on the breakpoint. Next, press **Step-over** ( **F10** ) and you should see the LED light up.

4.3. Debug

**12**
