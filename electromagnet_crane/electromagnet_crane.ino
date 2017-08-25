/**
 * Electromagnet Crane
 *
 * Controls a stationary electromagnetic crane using a PS3
 * controller.
 *
 * Required libraries:
 * - Servo
 * - USB Host Shield 2.0 
 * https://github.com/felis/USB_Host_Shield_2.0
 *
 * Components:
 * - Arduino Uno
 * - USB Host Shield
 * - 2x MG995 180 degrees
 * - MG995 360 degrees
 * - PS3 controller
 * - IRF540N MOSFET
 *
 * Created 2017-08-20
 * By Carl Marquez
 * Modified 2017-08-25
 * By Carl Marquez
 */
#include <PS3USB.h>
#include <Servo.h>

int8_t PAN_PIN = 3;
int8_t TILT_PIN = 4;
int8_t REEL_PIN = 5;
int8_t MAGNET_PIN = 6;

USB ps3Usb;
PS3USB ps3(&ps3Usb);

Servo pan;
Servo tilt;
Servo reel;
int8_t panAngle; // Horizontal turn
int8_t tiltAngle; // Vertical tilt, limited to 70 degrees

void setup() {
  if (ps3Usb.Init() == -1) while (true);

  pinMode(MAGNET_PIN, OUTPUT);

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
    // Moving the left joystick left/right controls pan, 
    // i.e., rotation parallel to the horizontal plane.
    if (ps3.getAnalogHat(LeftHatX) < 50) {
      panAngle += (int8_t)(panAngle < 90);
    }
    if (ps3.getAnalogHat(LeftHatX) > 205) {
      panAngle -= (int8_t)(panAngle > -90);
    }

    // Moving the left joystick up/down controls tilt, i.e., 
    // rotation perpendicular to the horizontal plane.
    if (ps3.getAnalogHat(LeftHatY) < 50) {
      tiltAngle += (int8_t)(tiltAngle < -20);
    }
    if (ps3.getAnalogHat(LeftHatY) > 205) {
      tiltAngle -= (int8_t)(tiltAngle > -90);
    }

    // Pressing up/down reels the wire out/in.
    if (ps3.getButtonPress(UP)) {
      reel.writeMicroseconds(1000);
    }
    else if (ps3.getButtonPress(DOWN)) {
      reel.writeMicroseconds(2000);
    }
    else reel.writeMicroseconds(1500);

    // Holding the square/L1 button down activates the 
    // electromagnet.
    if (ps3.getButtonPress(SQUARE)||ps3.getButtonPress(L1)) {
      digitalWrite(MAGNET_PIN, HIGH);
    }
    else digitalWrite(MAGNET_PIN, LOW);
    
    pan.writeMicroseconds(map(panAngle,-90,90,650,2300));
    tilt.writeMicroseconds(map(tiltAngle,-90,90,550,2200));
  }
  delay(20);
}
