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
 * - PS3 controller
 *
 * Created 2017-08-20
 * By Carl Marquez
 * Modified 2017-08-20
 * By Carl Marquez
 */
 
#include <PS3USB.h>
#include <Servo.h>

USB ps3Usb;
PS3USB ps3(&ps3Usb);

Servo pan;
Servo tilt;
int8_t panAngle; // Horizontal turn
int8_t tiltAngle; // Vertical tilt, limited to 70 degrees

void setup() {
  Serial.begin(115200);
  if (ps3Usb.Init() == -1) {
    Serial.println("USB Host Shield did not start");
    while (true);
  }
  Serial.println("PS3 USB Library Started");

  pan.attach(3);
  panAngle = 0;
  pan.writeMicroseconds(map(panAngle, -90, 90, 650, 2300));
  
  tilt.attach(4);
  tiltAngle = -45;
  tilt.writeMicroseconds(map(tiltAngle, -90, 90, 550, 2250));
}

void loop() {
  ps3Usb.Task();
  if (ps3.PS3Connected) {
    // Moving the joystick left / right controls pan
    if (ps3.getAnalogHat(LeftHatX) < 50) {
      panAngle += (int8_t)(panAngle < 90);
      Serial.println("LEFT");
    }
    if (ps3.getAnalogHat(LeftHatX) > 205) {
      panAngle -= (int8_t)(panAngle > -90);
      Serial.println("RIGHT");
    }

    // Moving the joystick up / down controls tilt
    if (ps3.getAnalogHat(LeftHatY) < 50) {
      tiltAngle += (int8_t)(tiltAngle < -20);
      Serial.println("UP");
    }
    if (ps3.getAnalogHat(LeftHatY) > 205) {
      tiltAngle -= (int8_t)(tiltAngle > -90);
      Serial.println("DOWN");
    }
    
    pan.writeMicroseconds(map(panAngle,-90,90,650,2300));
    tilt.writeMicroseconds(map(tiltAngle,-90,90,550,2200));
    Serial.println(tiltAngle + 90);
  }
  _delay_ms(20);
}
