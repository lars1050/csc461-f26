##### CSC461 Intelligent Systems

### Project 0 : Getting Started with Embedded Systems
Worksheets are due daily. <br>
Code is due by the end of class Thursday, September 10 <br>
Submit via GitLab (in your csc461 repo)


#### Learning Objectives

When finished with this lab, students will have/be able to:

1. Have all hardware necessary for completing Project 0 and Project 1.
1. Installed and tested all software necessary for completing Project 0.
1. Determine what Port and Pin corresponds to the GPIO pin on the Metro.
1. Use bitwise operations and (&), or (|), not (~) to clear and set bits.
1. Use a breadboard to connect the Metro to peripherals (e.g. LED).
1. Determine which port (/dev,COM) corresponds to the programmer and the serial communication.
1. Write a program in the Arduino IDE and flash onto the Metro.
1. Explain the need for the setup and loop structure of a cyclic executive.
1. Use a makefile to compile a C program and flash onto the Metro.

#### Useful Vocabulary

- Register, Port, and Pin
- bit, bitmask, clear, set
- VCC, power, ground
- COM or device port 
- schematic, datasheet
- Hertz, frequency
- peripheral, sonar sensor, serial communication

<hr>

#### Your First (and Second) Program

Get a hardware kit from Dr. Larson. This includes the Metro board, USB cable, breadboard, sonar sensor, LEDs, wires, resistors, and an orange case. You will return these items at the end of the semester.

Install the Arduino IDE (https://docs.arduino.cc/software/ide/). Please make sure that you are installing locally and not on the cloud.

Connect your Metro to your PC with the USB cable. 

Start the Arduino app and open the Blink.ino example (File-Examples-01.Basics-Blink). At the top of the window, choose the Arduino UNO board. Flash the program onto the Metro using the arrow-in-the-circle button at the top. If it cannot find the board, you might have to select the port or programmer under Tools. As it is programming the board, you will see LEDs blinking on the board. Once it is complete, a single LED should blink at a rate based on the delay in the program.

Modify the delay, flash again, and see how the behavior changes.

Load the Example ASCIITable (File-Examples-04.Communication-ASCIITable). Flash it. Open a serial terminal (Tools-Serial Monitor). Relate what is happening on the monitor to the code in the example. 

> It is time to learn about registers, ports, pins, bitwise operators, and reading schematics. Follow along with the lecture, then complete the worksheet.

<hr>

#### Working with Peripherals

Now that you understand registers, ports, pins, and bitwise logic, it is time to use an LED with your board. You need:

- breadboard
- LED
- Wire to connect a Metro GPIO pin to the breadboard.
- Wire to connect the Metro ground to the breadboard.
- Resistor to not "fry" the LED (220 ohm)

You need to make a circuit: 

GPIO -> breadboard pin -> resistor -> LED -> breadboard pin -> ground

see https://docs.arduino.cc/built-in-examples/basics/Blink/

Note that LEDs can be "backwards." You have to connect power (i.e. GPIO) to the anode, which is the long leg, and the ground to the cathode, which is the short leg. The resistor can be anywhere in the circuit (i.e. on either side of the LED). See https://makecode.adafruit.com/learnsystem/pins-tutorial/devices/led-connections.

Create a new sketch called myLED.ino (you can copy Blink.ino). Use the schematic to determine which pin to set as output. Toggle the on-board LED at 2Hz and the breadboard LED at 4Hz. Careful in how you "schedule" these tasks so they do not interfere with each other.

_Toggle_ means to change the state of the LED (from on to off or off to on). _Blink_ is to  turn the LED on then off. In other words, there is 1 blink for every 2 toggles. I try to be consistent in the requirements about which I mean. Please let me know if you are not sure what is meant in any of the project directions.

> Follow along in the lecture, then complete the worksheet.

<hr>

#### Command Line Compilation and Makefiles

The Arduino libraries are written in C. The .ino sketches are converted into a main function in a source file (which is a required file for all C programs). The way that C manages files and combines them into an executable during compilation is very different from how Java manages this. There is much to learn regarding C-compilation.

C-language Vocabulary:
- preprocessor directives (e.g. `#define` `#include`). These are requests to the compiler.
- header file (e.g. io.h). Contains the declaration of variables and functions (typically not the definition).
- source file (e.g. io.c or main.c). It might be a .c file that contains variable and function definitions. There must be a file that has the `main` function defined (by convention, it is defined in main.c).


First, get your hardware ready. Follow the directions in the avr toolchain set-up file: 
https://github.com/lars1050/csc461-f26/blob/main/hardware/avr-toolchain-setup.md

> It is time to learn about C compilation. Follow along with the lecture, then complete the worksheet.

At this point, you should be able to use the make command and the Makefile to compile then flash the Metro board.

<hr>

#### Important Things to know about your board.

**__Registers__**

x = A-F <br>
n = 0-7

- DDRx : Data Direction Register for Port "x"
- PORTx : Port Output for Port "x"
- PINx : Port Input for Port "x"
- PCICR : Pin Change Interrupt Control Register
- PCMSK0 : Pin Change Enable Mask for Interrupt 0

**__Pin/Bit Locations__**

- DDxn : Data Direction bit location for port "x" pin "n"
- PORTxn : Port Output location for port "x" pin "n"
- PINxn : Pin Output location for port "x" pin "n"
- PCINTn : Pin Change Interrupt Enable for pin "n"

sei() : function call to enable all interrupts
cli() : function call to disable all interrupts
