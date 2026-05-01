Getting started with Raspberry Pi Pico-series

## **Chapter 5. Say "Hello World" in C**

After blinking an LED on and off, the next thing that most developers will want to do is create and use a serial port, and say "Hello World."

## **5.1. Serial input and output on Pico-series devices**

Serial input (stdin) and output (stdout) can be directed to serial UART and/or to USB CDC (USB serial).

With a serial UART console, the input and output are sent over the UART pins on the device - by default it will use Pin 1 (GP0) for sending output (UART0_TX) and Pin 2 (GP1) for receiving input (UART0_RX). You will then need to connect the UART pins on the Pico-series device to a UART to USB converter, such as the Debug Probe, as shown in the Debug Probe Wiring diagram.

With a USB CDC console, the input and ouput are sent directly over the USB cable connected to your computer, so no additional wiring will be needed. However you may miss some of the printout when your code starts running, as your computer may take a second or two to connect to the Pico-series device after it restarts.

You can select either or both consoles when using the extension, depending on your preference.

## **5.2. Create a project**

##  **NOTE**

The SDK makes use of CMake to control its build system, see Manually Create your own Project if you don’t want to use the VS Code extension

1. In the VS Code left sidebar, select the Raspberry Pi Pico icon, labelled "Raspberry Pi Pico Project".

2. Select **New Project** .

3. In the **Name** field, name your project. For example "hello_world".

4. Choose the board type that matches your device.

5. Specify a folder where the extension can generate files. VS Code will create the new project in a sub-folder of the selected folder.

6. Under "STDIO support", select which consoles you would like

7. Click **Create** to create the project.

The extension will now generate the new project. VS Code will ask you whether you trust the authors because we’ve automatically generated the .vscode directory for you. Select yes.

## **5.3. Build your project**

To run the "Hello world" example:

1. Hold down the BOOTSEL button on your Pico-series device while plugging it into your development device using a micro USB cable to force it into USB Mass Storage Mode.

2. Press the **Run** button in the status bar or the **Run project** button in the sidebar.

5.1. Serial input and output on Pico-series devices

**13**

Getting started with Raspberry Pi Pico-series

You should see the terminal tab at the bottom of the window open. It will display information concerning the upload of the code. Once the code uploads, the device will reboot, and you should see the following output:

The device was rebooted to start the application.

Your "Hello world" code is now running.

Although the "Hello World" example is now running, we cannot yet see the text.

## **5.4. See console output**

If using STDIO UART make sure you have wired it up first. STDIO USB does not need any wiring other than being connected to your computer.

In VS Code. Go to the view menu, and select "Terminal" to open the bottom pane. In this pane, you will find the "Serial Monitor" tab. Select the serial port. There may be more than one. The baud rate should be 115200. Select "Start Monitoring" to see the output.

_Figure 6. VS Code serial monitor_

**==> picture [425 x 242] intentionally omitted <==**

5.4. See console output

**14**
