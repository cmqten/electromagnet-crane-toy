# Electromagnet Crane Toy

A stationary toy crane with an electromagnet for lifting light metallic objects.

## Longer Description

This toy crane has three main components: crane, electromagnet, and microcontroller. The crane consists of some wood for the base and the arm, and three servos for controlling pan, tilt, and reeling the rope in/out. Attached at the end of the rope is the electromagnet. The electromagnet is simply a screw with some magnet wire coiled around it about 500 times, and a transistor that activates and deactivates it. Finally, the microcontroller controls both the crane and the electromagnet by sending signals to the servos and the transistor. Each main component should have its own power supply as they all draw significant amounts of current.

## [Demo Video](https://vid.me/uzldo)

## Required Libraries
- Servo library (comes with Arduino IDE)
- [USB Host Shield Library 2.0](https://github.com/felis/USB_Host_Shield_2.0)

## General components
- Wood for the base and crane arm
- Wood screws
- Wood glue
- Nails
- M3 screws/nuts
- M3 standoffs 
- Servo horns for mounting and attaching servos together
- Servo brackets for mounting and attaching servos together
- Rope for reeling the electromagnet in/out
- Spool for reeling the electromagnet in/out (This project uses servo horns and M3 standoffs to create a makeshift spool)

## Electrical and Mechanical Components
- Jumper wires
- Stranded wires for connecting components to their respective power supplies, if necessary

### Crane
- 2x 180 degree servos (This project uses the MG995)
- 1x continuous rotation servo (This project uses the MG995)
- 3x 470uF electrolytic capacitors
- Power supply (This project uses a battery pack with a modified USB wire)

### Electromagnet
- Iron core (This project uses a metallic screw)
- Magnet wire
- Transistor (This project uses an IRF520N MOSFET)
- Power supply (This project uses two AA batteries)

### Microcontroller
- Arduino Uno
- USB Host Shield
- PS3 controller
- Power supply (This project uses power from the USB cable)

## Images
<img src=./images/crane_left.jpg width=600/>

<img src=./images/crane_right.jpg width=600/>

<img src=./images/controller.jpg width=600/>


