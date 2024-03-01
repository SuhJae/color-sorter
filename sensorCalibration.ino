#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6

int frequency = 0;

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
}

void loop() {
    const int sampleSize = 100;
    float GR_samples[sampleSize];
    float BR_samples[sampleSize];
    float BG_samples[sampleSize];

    // Collecting samples
    for (int i = 0; i < sampleSize; i++) {
        collectSamples(&GR_samples[i], &BR_samples[i], &BG_samples[i]);
        Serial.println("Sample " + String(i) + "/" + String(sampleSize) + " collected");
        delay(100);
    }

    Serial.println("====================================");
    Serial.println("All samples collected");
    Serial.println("====================================");
    Serial.println("");

    // Calculating statistics for each ratio
    calculateAndLogStats(GR_samples, sampleSize, "G/R");
    calculateAndLogStats(BR_samples, sampleSize, "B/R");
    calculateAndLogStats(BG_samples, sampleSize, "B/G");

    // Stop the program
    while (true) {
        delay(1000);
    }
}

// Collects sample color ratios from the color sensor and returns reduced set of ratios
void collectSamples(float* GR, float* BR, float* BG) {
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

    *GR = G / R;
    *BR = B / R;
    *BG = B / G;
}

void calculateAndLogStats(float samples[], int size, String ratioName) {
    // Calculating mean
    float sum = 0, mean, stdDev = 0;
    for (int i = 0; i < size; i++) {
        sum += samples[i];
    }
    mean = sum / size;

    // Calculating standard deviation
    for (int i = 0; i < size; i++) {
        stdDev += pow(samples[i] - mean, 2);
    }
    stdDev = sqrt(stdDev / size);

    // Statstically, it will remove 5% of outliers automatically
    float lowerBound = mean - stdDev - stdDev; 
    float upperBound = mean + stdDev - stdDev;

    Serial.println(ratioName + " Mean: " + String(mean) + ", Std Dev: " + String(stdDev));
    Serial.println(ratioName + " Suggested Range: " + String(lowerBound) + " to " + String(upperBound));
}
