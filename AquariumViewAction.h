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


class AquariumViewProgramAuto: public AquariumViewAction {
public:
  AquariumViewProgramAuto() {
    sprintf(name,"Auto Program");
  }

  void run_action() {
    AquariumTime& now = rtc_get_time();
    ProgramTime program_time(now.hour, now.minute);
    get_program_table().activate_program_by_time(program_time);
  }
};

class AquariumViewUptime: public AquariumViewAction {
public:
  AquariumViewUptime() {
    sprintf(name,"Uptime");
    timeout = 10;
  }

  void run_action() {
  }

  virtual void update_display(AquariumDisplay& display) {
    if (timer_alert.is_expired()) {
      complete_view();
    }
    AquariumTime& now = rtc_get_time();
    AquariumTime& start = rtc_get_start_time();
    
    char* buffer = get_buffer();

    display.setFont(System5x7);
    display.setCursor(0, 3);
    display.println("Current date:");
    display.setCursor(0, 4);
    sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%02d  ", now.hour, now.minute, now.second, now.month, now.date, now.year);
    display.println(buffer);
    display.setCursor(0, 6);
    display.println("Up since:");
    display.setCursor(0, 7);
    sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%02d  ", start.hour, start.minute, start.second, start.month, start.date, start.year);
    display.println(buffer);
  }

  
};

AquariumViewProgramAuto __viewProgramAuto;

AquariumViewProgramAuto* get_view_program_auto_ptr() {
  return &__viewProgramAuto;
}

AquariumViewUptime __viewUpTime;

AquariumViewUptime* get_view_uptime_ptr() {
  return &__viewUpTime;
}
