#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// LCD configuration (I2C address and size)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Replace 0x27 with your actual I2C address

// Pin configuration
const int analogInPin = A0;   // Analog input pin for sensor
const int buzzerPin = 6;      // Pin for the buzzer

int sensorValue = 0;          // Variable to store the sensor value
const int threshold = 50;     // Threshold to ignore noise when sensor is disconnected

void setup() {
  // Initialize LCD and Serial
  pinMode(analogInPin, OUTPUT);   // Set A0 temporarily to OUTPUT
  digitalWrite(analogInPin, LOW); // Drain any floating voltage
  pinMode(analogInPin, INPUT);    // Set back to INPUT for sensor reading
  
  Wire.begin();                   // Join I2C bus
  Serial.begin(9600);             // Start serial communication at 9600 bps
  Serial.println("\nWater Level Monitor");

  // Initialize pins as outputs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(buzzerPin, OUTPUT);     // Buzzer as output

  // Initialize the LCD
  lcd.init();                     // Initialize LCD
  lcd.backlight();                // Turn on the LCD backlight
  lcd.setCursor(0, 0);
  lcd.print("Water Level Monitor");
  delay(2000);                    // Show welcome message for 2 seconds
  lcd.clear();
}

void loop() {
  // Read and average multiple values from the sensor to reduce noise
  int total = 0;
  for (int i = 0; i < 10; i++) {       // Take 10 samples
      total += analogRead(analogInPin);
      delay(10);                       // Short delay between samples
  }
  sensorValue = total / 10;            // Averaged sensor value

  // Check for disconnected sensor by validating the sensor reading
  if (sensorValue < threshold) {       // If reading is below threshold, assume no sensor
    Serial.println("No valid reading (sensor disconnected)");
    lcd.setCursor(0, 1);
    lcd.print("Sensor disconnected");
    noTone(buzzerPin);                 // Turn off buzzer if disconnected
    digitalWrite(2, LOW);              // Turn off all LEDs
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  } else {
    // Print and display the sensor value if valid
    Serial.print("Sensor value: ");
    Serial.println(sensorValue);
    lcd.setCursor(0, 0);   // Display on LCD
    lcd.print("Sensor Value: ");
    lcd.print(sensorValue);
    lcd.setCursor(0, 1);   // Move to second line

    // Control LEDs and display water level status on the LCD based on sensor value
    if (sensorValue >= 100 && sensorValue <= 400) {
      digitalWrite(2, HIGH);  // Low level
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      lcd.print("Water Level: LOW   ");
      noTone(buzzerPin);       // No buzzer for low level
    } 
    else if (sensorValue >= 401 && sensorValue <= 580) {
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);  // Medium level
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      lcd.print("Water Level: MEDIUM");
      noTone(buzzerPin);       // No buzzer for medium level
    } 
    else if (sensorValue >= 581 && sensorValue <= 600) {
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);  // High level
      digitalWrite(5, HIGH);
      lcd.print("Water Level: HIGH  ");
      tone(buzzerPin, 1000);   // Sound buzzer for high level (1000 Hz)
    } 
    else if (sensorValue > 600) {
      // Maximum water level reached
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      lcd.print("Water Level: MAX   ");
      tone(buzzerPin, 2000);   // Buzzer sounds at 2000 Hz for max level
    }
  }
  
  delay(500);  // Update every half second
}
