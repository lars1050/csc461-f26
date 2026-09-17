### CSC461 Intelligent Systems
### Project 1 : Smart Parking

Due by the start of class Tuesday, September 22 <br>
- Arduino sketch folders proximity\_arduino and proximity\_avr.
- Arduino sketch folder with proximity\_avr\_reuse.ino, sonic.h, and sonic.cpp
- CDIO framework (pdf) <br>

Submit via GitLab

Worksheets are handed in at the end of the class

#### Learning Objectives

When finished with this lab, students will have/be able to:

1. Connect the Ultrasonic Distance Sensor SR04 to the Metro.
1. Read the datasheet for the sensor and understand how it corresponds to the code.
1. Use the Arduino library to operate the Ultrasonic sensor.
1. Create a .h and .cpp file that integrates with the Arduino IDE
1. Identify important aspects of engineering a smart parking system.
1. Create a prototype of smart parking that combines distance sensor readings
with an LED for occupancy status.

#### Useful Vocabulary

- peripheral, ultrasonic distance sensor 
- preprocessor directives (e.g. #define)
- serial communication
- Register, Port, and Pin
- bit, bitmask, clear, set
- VCC, power, ground
- COM or device port 
- schematic, datasheet
- Hertz, frequency

<hr>

#### Smart Parking

**Real-World Deliverable**: A parking ramp management company would like to create a “smart parking” system for an existing facility. For each parking spot, there will be an indicator that the space is either free or occupied. This should be easy for drivers to see from afar so that they can quickly determine where to park. Additionally, the system should track occupancy overall, so that it is known how many parking spots are currently available. The ramp is an outdoor parking facility for a shopping mall situated in the Midwest with extreme weather conditions in the winter.

**Prototype Deliverable**: A system that monitors a “space” for occupancy. When the space is occupied, the red LED should be on (i.e. space is not available), otherwise the green LED should be on (see note below). It will be important to think about when to change the LED status to occupied. It should not change any time the proximity sensor detects a change or something close by. Determine how your prototype corresponds to the actual ramp parking spot, and (approximately) design your system accordingly.

> If you would rather use the on-board LED, have the LED solid on when the space is occupied and blinking when it is free.

#### Conceive - Design - Implement - Operate (CDIO) Learning Framework

In engineering education, the CDIO framework is used as a tool for students to consider the important aspects of an engineering problem. First, in _conceive_, the team considers the stakeholders and important requirements of the system. In _design_, the team considers different solutions to the problem and analyzes the quality of those solutions. In _implement_, the team creates their solution, which is often a co-development of hardware and software. The final step, _operate_ involves the installation, testing, and maintenance of the system.

An example of this process has been provided on Google. 

https://docs.google.com/document/d/1RAQHYL2bbbHzJfccfnpR5xzaPUQS8jZEnp6TkkmS2BM/edit?usp=sharing

> As part of this lab, you will add a sensor analysis and some test cases to the CDIO document. As well as your thoughts on how to configure the prototype and use the readings (data) to intelligently detect the presence of a "vehicle." When complete, create a pdf of the final document and add it to your assignment folder to upload to Gitlab.

You are welcome (and encouraged) to work on the CDIO framework with classmates. Please do let me know who you worked with by adding names to your document. You are also welcome to use AI to learn more about the situation, but not to thoughtlessly complete your assignment. 

We will use this framework in future assignments, and it will be part of your final team project.

<hr>

#### Connecting the Ultrasonic Sensor

Connect your Ultrasonic sensor to the Metro using the breadboard or connecting direct and placing it on the table (your choice). Directions can be found here:

https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1

Datasheet: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf

There are 2 versions of the code provided on GitHub. One version uses the Arduino libraries and the other uses the avr libraries. Notice the pin choices in the code. If you use different pins, then you will need to change the code.

**Create 2 folders for the 2 sketches. Make sure both are functional.**

Take the time to get your ultrasonic sensor functioning. To see the readings while the system is operating, go to Tools > Serial Monitor.

> Look at the datasheet for the ultrasonic sensor to understand the code. Then answer the relevant questions on your worksheet.

#### Creating Reusable Code

Last week, you used the avr-gcc and a makefile to compile code using the avr libraries. You will learn more about programming in C by transforming the proximity_avr sketch to use a header and source file. You could compile this using avr-gcc, but we don't have serial communication working except through the Arduino IDE. Therefore, we will continue to use the IDE for this assignment.

Recall that C and C++ manage code reuse by separating the declarations and definitions into header (.h) and source (.c or .cpp) files. You will create this same file structure so that if you use the sensor in another situation, you simply add those files, rather than copy and paste code.

1. Copy the folder proximity_avr into a new "Arduino" folder proximity_avr_reuse.
2. The file sonic.h has been provided for you. Copy it into this new folder.
3. Create the file sonic.cpp by copying the proximity_avr.ion file using your file explorer (save it as sonic.cpp). Arduino compiles code with C++, so the file name has to have the .cpp extension. C++ is backwards compatible, so that any C code can be compiled with a C++ compiler (but not the other way round).
4. Open this new Sketch in Arduino and notice the tabs for the other files.
5. Edit the files so they all work together.

> There are questions on the worksheet that both help you with this and assess your understanding of the code.

#### Complete the Prototype

Now that you have all the pieces, create a smart parking prototype that indicates when a space is either occupied or free. This should be similar to a parking space in that you want a clear indication when a _vehicle_ is in the space, not when something briefly passes through or you get a noisy reading. The LED should not change every time there is a change in the sensor reading. Maybe you can use a data structure to calculate a running average or track the time that the sensor has been consistently in a certain range. 

> I realize that you have not been given a means to mount the sensor, so it can be difficult to get it working as you imagined. I am much more interested in how you approach this problem than in the success of the implementation (i.e. if your system is working the way you imagine it should work).

**In the CDIO framework, provide a reasonable justification for how you would intelligently control the LED signal for this prototype (not the real-world version).**

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
