
#include "AquariumViewBase.h"


class AquariumViewDateSet: public AquariumViewBase {
private:
  enum SelectionType {
    MONTH,
    DATE,
    YEAR,
    SET,
    CANCEL,
    ENUM_SIZE
  };
  byte active_selection;
  byte selection_update;
  byte month;
  byte date;
  byte year;
public:
  AquariumViewDateSet() {
    sprintf(name, "Date Set");
    active_selection = 0;
    selection_update = 0xff;
    month = 0;
    date = 0;
    year = 0;
  }
  
  void init() {
    AquariumTime& time_now = rtc_get_time();  
    active_selection = 0;
    month = time_now.month;
    date = time_now.date;
    year = time_now.year;
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
      if (selection_update == MONTH) {
        month = increment_mod_one(month, knob.increment, 12);
      } else if (selection_update == DATE) {
        date = increment_mod_one(date, knob.increment, 31);
      } else if (selection_update == YEAR) {
        year = increment_mod(year, knob.increment, 100);
      } else {
        active_selection = increment_mod(active_selection, knob.increment, ENUM_SIZE);
      }
    }
  }

  void complete(bool changes_made) {
    active_selection = 0;    
    selection_update = 0xff;
    if (changes_made) {
      rtc_set_date(month, date, year);
    }
  }
  
 
  void update_display(AquariumDisplay& display) {
    char* buffer = get_buffer();
  
    display.setFont(X11fixed7x14);

    display.setInvertMode(active_selection == MONTH);
    display.setCursor(10, 3);
    sprintf(buffer, " %02d ", month);
    display.print(buffer);

    display.setInvertMode(active_selection == DATE);
    display.setCursor(50, 3);
    sprintf(buffer, " %02d ", date);
    display.print(buffer);

    display.setInvertMode(active_selection == YEAR);
    display.setCursor(90, 3);
    sprintf(buffer, " %02d ", year);
    display.print(buffer);

    display.setInvertMode(false);
    display.setCursor(40, 3);
    display.print("/");
    display.setCursor(80, 3);
    display.print("/");

    display.setInvertMode(active_selection == SET);
    display.setCursor(10, 5);
    display.print("SET");

    display.setInvertMode(active_selection == CANCEL);
    display.setCursor(40, 5);
    display.print("CANCEL");

    display.setInvertMode(false);
    display.setCursor(90, 5);
    if (selection_update<ENUM_SIZE) {
      display.print("*");
    } else {
      display.print(" ");
    }
  }
};

AquariumViewDateSet __viewDateSet;

AquariumViewDateSet* get_view_date_set_ptr() {
  return &__viewDateSet;
}
