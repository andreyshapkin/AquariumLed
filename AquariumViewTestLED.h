class AquariumViewTestLED: public AquariumViewBase {
private:
  byte active_selection;
  AquariumTimerAlert timer_alert;
public:
  AquariumViewTestLED() {
    sprintf(name, "Test LED");
  }
  
  void init() {
    active_selection = 0;
    timer_alert.reset(2);
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      complete_view();
    }
  }

  void complete(bool changes_made) {
    active_selection = 0;
  }

  void update_display(AquariumDisplay& display) {
    if (timer_alert.is_expired()) {
      active_selection++;
      timer_alert.reset(2);
    }

    if (needs_refresh()) {
      display.clear();
      display_header(display);
    }

    const char* color;

    if (active_selection == 0) {
      force_rgb(255,0,0);
      color = "RED";
    } else if (active_selection == 1) {
      force_rgb(0,255,0);
      color = "GREEN";
    } else if (active_selection == 2) {
      force_rgb(0,0,255);
      color = "BLUE";
    } else if (active_selection == 3) {
      force_rgb(255,255,255);
      color = "WHITE";
    } else if (active_selection == 4) {
      force_rgb(0,0,0);
      color = "OFF";
    } else if (active_selection == 5) {
      unforce_rgb();
      color = "DONE";
      complete_view();
    }

    display.setFont(Callibri15);
    display.setCursor(30, 5);
    sprintf(get_buffer(),"%s     ", color);
    display.println(get_buffer());
  }
};

AquariumViewTestLED __viewTestLed;

AquariumViewTestLED* get_view_test_led_ptr() {
  return &__viewTestLed;
}
