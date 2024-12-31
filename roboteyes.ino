#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height, SSD1306_128x64 is standard
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create an SSD1306 display object connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void drawEyes(int width, int height, int leftEyeX, int rightEyeX) {
  display.clearDisplay();

  // Draw the left eye
  int leftEyeY = 20;
  display.fillRect(leftEyeX, leftEyeY, width, height, SSD1306_WHITE);

  // Draw the right eye
  int rightEyeY = 20;
  display.fillRect(rightEyeX, rightEyeY, width, height, SSD1306_WHITE);

  display.display();
}

// Function to animate eyes moving to the left
void eyesLeft() {
  for (int leftEyeX = 20, rightEyeX = 80; leftEyeX >= 5; --leftEyeX, --rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(5000); // Pause for 5 seconds after the movement
}

void eyesLeftback() {
  for (int leftEyeX = 5, rightEyeX = 65; leftEyeX <= 20; ++leftEyeX, ++rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(1000); // Pause for 5 seconds after the movement
}


// Function to animate eyes moving to the right
void eyesRight() {
  for (int leftEyeX = 20, rightEyeX = 80; leftEyeX <= 35; ++leftEyeX, ++rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(5000); // Pause for 5 seconds after the movement
}

void eyesRightback() {
  for (int leftEyeX = 35, rightEyeX = 95; leftEyeX >= 20; --leftEyeX, --rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(1000); // Pause for 5 seconds after the movement
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the display
  if (!display.begin(SSD1306_PAGEADDR, 0x3C)) { // Address 0x3C for most OLEDs
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();

  // Draw initial eyes
  drawEyes(30, 30, 20, 80);
}

void loop() {
  // Animate the blink
  for (int size = 30; size > 20; size -= 2) { // Gradually reduce the size
    drawEyes(size, size, 20, 80);
    delay(100); // Delay to control the animation speed
  }

  delay(500); // Pause for a moment while eyes are closed

  // Reopen the eyes
  for (int size = 20; size <= 30; size += 2) {
    drawEyes(size, size, 20, 80);
    delay(100); // Delay to control the animation speed
  }

  delay(5000); // Wait for 5 seconds before the next blink

  // Move eyes to the left
  eyesLeft();

  eyesLeftback();

  // Move eyes to the right
  eyesRight();

  eyesRightback();
}
