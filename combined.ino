#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
#include <Servo.h>

Servo bottomServo;

int frequency = 0;
const int angles[] = {0, 45, 90, 135, 180};

void setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Setting frequency-scaling to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    Serial.begin(9600);

    bottomServo.attach(9);  // Attach the servo to pin 9
}

void loop() {
  int color = scanColor();

  if (color > -1){
    bottomServo.write(angles[color]);
    delay(50);
  }
}


int scanColor() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    frequency = pulseIn(sensorOut, LOW);
    float R = frequency;
    
    delay(30);
    
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    frequency = pulseIn(sensorOut, LOW);
    float G = frequency;

    delay(30);

    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    frequency = pulseIn(sensorOut, LOW);
    float B = frequency;

    delay(30);

    Serial.println("R: " + String(R) + " G: " + String(G) + " B: " + String(B));

    int color = -1;
    float GR = G / R;
    float BR = B / R;
    float BG = B / G;
    
    // Expanded ranges for leniency
    if ((GR >= 0.80 && GR <= 0.93) && (BR >= 0.50 && BR <= 0.70) && (BG >= 0.62 && BG <= 0.75)) {
        color = 0; // Blue
    } else if ((GR >= 1.55 && GR <= 2.45) && (BR >= 1.20 && BR <= 1.95) && (BG >= 0.74 && BG <= 0.82)) {
        color = 1; // Red
    } else if ((GR >= 1.05 && GR <= 1.29) && (BR >= 1.35 && BR <= 1.93) && (BG >= 1.15 && BG <= 1.57)) {
        color = 2; // Yellow
    }

    return color;
}
