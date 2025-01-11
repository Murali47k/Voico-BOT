#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment to initialize the I2C address; uncomment only one.
   If you get a blank screen, try the other. */
#define i2c_Address 0x3C // Typically eBay OLED's
//#define i2c_Address 0x3D // Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // QT-PY / XIAO

// Create an SH1106 display object connected to I2C
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  delay(5000); // Pause for 5 seconds after the movement
}

void eyesLeftback() {
  for (int leftEyeX = 5, rightEyeX = 65; leftEyeX <= 20; ++leftEyeX, ++rightEyeX) {
    drawEyes(30, 30, leftEyeX, rightEyeX);
    delay(50); // Delay for smooth animation
  }
  delay(1000); // Pause for 1 second after the movement
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
  delay(1000); // Pause for 1 second after the movement
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the display
  if (!display.begin(i2c_Address)) {
    Serial.println(F("SH1106 allocation failed"));
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
