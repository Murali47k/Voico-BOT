
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C // Typically eBay OLED's
//#define i2c_Address 0x3D // Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // QT-PY / XIAO

// Create an SH1106 display object connected to I2C
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


Servo servo1;  
Servo servo2;
Servo servo3;

// Replace with your network credentials
const char* ssid = "realme 11 Pro+ 5G";
const char* password = "987654321";

// HTTP server instance
WebServer server(80);

// Command handler
void handleCommand() {
  String command = server.arg("command"); // Get command from request
  Serial.println("Received command: " + command);

  // Perform actions based on the command
  if (command == "forward") {
    moveForward();
  } else if (command == "backward") {
    moveBackward();
  } else if (command == "left") {
    turnLeft();
  } else if (command == "right") {
    turnRight();
  } else if (command == "stop") {
    stopMovement();
  } else {
    Serial.println("Unknown command");
  }

  // Add CORS header and send response
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Command received and executed");
}

// Handle CORS preflight requests
void handleCors() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204); // No Content
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  Serial.println("ESP32 IP address: " + WiFi.localIP().toString());
}
void drawEyes(int width, int height, int leftEyeX, int rightEyeX) {
  display.clearDisplay();

  // Draw the left eye
  int leftEyeY = 20;
  display.fillRect(leftEyeX, leftEyeY, width, height, SH110X_WHITE);

  // Draw the right eye
  int rightEyeY = 20;
  display.fillRect(rightEyeX, rightEyeY, width, height, SH110X_WHITE);

  display.display();
}

// Function to animate eyes moving to the left
void eyesLeft() {
  for (int leftEyeX = 20, rightEyeX = 80; leftEyeX >= 5; --leftEyeX, --rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(500); // Pause for 5 seconds after the movement
}

void eyesLeftback() {
  for (int leftEyeX = 5, rightEyeX = 65; leftEyeX <= 20; ++leftEyeX, ++rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(500); // Pause for 1 second after the movement
}

// Function to animate eyes moving to the right
void eyesRight() {
  for (int leftEyeX = 20, rightEyeX = 80; leftEyeX <= 35; ++leftEyeX, ++rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(500); // Pause for 5 seconds after the movement
}

void eyesRightback() {
  for (int leftEyeX = 35, rightEyeX = 95; leftEyeX >= 20; --leftEyeX, --rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(500); // Pause for 1 second after the movement
}
void setup() {
  Serial.begin(115200);
  initWiFi();

  // Handle CORS preflight requests
  server.on("/command", HTTP_OPTIONS, handleCors);

  // Handle GET requests
  server.on("/command", HTTP_GET, handleCommand);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");

  servo1.attach(16);  
  servo2.attach(17); 
  servo3.attach(18); 

  // Initialize the display
  if (!display.begin(i2c_Address)) {
    Serial.println(F("SH1106 allocation failed"));
    for (;;);
  }

  display.clearDisplay();

  // Draw initial eyes
  drawEyes(30, 30, 20, 80);
}

// Function to move forward
void moveForward() {
  Serial.println("Action: Moving forward");
  servo1.write(120); // Adjust values for appropriate speed/direction
  servo2.write(60);
  servo3.write(90);  // Neutral for servo3
}

// Function to move backward
void moveBackward() {
  Serial.println("Action: Moving backward");
  servo1.write(60);  // Reverse direction
  servo2.write(120); // Reverse direction
  servo3.write(90);  // Neutral for servo3
}

// Function to turn left
void turnLeft() {
  Serial.println("Action: Turning left");
  eyesLeft();
  servo1.write(90);  // Neutral for servo1
  servo2.write(120); // Spin in one direction
  servo3.write(60);  // Spin in the opposite direction
  eyesLeftback();

}

// Function to turn right
void turnRight() {
  Serial.println("Action: Turning right");
  eyesRight();
  servo1.write(90);  // Neutral for servo1
  servo2.write(60);  // Spin in one direction
  servo3.write(120); // Spin in the opposite direction
  eyesRightback();

}

// Function to stop movement
void stopMovement() {
  Serial.println("Action: Stopping movement");
  servo1.write(90); // Stop
  servo2.write(90); // Stop
  servo3.write(90); // Stop
}

void loop() {
  server.handleClient(); // Handle incoming HTTP requests
  // Animate the blink
  for (int size = 30; size > 20; size -= 2) { // Gradually reduce the size
    drawEyes(size, size, 20, 80);
    delay(100); // Delay to control the animation speed
  }

  delay(250); // Pause for a moment while eyes are closed

  // Reopen the eyes
  for (int size = 20; size <= 30; size += 2) {
    drawEyes(size, size, 20, 80);
    delay(100); // Delay to control the animation speed
  }

}