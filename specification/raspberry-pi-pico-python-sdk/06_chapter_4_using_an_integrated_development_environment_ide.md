Raspberry Pi Pico-series Python SDK

## **Chapter 4. Using an Integrated Development Environment (IDE)**

The MicroPython port to Pico-series devices and other RP-series microcontroller-based boards works with commonly used development environments.

## **4.1. Using Thonny**

Thonny packages are available for Linux, MS Windows, and macOS. After installation, the Thonny development environment works the same on all platforms. The latest release of Thonny can be downloaded from thonny.org

Alternatively if you are working on a Raspberry Pi you should install Thonny using apt from the command line:

$ sudo apt install thonny

This will add a Thonny icon to the Raspberry Pi desktop menu. Go ahead and select Raspberry Pi → Programming → Thonny Python IDE to open the development environment.

When opening Thonny for the first time select "Standard Mode." For some versions this choice will be made via a popup when you first open Thonny. However for the Raspberry Pi release you should click on the text in the top right of the window to switch to "Regular Mode."

Make sure your Pico-series device is plugged into your computer and, click on the word ‘Python’ followed by a version number at the bottom-right of the Thonny window — this is the Python interpreter that Thonny is currently using. Normally the interpreter is the copy of Python running on Raspberry Pi, but it needs to be changed in order to run your programs in MicroPython on your Pico, clicking the current interpreter will open a drop down.

Select "MicroPython (Raspberry Pi Pico W)" or "MicroPython (Raspberry Pi Pico)" from the list, see Figure 5.

_Figure 5. Switching to MicroPython_

**==> picture [382 x 277] intentionally omitted <==**

4.1. Using Thonny

**31**

Raspberry Pi Pico-series Python SDK

##  **NOTE**

The Pico-series interpreter is only available in the latest version of Thonny. If you’re running an older version and can’t update it, look for "MicroPython (generic)" instead. If your version of Thonny is older still and has no interpreter option at the bottom-right of the window and you can’t update it, restart Thonny, click the "Run" menu, and click ‘Select interpreter.’ Click the drop-down arrow next to ‘The same interpreter that runs Thonny (default)’, click on ‘MicroPython (generic)’ in the list, then click on the drop-down arrow next to ‘Port’ and click on ‘Board in FS mode’ in that list before clicking "OK" to confirm your changes.

You can now access the REPL from the Shell panel,

>>> print('Hello Pico!') Hello Pico! >>>

see Figure 6.

_Figure 6. Saying "Hello Pico!" from the MicroPython REPL inside the Thonny environment._

**==> picture [319 x 235] intentionally omitted <==**

## **4.1.1. Blinking the LED from Thonny**

The following example uses Thonny to execute an example program that uses a timer to blink the onboard LED on your device.

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/blink/blink.py_

1 from machine import Pin, Timer 2 3 led = Pin("LED", Pin.OUT) 4 tim = Timer() 5 def tick(timer): 6     global led 7     led.toggle() 8 9 tim.init(freq=2.5, mode=Timer.PERIODIC, callback=tick)

4.1. Using Thonny

**32**

Raspberry Pi Pico-series Python SDK

Enter the code in the main panel, then click on the green run button. Thonny will present you with a popup, click on "MicroPython device" and enter test.py to save the code to the Pico-series device, see Figure 7.

_Figure 7. Saving code to Raspberry Pi Pico inside the Thonny environment._

**==> picture [319 x 234] intentionally omitted <==**

##  **TIP**

If you "save a file to the device" and give it the special name main.py, then MicroPython starts running that script as soon as power is supplied to the Pico-series device in the future.

The program should upload to the Pico-series device using the REPL, and automatically start running. The on-board LED should start to blink.

## **4.2. Using Visual Studio Code**

Visual Studio Code (VSCode) is a popular open source editor developed by Microsoft. It is the recommended Integrated Development Environment (IDE) on the Raspberry Pi 4 if you want a graphical interface to edit and debug your code.

Visual Studio Code (VSCode) can be installed in Raspberry Pi OS using the usual apt procedure:

$ sudo apt update $ sudo apt install code

Once the install has completed, you can then install the MicroPico Visual Studio Code Extension (aka Pico-W-Go) for working with MicroPython on a Pico-series device.

$ code --install-extension ms-python.python $ code --install-extension visualstudioexptteam.vscodeintellicode $ code --install-extension ms-python.vscode-pylance $ code --install-extension paulober.pico-w-go

This third-party extension includes:

- [Auto-completion and docs]

4.2. Using Visual Studio Code

**33**

Raspberry Pi Pico-series Python SDK

- [Console integration for communication with MicroPython REPL on a Pico-series device]

- [Running and transferring files to and from your board]

Finally, start Visual Studio Code from a Terminal window:

$ export PICO_SDK_PATH=/home/pi/pico/pico-sdk $ code

Open a folder and press **Ctrl-Shift-P** (or **Cmd-Shift-P** on a Mac) to open the VS Code command palette. Select **MicroPico** > **Configure Project** . Then click on the **Pico Disconnected** button on the bottom (blue) toolbar. You should now be connected to your Pico-series device, see Figure 8.

_Figure 8. Visual Studio Code running with the MicroPico extension connected to a Picoseries device._

**==> picture [425 x 329] intentionally omitted <==**

To run a program on a connected Pico-series device:

- [Select ][MicroPico > Run current file on Pico]

- [Use the status bar ][Run][ button in the bottom (blue) toolbar]

To stop a program running on a connected Pico-series device:

- [Select ][MicroPico > Stop execution]

- [Use the ][Stop][ button in the bottom (blue) toolbar]

## **4.3. Using Remote MicroPython shell (rshell)**

Remote MicroPython shell packages are available for Linux, MS Windows, and macOS. After installation, rshell works the same on all platforms. For full documentation on rshell, see the project’s GitHub repository.

4.3. Using Remote MicroPython shell (rshell)

**34**

Raspberry Pi Pico-series Python SDK

The Remote Shell for MicroPython (rshell) runs on the host. Using MicroPython’s REPL, rshell sends Python code to the Pico-series device to copy files to and from MicroPython’s own filesystem.

To install rshell, run the following command on your host device:

$ sudo apt install python3-pip $ sudo pip3 install rshell

Next, connect your board to the host device _without holding the_ BOOTSEL _button_ .

You can then connect to your Pico-series device with the following command:

$ rshell --buffer-size=512 -p /dev/ttyACM0 Connecting to /dev/ttyACM0 (buffer-size 512)... Trying to connect to REPL  connected Testing if sys.stdin.buffer exists ... N Retrieving root directories ... Setting time ... Aug 21, 2020 15:35:18 Evaluating board_name ... pyboard Retrieving time epoch ... Jan 01, 2000 Welcome to rshell. Use Control-D (or the exit command) to exit rshell. /home/pi>

You now have access to an interactive shell on your device. You can use this access to read, write, and execute files.

##  **TIP**

To view the program that runs automatically after boot, use the following command: rshell -p /dev/ttyACM0 --buffer -size 512 cat /pyboard/main.py.

## **4.3.1. Blinking the LED from rshell**

The following example uses rshell to execute an example program that uses a timer to blink the onboard LED on your device.

Create a file named blink.py that contains the following code:

_Pico MicroPython Examples: https://github.com/raspberrypi/pico-micropython-examples/blob/master/blink/blink.py_

1 from machine import Pin, Timer 2 3 led = Pin("LED", Pin.OUT) 4 tim = Timer() 5 def tick(timer): 6     global led 7     led.toggle() 8 9 tim.init(freq=2.5, mode=Timer.PERIODIC, callback=tick)

Next, copy your program to the board using rshell:

$ rshell -p /dev/ttyACM0 --buffer-size 512 cp blink.py /pyboard/main.py

4.3. Using Remote MicroPython shell (rshell)

**35**

Raspberry Pi Pico-series Python SDK

##  **TIP**

rshell represents your device’s flash storage as /pyboard.

##  **TIP**

Use the special filename main.py to automatically execute your program on boot.

The program should upload to the Pico-series device using the REPL, and automatically start running. You should see the on-board LED start blinking.

4.3. Using Remote MicroPython shell (rshell)

**36**
