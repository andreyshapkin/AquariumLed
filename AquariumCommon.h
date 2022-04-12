#ifndef AQUARIUM_COMMON
#define AQUARIUM_COMMON

char __str[60];

char* get_str() {
  return __str;
}

char* get_buffer() {
  return __str;
}


class AquariumTime {
  public:
    byte year;
    byte month;
    byte date;
    byte day;
    byte hour;
    byte minute;
    byte second;
    
    void set(AquariumTime& time) {
      year = time.year;
      month = time.month;
      date = time.date;
      day = time.day;
      hour = time.hour;
      minute = time.minute;
      second = time.second;
    }
};

class Color {
public:
  byte red;
  byte green;
  byte blue;

  Color() {
    red = 0;
    green = 0;
    blue = 0;
  }

  Color(byte r, byte g, byte b) {
    red = r;
    green = g;
    blue = b;
  }
  
  void set(byte r, byte g, byte b) {
    red = r;
    green = g;
    blue = b;
  }
  
  void set(Color& color) {
    set(color.red, color.green, color.blue);
  }

  Color& get(byte r, byte g, byte b) {
    set(r, g, b);
    return *this;
  }
};

#endif // AQUARIUM_COMMON
