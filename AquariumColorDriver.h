#ifndef AQUARIUM_COLOR_DRIVER
#define AQUARIUM_COLOR_DRIVER

#define LED_RED 9
#define LED_BLUE 10
#define LED_GREEN 11

void color_driver_init() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

bool color_driver_is_forced = false;

void write_rgb(byte red, byte green, byte blue) {
  if (color_driver_is_forced) {
    return;
  }
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue); 
}

// used for LED testing
void force_rgb(byte red, byte green, byte blue) {
  color_driver_is_forced = true;
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);   
}

void unforce_rgb() {
  color_driver_is_forced = false;
}

#endif // AQUARIUM_COLOR_DRIVER
