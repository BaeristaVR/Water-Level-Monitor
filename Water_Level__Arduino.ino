#include "lcd_display.h"

// Pin configuration
const int analogInPin = A0;   // Analog input pin for sensor
const int buzzerPin = 6;      // Pin for the buzzer
const int threshold = 50;     // Threshold to ignore noise when sensor is disconnected

void setup() {
    // Initialize LCD and Serial
    setupLCD();
    Serial.begin(9600);
    Serial.println("\nWater Level Monitor");

    // Initialize pins as outputs
    pinMode(buzzerPin, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
}

void loop() {
    int sensorValue = 0;

    // Read sensor and average multiple values
    int total = 0;
    for (int i = 0; i < 10; i++) {
        total += analogRead(analogInPin);
        delay(10);
    }
    sensorValue = total / 10;

    // Check for disconnected sensor
    if (sensorValue < threshold) {
        Serial.println("No valid reading (sensor disconnected)");
        displayWaterLevel(sensorValue, "Disconnected");
        noTone(buzzerPin);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
    } else {
        // Determine water level and set LEDs and buzzer
        String level;
        if (sensorValue >= 100 && sensorValue <= 400) {
            level = "LOW";
            digitalWrite(2, HIGH);
            digitalWrite(3, LOW);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            noTone(buzzerPin);
        } else if (sensorValue >= 401 && sensorValue <= 580) {
            level = "MEDIUM";
            digitalWrite(2, LOW);
            digitalWrite(3, HIGH);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            noTone(buzzerPin);
        } else if (sensorValue >= 581 && sensorValue <= 600) {
            level = "HIGH";
            digitalWrite(2, LOW);
            digitalWrite(3, LOW);
            digitalWrite(4, HIGH);
            digitalWrite(5, HIGH);
            tone(buzzerPin, 1000);
        } else {
            level = "MAX";
            digitalWrite(2, LOW);
            digitalWrite(3, LOW);
            digitalWrite(4, HIGH);
            digitalWrite(5, HIGH);
            tone(buzzerPin, 2000);
        }
        
        Serial.print("Sensor value: ");
        Serial.println(sensorValue);
        displayWaterLevel(sensorValue, level);
    }
    
    delay(500);  // Update every half second
}
