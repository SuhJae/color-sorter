#include <Servo.h>

// Pin definitions for color sensor
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6

// Servo motor objects for sorting mechanism
Servo topMotor;
Servo bottomMotor;

// Global variable to store the frequency reading from the color sensor
int frequency = 0;

// Tolerance for color detection to account for slight variations
const float COLOR_TOLERANCE = 0.3;

// RGB ratios for different Skittle colors: [G/R, B/R, B/G]
const float RED[] = {1.57, 1.44, 0.91};
const float ORANGE[] = {1.81, 1.75, 0.97};
const float YELLOW[] = {1.38, 1.72, 1.24};
const float GREEN[] = {1.02, 1.22, 1.19};
const float PURPLE[] = {1.28, 1.20, 0.94};

// Servo Motor Angles corresponding to positions for each Skittle color
const int ANGLES[] = {0, 45, 90, 135, 180};

// Array of pointers to each color array for easier iteration
const float* COLORS[] = {YELLOW, ORANGE, YELLOW, GREEN, PURPLE}
const int NUM_COLORS = sizeof(COLORS) / sizeof(COLORS[0]); // Number of color arrays

void setup() {
    // Attach servo motors to their respective pins
    topMotor.attach(12);
    bottomMotor.attach(9);

    // Initialize color sensor pins
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Set frequency-scaling to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    // Start serial communication for debugging purposes
    Serial.begin(9600);
}

void loop() {  
    topMotor.write(90);  // Set the servo position to the middle position (90 degrees)
    delay(100);
    topMotor.write(115);

    // Continuously read the sensor and sort Skittles by color
    while (true) {
        int result = readSensor(); // Read the color sensor
        if (result != -1) { // If a color is recognized
            bottomMotor.write(ANGLES[result]); // Sort the Skittle by color
        }

    }
}

// Reads the color sensor and returns the index of the detected color
// Returns -1 if no color is detected within the tolerance
int readSensor() {
    // Read red component
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    frequency = pulseIn(sensorOut, LOW);
    float R = frequency;
    delay(30);

    // Read green component
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    frequency = pulseIn(sensorOut, LOW);
    float G = frequency;
    delay(30);

    // Read blue component
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    frequency = pulseIn(sensorOut, LOW);
    float B = frequency;
    delay(30);

    // Calculate RGB ratios
    float GR = G / R;
    float BR = B / R;
    float BG = B / G;

    // Identify the color based on the calculated ratios
    for (int i = 0; i < NUM_COLORS; i++) {
        const float* color = COLORS[i];
        bool inTolerance = true;

        // Check each RGB ratio against the stored values, considering the tolerance
        if (abs(GR - color[0]) > COLOR_TOLERANCE || abs(BR - color[1]) > COLOR_TOLERANCE || abs(BG - color[2]) > COLOR_TOLERANCE) {
            inTolerance = false;
        }

        if (inTolerance) {
            Serial.print("Color: ");
            Serial.println(i);
            return i; // Return the color index if a match is found
        }
    }

    return -1; // Return -1 if no match is found
}
