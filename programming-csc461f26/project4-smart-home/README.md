## CSC461 Intelligent Systems
## Project 4 : Smart Home

### Deliverables and Due Dates

- Worksheet Part I due end of class Thursday, Oct 15 (print and submit)
- Worksheet Part II due end of class Tuesday, Oct 20 (print and submit)
- Prototype Draft due end of class Tuesday, Oct 20 (submit via Gitlab)
- Implementation in sketch lighting.ino due Tuesday, Oct 27 beginning of class

You have been assigned to a group to work with to complete the Google drive worksheet. https://docs.google.com/document/d/16sXM4AEZVF_Iyu5blKrbalH72Ngct5maIZkI_gobjC0/edit?usp=sharing

For the implementation, you may work independently or as a group. If it makes it easier, you can use a repo to collaboratively develop the code.

**Prototype Draft Requirements**: The OCR1A and OCR1B match interrupts are implemented. The general framework for the code is set such that interrupts are releasing tasks and the main loop is watching for released tasks.


### Learning Objectives

When finished with this lab, students will be able to:

1. Set up a timer interrupt to control task timing.
1. Set up a PWM waveform to output on one of the general I/O pins.
1. Use a PIR Sensor to detect motion.
1. Use an analog ambient light sensor.
1. Use a ring buffer for collection and management.
1. Create a prototype of a smart home that combines motion sensing and ambient light sensing to control lighting based on time of day and current ambient lighting, as well as simulated communication with a networked device that interfaces with a web app.

<hr>

### Real-World Deliverable

One aspect of a smart home system is to control the lighting of each room as the owners move through the house. By controlling the lighting, it can improve safety and save electricity, which can have environmental impacts. Imagine the impact of doing this for a 50-story building in Manhattan!

To be truly smart about lighting control, the system needs to know which persons are entering and leaving rooms and their lighting preference for that room, which is likely to depend on both the time of day and the current ambient light. For example, when a person enters a room, there might be a preference for very dim lighting at 2am, medium lighting in the middle of the day when its raining, bright lights in the kitchen at night, or no lighting in the middle of a sunny day. The idea would be that the system would use facial recognition to know which person is entering a room and adjust the lighting according to their known (or learned) preferences.

And no smart home system would be complete without a web app for remote control and for system reporting.

### Prototype Deliverable 

There are a collection of tasks that need to be managed for this prototype.

1. Monitor motion in and out of the room using a PIR sensor that triggers an external interrupt when motion is detected.

1. Monitor ambient light conditions using an analog sensor. The sensor will be polled regularly and a sliding average of collected readings will be used as one criteria to base room lighting.

1. Control the brightness of an led using a hardware generated PWM waveform. The brightness will depend on if a person is in the room, the ambiant light, and the simulated time of day.

1. Simulated time keeper. 

1. Status report using the serial monitor (imagine this is the communication to the network for the web app).

### LED Lighting Control

Use timer/counter 2 to generate a PWM waveform to control an led attached to pin OC2B. Example code has been provided that varies the brightness of the led. The breadboard led must use the OC2B pin which corresponds to Port D pin 3. The control of the brightness through the PWM is managed through the hardware. You do not need an interrupt or turn the led on or off. In software, you will modify the duty cycle by setting the OCR2B register, which in turn modifies the waveform. Sample code has been provided.


### Scheduled Tasks and External Interrupt

**_Motion Sensing_** is the one task that will be triggered using an external interrupt. The PIR sensor generates a high signal when motion is detected. The signal stays high until its fixed timeout period. You can configure the external interrupt INT0 to trigger on the rising edge or on any pin change. This task will manage the state of whether or not someone is in the room. Sample code has been provided.

Tasks will be scheduled using timer interrupts to control the frequency of those tasks. There are 2 primary ways to use timer interrupts to perform tasks. The first is to complete the task within the interrupt. This is appropriate when there is very little computation for the task. The second is to "release" a task by setting a flag in the interrupt, then return control to the main loop. The main loop continuously checks for released tasks, and when released, completes them in the background of all other interrupts. When the task is complete, the release flag is reset.

**Implementation**: set Timer/Counter 1 to interrupt on the match to both OCR1A and OCR1B. Use CTC mode with OCR1A as the top value. The OCR1A value should be set so that the period of the interrupt is 1000ms. Also, set the OCR1B at half the value of OCR1A and enable the match interrupt. Note that the OCR1B interrupt will also trigger every 1000ms.

**_Record Ambiant Light_**. Use the OCR1B match interrupt with a 1000ms period (i.e. 1Hz) to complete this task. In the interrupt, read the light sensor and save the results in a global variable, then set a flag to release the Average Light task that maintains a windowed average of light readings. _For now, set this as a random number generator between 0 and 1000 -- 0 means it is dark outside and 1000 means it is very bright. You might want to limit the range based on the time of day. Once you learn how to use a photocell sensor, you can get an actual reading._

**_Average Estimate Ambiant Light_**. Released through a flag that is set when a new light reading has been obtained. The task maintains an average of the most recent 5 light readings using a ring buffer. To maintain the average, remove the oldest reading from the buffer and subtract that value/5 from the average. Add the current reading to the buffer and add current/5 to the average. HINT: in the setup, I would fill the buffer with readings and calculate the average. When you get a new reading, you can then always assume the buffer is full. Ring buffer code has been provided.

**_Lighting Control_**. Use the OCR1A match interrupt with a 1000ms period to release the task. The task in the main loop will make adjustments to the duty cycle of the PWM, as appropriate. If no one is in the room, the light is off. If someone is in the room, use the ambiant light average and the time of day to determine what the brightness should be. The easiest thing is probably a kind of look-up table that results in off, very dim, dim, medium, and bright, for example. However, you are free to create your own scheme about how to use environmental information to adjust lighting. 

**_Time Keeping_**. Use the OCR1A match interrupt to also update a global time every 1 second. Equate 1 second to 1 hour of the day. Be sure to modulo 24. _If we did need the time, we would likely have a real-time clock peripheral added to the system that keeps "human" time, or maybe we could get it from the web the system were networked._

**_Status Report_**. Use the OCR1A match interrupt to also release a status report every 5 seconds (i.e. every 5th time the interrupt triggers) by setting a flag. In the main loop, the task prints information to the serial monitor including time of day, ambient light average, and current room lighting condition.  

### Complete the Prototype

Create the complete prototyp in a new sketch lighting.ino. The draft is due on Tuesday, October 20. The draft focuses on scheduling and implementing the timer/counter interrupts.

In the second week of this project, you will learn about photocell sensors, which are analog and require ADC (analog to digital conversion) and the PIR motion sensor. This will complete all the technical skills that you will need for this project.

> Please heavily comment your code (with meaningful information).












