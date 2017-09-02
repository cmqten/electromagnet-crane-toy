/**
 * Electromagnet Crane
 *
 * Controls a stationary electromagnetic crane using a PS3
 * controller.
 * 
 * Controls:
 * - Left joystick, right joystick : pan and tilt
 * - Up, Triangle : reel in
 * - Down, X : reel out
 * - Left, Right, Square, Circle : Activate electromagnet
 * - L1, R1 : Toggle electromagnet
 * 
 * The electromagnet can be activated in multiple ways:
 * 
 * Hold/Release mode:
 * If any of Left, Right, Square, or Circle are held down,
 * the electromagnet is activated. Once all of them are 
 * released, the electromagnet is deactivated.
 * 
 * Toggle mode: 
 * If L1 or R1 are pressed, the electromagnet is activated.
 * It stays activated until L1 or R1 are pressed again, or 
 * if the electromagnet times out, i.e., no user input has
 * been received for a certain amount of time, to prevent
 * overheating in case the user leaves the electromagnet 
 * activated by accident. While in toggle mode, hold/release 
 * has no effect.
 * 
 * By default, the electromagnet starts in hold/release mode.
 * To switch to toggle mode, press L1 or R1. To go back to 
 * hold/release mode, press L1 or R1 again, or wait for the 
 * electromagnet to time out.
 *
 * Required libraries:
 * - Servo
 * - USB Host Shield 2.0 
 * https://github.com/felis/USB_Host_Shield_2.0
 *
 * General components:
 * - Arduino Uno
 * - USB Host Shield
 * - PS3 controller
 * - Arduino Uno power supply
 * - Jumper wires
 * 
 * Crane components:
 * - 2x MG995 180 degrees
 * - MG995 360 degrees
 * - 3x 16V 470uF electrolytic capacitors
 * - Servo power supply
 * - Rope
 * - M3 standoffs/nuts
 * - Servo horns
 * - Servo brackets
 * 
 * Electromagnet components:
 * - Iron core
 * - Magnet wire
 * - IRF520N MOSFET
 * - Electromagnet power supply
 *
 * Created 2017-08-20
 * By Carl Marquez
 * Modified 2017-09-01
 * By Carl Marquez
 */
#include <PS3BT.h>
#include <PS3USB.h>
#include <Servo.h>
#include <usbhub.h>

// Define PS3_BTD if bluetooth is going to be used to connect
// to the PS3 controller. Comment the #define line out if USB
// is going to be used.
// #define PS3_BTD

int8_t PAN_PIN = 3;
int8_t TILT_PIN = 4;
int8_t REEL_PIN = 5;
int8_t MAGNET_PIN = 6;
int8_t HOLD_RELEASE = 0;
int8_t TOGGLE = 1;

USB ps3Usb;

#ifdef PS3_BTD
BTD ps3Btd(&ps3Usb);
// Second to seventh arguments are the bluetooth's MAC 
// address. Replace accordingly.
PS3BT ps3(&ps3Btd, 0x00, 0x1A, 0x7D, 0xDA, 0x71, 0x0C);

#else
PS3USB ps3(&ps3Usb);
#endif

Servo pan; // Rotation perpendicular to horizontal plane
Servo tilt; // Rotation parallel to horizontal plane
Servo reel; // Reels wire in/out
int8_t panAngle; // Horizontal turn
int8_t tiltAngle; // Vertical tilt, limited to 70 degrees
int8_t magnetMode; // Hold/release or toggle mode
unsigned long timeout; // Last input time, for magnet timeout

void setup() {
  if (ps3Usb.Init() == -1) while (true);

  // Initializes the electromagnet as deactivated.
  magnetMode = HOLD_RELEASE;
  pinMode(MAGNET_PIN, OUTPUT);
  digitalWrite(MAGNET_PIN, LOW);
  timeout = millis();

  // Initializes servos and moves crane to initial position;
  // Facing directly forward and tilted 45 degrees from the
  // horizontal plane.
  pan.attach(PAN_PIN);
  panAngle = 0;
  pan.writeMicroseconds(map(panAngle, -90, 90, 650, 2300));
  
  tilt.attach(TILT_PIN);
  tiltAngle = -45;
  tilt.writeMicroseconds(map(tiltAngle, -90, 90, 550, 2250));

  reel.attach(REEL_PIN);
}

void loop() {
  ps3Usb.Task();
  
  if (ps3.PS3Connected) {
    // Moving any of the joysticks left/right controls pan, 
    // i.e., rotation parallel to the horizontal plane.
    if (ps3.getAnalogHat(LeftHatX) < 50 || 
      ps3.getAnalogHat(RightHatX) < 50) {
      panAngle += (int8_t)(panAngle < 90);
      timeout = millis();
    }
    if (ps3.getAnalogHat(LeftHatX) > 205 || 
      ps3.getAnalogHat(RightHatX) > 205) {
      panAngle -= (int8_t)(panAngle > -90);
      timeout = millis();
    }

    // Moving any of the joysticks up/down controls tilt, 
    // i.e., rotation perpendicular to the horizontal plane.
    if (ps3.getAnalogHat(LeftHatY) < 50 || 
      ps3.getAnalogHat(RightHatY) < 50) {
      tiltAngle += (int8_t)(tiltAngle < -20);
      timeout = millis();
    }
    if (ps3.getAnalogHat(LeftHatY) > 205 || 
      ps3.getAnalogHat(RightHatY) > 205) {
      tiltAngle -= (int8_t)(tiltAngle > -90);
      timeout = millis();
    }

    // Pressing up/down or triangle/x reels the wire out/in.
    if (ps3.getButtonPress(UP) || 
      ps3.getButtonPress(TRIANGLE)) {
      reel.writeMicroseconds(1000);
      timeout = millis();
    }
    else if (ps3.getButtonPress(DOWN) || 
      ps3.getButtonPress(CROSS)) {
      reel.writeMicroseconds(2000);
      timeout = millis();
    }
    else reel.writeMicroseconds(1500);
    
    if (magnetMode == HOLD_RELEASE) {
      // Hold and release mode. Left, right, square, and 
      // circle buttons activate the electromagnet. L1 and
      // R1 switch to toggle mode. 
      if (ps3.getButtonClick(L1) || ps3.getButtonClick(R1)) {
        magnetMode = TOGGLE;
        digitalWrite(MAGNET_PIN, HIGH);
        timeout = millis();
      }
      else if (ps3.getButtonPress(LEFT) ||
        ps3.getButtonPress(RIGHT) || 
        ps3.getButtonPress(SQUARE) || 
        ps3.getButtonPress(CIRCLE)) {
        digitalWrite(MAGNET_PIN, HIGH);
      }
      else digitalWrite(MAGNET_PIN, LOW);
    }
    else {
      // Toggle mode. Turns off electromagnet and goes back 
      // to hold and release mode if L1 or R1 are pressed, or
      // 10 seconds has passed without a single user input.
      if (ps3.getButtonClick(L1) || ps3.getButtonClick(R1) ||
        millis() - timeout >= 10000) { 
        magnetMode = HOLD_RELEASE;
        digitalWrite(MAGNET_PIN, LOW);
      }
    }
    
    pan.writeMicroseconds(map(panAngle,-90,90,650,2300));
    tilt.writeMicroseconds(map(tiltAngle,-90,90,550,2200));
  }
  delay(20);
}
