#ifndef AQUARIUM_VIEW_BASE
#define AQUARIUM_VIEW_BASE

AquariumTimerAlert _timer_alert;


class AquariumViewBase {
  bool _active;
  // full screen refresh - clearing display and drawing header/static text
  bool _needs_display_refresh;
  // partial interactive refresh
  bool _needs_display_update;
public:
  char name[20];
  byte timeout;
  byte header_offset;

  virtual void init() {};
  virtual void update_control(RotaryKnob& knob) {};
  virtual void update_display(AquariumDisplay& display) {};
  virtual void step() {};
  virtual void complete(bool result) {};
  virtual void display_static(AquariumDisplay& display) {}

  AquariumViewBase() {
    _active = false;
    _needs_display_refresh = true;
    _needs_display_update = true;
    timeout = 120;
    header_offset = 0;
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
  void set_refresh() {
    _needs_display_refresh = true;
  }
  bool needs_refresh() {
    bool result = _needs_display_refresh;
    _needs_display_refresh = false;
    return result;
  }
  void set_update() {
    _needs_display_update = true;
  }
  bool needs_update() {
    bool result = _needs_display_update;
    _needs_display_update = false;
    return result;
  }
 
  void init_view(AquariumDisplay& display) {
    activate();
    set_refresh();
    set_update();
    _timer_alert.reset(timeout);
    init();
  }

  void complete_view(bool result=false) {
    deactivate();
    complete(result);
  }


  // display header of the View
  virtual void display_header(AquariumDisplay& display) {
    display.setFont(Callibri15);
    display.setCursor(header_offset, 0);
    display.println(name);
  }
 
  void update_display_view(AquariumDisplay& display) {
    if (_timer_alert.is_expired()) {
      complete_view();
    }
    step();
    if (needs_refresh()) {
      display.clear();
      display_header(display);
      display_static(display);
    }    
    if (needs_update()) {
      update_display(display);
    }
  }

  void update_control_view(RotaryKnob& knob) {
    _timer_alert.reset(timeout);
    if (knob.click_count || knob.increment) {
      set_update();
    }
    update_control(knob);
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
  AquariumViewAction() {
    timeout = 2;
  }
  
  void init() {
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      complete_view();
    }
  }

  void complete(bool result) {
    run_action();
  }

  virtual void run_action() {}

  virtual void update_display(AquariumDisplay& display) {   
  }

  void display_static(AquariumDisplay& display) {
    display.setFont(Callibri15);
    display.setCursor(0, 5);
    display.println("BEBUNYA ROCKS!!!");
  }

};


#endif // AQUARIUM_VIEW_BASE
