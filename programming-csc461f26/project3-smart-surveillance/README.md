## CSC461 Intelligent Systems
## Project 3 : Smart Surveillance

Deliverables and Due Dates (submit via Gitlab)
- State Machine Due Thursday, Oct 8 start of class
- Prototype Due Tuesday, Oct 13 start of class

### Learning Objectives

When finished with this lab, students will be able to:

1. Learn about external interrupts and how to enable.
1. Learn about pin change interrupts and how to enable.
1. Understand flow-of-control with respect to interrupts and ISRs.
1. Use a state machine to model the behavior of the system.
1. Create a prototype of smart surveillance that combines buttons, simulated communication with other devices, and blinking LEDs.

**It is important that you build this up one piece at a time. Get your button interrupt working first, independent of anything else. Then work on the PCINT signals from the second camera, independent of button presses. Finally, clean up the external interrupt to only watch for INT0. Now you are ready to put the pieces together and build the whole system.**

<hr>

### Real-World Deliverable

A surveillance system uses both cameras and a key card reader to detect persons inside a room that is highly classified. Due to being highly classified, any information obtained from the room needs to be kept local and not on any network. To monitor the room, there are multiple very small cameras distributed throughout the room, each capable of local, on-board, face recognition of authorized users only. These cameras are locally networked and can communicate limited information with each other. Each camera is attached to a flash drive that can save at most 2 minutes of video. 

Upon entry with a key card, the camera system turns on and all cameras take a snapshot and engage their face recognition. If the person identified matches the key card identity, it communicates the match to all other cameras, makes a note of their presence, and turns off the camera system. If none of the cameras recognize the person, building security is alerted, and each camera records 2 minutes of video, in series. For example, camera 1 records the first 2 minutes, camera 2 the next 2 minutes, and so on. This continues until the person leaves the room or they are out of space to store the video.

When an authorized person leaves the room (based on the door opening), the cameras are again used to make sure no one has entered the room as the other person exited. All cameras report an "all-clear" if no one is in the room, otherwise they follow the protocol described above regarding unauthorized entry. When an unauthorized person leaves the room, the camera system alerts security, checks the room again to ensure it is empty, and turns off the cameras.

### Prototype Deliverable 

- A button is pressed to indicate entry into a room, which activates an external interrupt. The interrupt turns on an led on the breadboard to indicate an image has been captured and it is about to engage in facial recognition. 

- After a person has entered the room and control returns to the main loop, simulate the system comparing the image to all authorized users in the database. It should take about 1 second per image and there are 25 authorized users.

	- At any point if one of the other cameras confirms the user, the system will wait for the authorized user to exit.
	- If the system matches to any authorized user, the system will wait for the authorized user to exit.
	- If the system does not match any of the authorized user and no signal was received from the other cameras, the system alerts security and goes into unauthorized user monitoring.

- When an authorized user is in the room, a button activation will indicate the person is leaving the room and the system should reset.

- When an unauthorized user is in the room, the system waits for the external interrupt to signal to indicate it is time to record for 2 minutes (_the other modules will record and then send a signal to the next module_). 
	- When the system receives the signal to start recording, it records for 2 minutes. After 2 minutes it waits for the button press to indicate the person has left the room.
	
- When the button is pressed while someone is in the room, it indicates that they are leaving the room and the system should reset.

_This all has to be simulated since we do not have facial recognition nor a local network of devices._

#### Status and the LED:

- OFF. No one is in the room and it is waiting for someone to enter.
- SOLID ON. Someone has entered the room and it is in the process of facial recognition.
- BLINKING at 0.5 Hz (1 toggle per second). Authorized user is in the room, waiting for them to leave.
- BLINKING at 2 HZ. Unauthorized user is in the room. Not actively recording camera feed. Either waiting for signal to start recording or already recorded and waiting for person to exit.
- BLINKING at 5 HZ. Actively recording camera feed.



<hr>

### State Machine

Create a state machine drawing of the system from the perspective of a single monitoring module (i.e. your Metro) that is in communication with other monitoring modules.

Circles represent states of the system. It is useful to describe the status of variables or leds within the state.

Arcs represent transitions between states. Along the arc, write what actions result in the transition.

You can draw this on paper or your ipad or you can use a tool to make it digitally. Please make it professional looking and legible.

When you write the code, the state machine structure should be obvious within the code structure. You probably want to use a variable _state_ to track what is happening in the system.

<hr>

### Status and Debugging LEDs

Use the on-board LED as a way to monitor interrupts and debug the system. Notice how it was used during external interrupts and pin change interrupts to indicate the ISR was entered. Also, it provided a "heartbeat" in the main loop, and it was flashed in initialization to show when the board is reset.

> Connect another breadboard LED to PORT D pin 7

The status led is for indicating which state the system is currently in.

<hr>

### Implementing the Button with an Interrupt

Make a new sketch button\_operation.ino ...

Again create a buttons.h and buttons.cpp file. However, this time it will be implemented using an external interrupt. An example of using an external interrupt has been provided for you. You should only have to make small adjustments to get the button working.

> Please install your button on PORTD pin 3. This corresponds to External Interrupt 1 (i.e. INT1). Do not forget to activate the pull-up resistor on the button pin.

Set the interrupt to trigger on the signal associated with the release of the button. When the button is released, use whatever makes sense to you to test if it is working.

<hr>

### Implementing the Pin Change Interrupt for the 2nd camera

In the sketch pcint\_coms.ino ...

Modify the provided files so that it can receive a signal from either camera0 (pin 0) or camera1 (pin 1). When you test this, test for only 1 pin at a time, otherwise you will not know what is going on.

<hr>

### Implementing the External Interrupt for INT0

Clean up the sketch ext\_int.ino and its files so that it only pays attention to INT0. This will be used to signal when it is time to record the feed.

More on this for Thursday ...

<hr>

### Complete the Prototype

Create a new sketch surveillance.ino.

More on this for Thursday ...




