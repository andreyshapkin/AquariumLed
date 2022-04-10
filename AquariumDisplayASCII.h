
// https://github.com/greiman/SSD1306Ascii

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3c

SSD1306AsciiAvrI2c display;

typedef SSD1306AsciiAvrI2c AquariumDisplay;

void display_init() {
  //Serial.println(F("Starting setup"));
  //display.begin(&Adafruit128x64, I2C_ADDRESS);

  display.begin(&SH1106_128x64, I2C_ADDRESS);
  display.setI2cClock(60000);
  display.clear();
}

AquariumDisplay& get_display() {
  return display;
}
