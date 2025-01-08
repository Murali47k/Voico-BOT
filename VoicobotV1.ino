
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>


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
  servo1.write(90);  // Neutral for servo1
  servo2.write(120); // Spin in one direction
  servo3.write(60);  // Spin in the opposite direction
}

// Function to turn right
void turnRight() {
  Serial.println("Action: Turning right");
  servo1.write(90);  // Neutral for servo1
  servo2.write(60);  // Spin in one direction
  servo3.write(120); // Spin in the opposite direction
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
}
