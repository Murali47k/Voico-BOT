
#include <WiFi.h>
#include <WebServer.h>

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
  if (command == "move_forward") {
    Serial.println("Action: Moving forward");
  } else if (command == "move_backward") {
    Serial.println("Action: Moving backward");
  } else if (command == "turn_left") {
    Serial.println("Action: Turning left");
  } else if (command == "turn_right") {
    Serial.println("Action: Turning right");
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
}

void loop() {
  server.handleClient(); // Handle incoming HTTP requests
}

