#ifndef AQUARIUM_VIEW_BASE
#define AQUARIUM_VIEW_BASE

class AquariumViewBase {
  bool _active;
public:
  char name[20];
  virtual void init(AquariumDisplay& display) {};
  virtual void update_control(RotaryKnob& knob) {};
  virtual void update_display(AquariumDisplay& display) {};
  AquariumViewBase() {
    _active = false;
  }

  void activate() {
    _active = true;
  }
  void deactivate() {
    _active = false;
  }
  bool active() {
    return _active;
  }

  virtual void display_header(AquariumDisplay& display) {
    display.setFont(Callibri15);
    display.setCursor(0, 0);
    display.println(name);
  }
};

// range of values [0:max-1]
int increment_mod(int value, int increment, int max) {
  value += increment;
  if (value<0) {
    value += max;
  }
  value = value % max;
  return value;
}

// range of values [1:max]
int increment_mod_one(int value, int increment, int max) {
  int value_mod_0 = value - 1;
  value_mod_0 = increment_mod(value_mod_0, increment, max);
  return value_mod_0 + 1;
}



class AquariumViewAction: public AquariumViewBase {
private:
public:
  AquariumTimerAlert timer_alert;
  byte timeout = 2;
  AquariumViewAction() {}
  
  void init(AquariumDisplay& display) {
    activate();
    display.clear();
    timer_alert.reset(timeout);
    display_header(display);
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      complete_view();
    }
  }

  void complete_view() {
    run_action();
    deactivate();
  }

  virtual void run_action() {}

  virtual void update_display(AquariumDisplay& display) {
    if (timer_alert.is_expired()) {
      complete_view();
    }
    
    display.setFont(Callibri15);
    display.setCursor(0, 5);
    display.println("BEBUNYA ROCKS!!!");
  }
};


#endif // AQUARIUM_VIEW_BASE
