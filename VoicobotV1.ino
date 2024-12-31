#include <ESP32Servo.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial ESP_BT;

Servo servo_M1;
Servo servo_M2;
Servo servo_M3;

int servo_M1_pin = 17;   //P17 M1
int servo_M2_pin = 18;   //P18 M2
int servo_M3_pin = 19;   //P19 M3

// Servo motor speed settings (values between 0 and 180)
const int stopSpeed = 90;  // Stop position for servo
const int forwardSpeed = 120;
const int backwardSpeed = 60;

void setup() {
  Serial.begin(115200);

  ESP_BT.begin("VoicoBot"); // Name of your Bluetooth interface -> will show up on your phone

  Serial.println("The device started, now you can pair it with bluetooth!");

  servo_M1.attach(servo_M1_pin);
  servo_M2.attach(servo_M2_pin);
  servo_M3.attach(servo_M3_pin);

  stopAllMotors(); // Stop all motors initially
}

void loop() {
  if (ESP_BT.available()) {
    char command = ESP_BT.read();
    Serial.print("Received command: ");
    Serial.println(command);

    switch (command) {
      case 'F':
        moveForward();
        break;
      case 'B':
        moveBackward();
        break;
      case 'R':
        moveRight();
        break;
      case 'L':
        moveLeft();
        break;
      default:
        stopAllMotors();
        break;
    }
  }
}

void moveForward() {
  servo_M1.write(stopSpeed);  // Front motor moves forward
  servo_M2.write(forwardSpeed); // Rear left motor moves backward
  servo_M3.write(backwardSpeed);  // Rear right motor moves forward
}

void moveBackward() {
  servo_M1.write(stopSpeed); // Front motor moves backward
  servo_M2.write(backwardSpeed);  // Rear left motor moves forward
  servo_M3.write(forwardSpeed); // Rear right motor moves backward
}

void moveRight() {
  servo_M1.write(backwardSpeed);     // Front motor stops
  servo_M2.write(backwardSpeed); // Rear left motor moves backward
  servo_M3.write(backwardSpeed);  // Rear right motor moves forward
}

void moveLeft() {
  servo_M1.write(forwardSpeed);     // Front motor stops
  servo_M2.write(forwardSpeed);  // Rear left motor moves forward
  servo_M3.write(forwardSpeed); // Rear right motor moves backward
}

void stopAllMotors() {
  servo_M1.write(stopSpeed);
  servo_M2.write(stopSpeed);
  servo_M3.write(stopSpeed);
}
