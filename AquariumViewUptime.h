

class AquariumViewUptime: public AquariumViewBase {
private:
  byte active_selection;
  AquariumTimerAlert timer_alert;
public:
  AquariumViewUptime() {
    active_selection = 0;
    timer_alert.reset(20);
    sprintf(name,"Uptime");
  }

  void init(AquariumDisplay& display) {
    activate();

    active_selection = 0;
    timer_alert.reset(10);

    display.clear();
    display_header(display);
  }
  
  void complete_view() {
    deactivate();
    active_selection = 0;    
  }

  void update_control(RotaryKnob& knob) {
    timer_alert.reset(10);

    if (knob.click_count) {
      complete_view();
    }
    if (knob.increment) {
      active_selection = increment_mod(active_selection, knob.increment, 2);
      AquariumDisplay& display = get_display();
      display.clear();
      display_header(display);
    }
  }

  void update_display(AquariumDisplay& display) {
    if (timer_alert.is_expired()) {
      complete_view();
    }
    display.setFont(System5x7);
    display.setCursor(70, 1);
    display.println(" <--->");

    if (active_selection == 0) {
      display_current_view(display);
    } else {
      display_history(display);
    }
  }

  void display_current_view(AquariumDisplay& display) {
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

  void display_history(AquariumDisplay& display) {
    char* buffer = get_buffer();

    for (int i=0; i <6; i++) {
      AquariumTime& history_time = eeprom_read_boot_time(i);
      display.setFont(System5x7);
      display.setCursor(0, 2 + i);
      sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%02d  ", history_time.hour, history_time.minute, history_time.second, history_time.month, history_time.date, history_time.year);
      display.println(buffer);
    }
  } 
};


AquariumViewUptime __viewUpTime;

AquariumViewUptime* get_view_uptime_ptr() {
  return &__viewUpTime;
}
