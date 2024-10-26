// lcd_display.h
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// LCD configuration (I2C address and size)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Replace 0x27 with your actual I2C address

// Function to initialize the LCD
void setupLCD() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Water Level Monitor");
    delay(2000);
    lcd.clear();
}

// Function to display the water level status
void displayWaterLevel(int sensorValue, String level) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Value: ");
    lcd.print(sensorValue);
    lcd.setCursor(0, 1);
    lcd.print("Water Level: ");
    lcd.print(level);
    lcd.print("   ");
}
