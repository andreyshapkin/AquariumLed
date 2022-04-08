
class AquariumViewCustomRGB: public AquariumViewBase {
private:
  byte active_selection;
  bool updating_color;
  byte color[3];
  AquariumTimerAlert timer_alert;
public:
  AquariumViewCustomRGB() {
    sprintf(name, "Custom RGB");
  }
  
  void init(AquariumDisplay& display) {
    activate();
    active_selection = 0;
    updating_color = false;
    Color& current_color = get_color_control().color();
    color[0] = current_color.red;
    color[1] = current_color.green;
    color[2] = current_color.blue;
    timer_alert.reset(120);

    display.clear();
    display_header(display);
  }
  
  void update_control(RotaryKnob& knob) {
    timer_alert.reset(120);
    
    if (knob.click_count) {
      // select/unselect color to update
      if (active_selection<3) {
        updating_color = ! updating_color;
      } else {
        complete_view();
      }
    }
    if (knob.increment) {
      if (updating_color) {
        color[active_selection] += knob.increment;
        force_rgb(color[0],color[1],color[2]);
      } else {
        active_selection = increment_mod(active_selection, knob.increment, 4);
      }
    }
  }

  void complete_view() {
    deactivate();
    unforce_rgb();

    active_selection = 0;
    updating_color = false;
  }

  void update_display(AquariumDisplay& display) {
    if (timer_alert.is_expired()) {
      active_selection = 0;
      complete_view();
    }
    char* buffer = get_buffer();

    display.setFont(X11fixed7x14);
    
    for (byte i=0;i<3;i++) {
      display.setInvertMode(active_selection == i);
      display.setCursor(20 + 20*i, 3);
      sprintf(buffer, "%02x", color[i]);
      display.println(buffer);
    }

    display.setInvertMode(false);
    display.setCursor(100, 3);
    if (updating_color) {
      display.println("***");
    } else {
      display.println("   ");      
    }

    display.setInvertMode(active_selection == 3);
    display.setCursor(30, 6);
    sprintf(buffer, "Exit");
    display.println(buffer);

    display.setInvertMode(false);

  }
};

AquariumViewCustomRGB __viewCustomRGB;

AquariumViewCustomRGB* get_view_custom_rgb() {
  return &__viewCustomRGB;
}
