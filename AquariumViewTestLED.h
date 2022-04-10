class AquariumViewTestLED: public AquariumViewBase {
private:
  byte active_selection;
  AquariumTimerMilliSecond timer_alert;
  const char* color;
  bool first_call;

public:
  AquariumViewTestLED() {
    sprintf(name, "Test LED");
  }
  
  void init() {
    active_selection = 0;
    first_call = true;
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      complete_view();
    }
  }

  void complete(bool changes_made) {
    active_selection = 0;
  }

  void step() {
    if (first_call) {
      force_rgb(255,0,0);
      color = "RED";
      
      timer_alert.reset(millis());
      first_call = false;
    }
    
    if (timer_alert.time_ticked(millis(), 3000)) {
      active_selection++;
      set_update();
      
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
    }
  }

  void update_display(AquariumDisplay& display) {
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
