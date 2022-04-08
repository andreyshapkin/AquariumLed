// https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/

// !!!! NOT USED - TAKES TOO MUCH MEMORY
// !!! It malloc's 1K buffer for GFX lib

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "AquariumLed.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define DISPLAY_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void display_init() {
  //Serial.println(F("Starting setup"));

  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) { // Address 0x3D for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
 
  display.display();
}

void display_header() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Vikusa project");
  display.drawChar(90,0,0x03,WHITE,BLACK,1);
  display.drawChar(100,0,0x03,WHITE,BLACK,1);
  display.drawChar(110,0,0x03,WHITE,BLACK,1);  
}

const char* DAYS[] = {"", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat", "Sun"};

void display_time(AquariumTime& now) {
  char buffer[40];

  // clock
  display.setTextSize(2);
  display.setCursor(0, 20);
  sprintf(buffer, "%02d:%02d ", now.hour, now.minute);
  display.print(buffer);

  // seconds
  display.setTextSize(1);
  display.setCursor(62, 20);
  sprintf(buffer, "%02d", now.second);
  display.print(buffer);

  // day
  display.setTextSize(1);
  display.setCursor(100, 18);
  display.print(DAYS[now.day]);

  // date
  display.setTextSize(1);
  display.setCursor(80, 27);
  sprintf(buffer, "%02d/%02d/%02d", now.month, now.date, now.year);
  display.print(buffer);
}

void display_mode(char* mode) {
  char buffer[40];

  // day
  display.setTextSize(1);
  display.setCursor(0, 45);
  sprintf(buffer, "Current: %s", mode);
  display.print(buffer);
}

void display_next() {
  char buffer[40];

  // day
  display.setTextSize(1);
  display.setCursor(0, 55);
  sprintf(buffer, "Next: %02d:%02d %s", 21, 00, "Sunset");
  display.print(buffer);

  display.display();
}

void display_update(AquariumTime& now) {
  display.clearDisplay();
  display_header();
  display_time(now);
  display_mode("Sunrise");
  display_next();
  display.display();  
}
