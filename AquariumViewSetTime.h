
#include "AquariumViewBase.h"


class AquariumViewTimeSet: public AquariumViewBase {
private:
  enum SelectionType {
    HOUR,
    MINUTE,
    SECOND,
    SET,
    CANCEL,
    ENUM_SIZE
  };

  byte active_selection;
  byte selection_update;
  byte hour;
  byte minute;
  byte second;

public:
  AquariumViewTimeSet() {
    sprintf(name, "Time Set");
    active_selection = 0;
    selection_update = 0xff;
    hour = 0;
    minute = 0;
    second = 0;
  }
  
  void init() {
    activate();

    AquariumTime& time_now = rtc_get_time();  
    active_selection = 0;
    selection_update = 0xff;
    hour = time_now.hour;
    minute = time_now.minute;
    second = 0;
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      if (selection_update != 0xff) {
        selection_update = 0xff;
      } else {
        if (active_selection < SET) {
          selection_update = active_selection;
        } else if (active_selection == SET) {
          complete_view(true);
        } else {
          complete_view(false);
        }
      }
    }
    if (knob.increment) {
      if (selection_update == HOUR) {
        hour = increment_mod(hour, knob.increment, 24);
      } else if (selection_update == MINUTE) {
        minute = increment_mod(minute, knob.increment, 60);
      } else if (selection_update == SECOND) {
        second = increment_mod(second, knob.increment, 60);
      } else {
        active_selection = increment_mod(active_selection, knob.increment, ENUM_SIZE);
      }
    }
  }

  void complete(bool changes_made) {
    deactivate();
    active_selection = 0;    
    selection_update = 0xff;
    if (changes_made) {
      rtc_set_time(hour, minute, second);
    }
  }

  void update_display(AquariumDisplay& display) {
    char* buffer = get_buffer();
  
    display.setFont(lcdnums14x24);

    display.setInvertMode(active_selection == HOUR);
    display.setCursor(10, 2);
    sprintf(buffer, "%02d", hour);
    display.print(buffer);

    display.setInvertMode(active_selection == MINUTE);
    display.setCursor(50, 2);
    sprintf(buffer, "%02d", minute);
    display.print(buffer);

    display.setInvertMode(active_selection == SECOND);
    display.setCursor(90, 2);
    sprintf(buffer, "%02d", second);
    display.print(buffer);

    display.setFont(X11fixed7x14);

    
    display.setInvertMode(active_selection == SET);
    display.setCursor(10, 6);
    display.print("SET");

    display.setInvertMode(active_selection == CANCEL);
    display.setCursor(40, 6);
    display.print("CANCEL");

    display.setInvertMode(false);
    display.setCursor(90, 6);
    if (selection_update<ENUM_SIZE) {
      display.print("*");
    } else {
      display.print(" ");
    }  
  }
};

AquariumViewTimeSet __viewTimeSet;

AquariumViewTimeSet* get_view_time_set_ptr() {
  return &__viewTimeSet;
}
