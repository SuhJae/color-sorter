#include <Servo.h>

Servo myServo;
int currentAngle = 0;
unsigned long previousMillis = 0;
unsigned long interval = 500;

void setup() {
    myServo.attach(9);  // Attach the servo to pin 9
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        int angles[] = {0, 45, 90, 135, 180};
        int randomIndex = random(5);
        int newAngle = angles[randomIndex];
        while (newAngle == currentAngle) {
            randomIndex = random(5);
            newAngle = angles[randomIndex];
        }
        currentAngle = newAngle;
        myServo.write(currentAngle);
    }
}
