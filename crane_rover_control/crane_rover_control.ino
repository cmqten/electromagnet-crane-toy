/**
 * Crane Rover Controller
 * 
 * Controls the rover on which the electromagnetic crane is
 * mounted. Rover can go forward, back, turn left, and turn
 * right. Created for the attiny85 using David Mellis' attiny
 * core and Ken Shirriff's IR remote library.
 * 
 * This sketch contains pure AVR C code to fit in the small
 * flash memory space of the attiny85. Pure AVR C code will 
 * be labeled as such to distinguish from Arduino code. The 
 * recommended clock source is the internal 8 mhz clock.
 * 
 * Required cores/libraries:
 * - attiny core: https://github.com/damellis/attiny
 * - IR remote: https://github.com/z3t0/Arduino-IRremote
 * 
 * Components:
 * - attiny85
 * - IR receiver
 * - IR remote
 * - L293D dual h-bridge IC
 * - 2x DC motors
 * 
 * Created 2017-07-24
 * By Carl Marquez
 * Modified 2017-07-24
 * By Carl Marquez
 */
 
#include <IRremote.h>

// Mapping to IR remote button codes + ir pin location
const unsigned long MOVE_FORWARD = 0xFF18E7; // 2
const unsigned long MOVE_BACKWARD = 0xFF4AB5; // 8
const unsigned long TURN_LEFT = 0xFF10EF; // 4
const unsigned long TURN_RIGHT = 0xFF5AA5;  // 6
const unsigned long CONTINUE = 0xFFFFFFFF;// hold last button
const byte irPin = 5;

IRrecv ir(irPin);
decode_results results;

unsigned long timer; // Time last received an input

void setup() {
  DDRB |= 0x1E; // Sets pins 1, 2, 3, 4 as output, AVR 
  PORTB &= (~0x1E); // Sets pins 1, 2, 3, 4 to low, AVR
  ir.enableIRIn();
  timer = millis();
}

void loop() {
  if (ir.decode(&results)) {
    switch (results.value) {
      case MOVE_FORWARD:
        PORTB &= (~0x1E); // Sets pins 1, 2, 3, 4 to low, AVR
        PORTB |= 0x14; // Sets pins 2, 4 to high, AVR
        timer = millis();
        break;
        
      case MOVE_BACKWARD:
        PORTB &= (~0x1E); // Sets pins 1, 2, 3, 4 to low, AVR
        PORTB |= 0x14; // Sets pins 2, 4 to high, AVR
        timer = millis();
        break;
        
      case TURN_LEFT:
        PORTB &= (~0x1E); // Sets pins 1, 2, 3, 4 to low, AVR
        PORTB |= 0x14; // Sets pins 2, 4 to high, AVR
        timer = millis();
        break;
        
      case TURN_RIGHT:
        PORTB &= (~0x1E); // Sets pins 1, 2, 3, 4 to low, AVR
        PORTB |= 0x14; // Sets pins 2, 4 to high, AVR
        timer = millis();
        break;
        
      case CONTINUE:
        timer = millis();
        break;

      default: break;
    }
  }
  else if (millis() - timer > 150)
    PORTB &= (~0x1E); // Sets pins 1, 2, 3, 4, to low AVR
}
