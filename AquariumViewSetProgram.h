
#include "AquariumViewBase.h"


class AquariumViewProgramSet: public AquariumViewBase {
private:
  enum SelectionType {
    HOUR,
    MINUTE,
    SET,
    NEXT,
    ENUM_SIZE
  };

  byte program_selection;
  byte active_selection;
  byte selection_update;
  byte hour;
  byte minute;

public:
  AquariumViewProgramSet() {
    sprintf(name, "Program Set");
    program_selection = 0;
    active_selection = 0;
    selection_update = 0xff;
    hour = 0;
    minute = 0;
  }
  
  void init() {
    program_selection = 0;
    init_tab();
  }

  void init_tab() {
    active_selection = NEXT;
    selection_update = 0xff;
    ProgramTime time_now = eeprom_read_program_time(program_selection);
    hour = time_now.hour;
    minute = time_now.minute;    
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      if (selection_update != 0xff) {
        selection_update = 0xff;
      } else {
        if (active_selection < SET) {
          selection_update = active_selection;
        } else if (active_selection == SET) {
          eeprom_save_program_time(program_selection, hour, minute);
          get_program_table().get_program(program_selection).set_time(hour, minute);
          active_selection = NEXT;
        } else {
          program_selection++;
          init_tab();
          if (program_selection>=PROGRAM_ENUM_SIZE) {
            complete_view(false);
          }
        }
      }
    }
    if (knob.increment) {
      if (selection_update == HOUR) {
        hour = increment_mod(hour, knob.increment, 24);
      } else if (selection_update == MINUTE) {
        minute = increment_mod(minute, knob.increment, 60);
      } else {
        active_selection = increment_mod(active_selection, knob.increment, ENUM_SIZE);
      }
    }
  }

  void complete(bool changes_made) {
  }

  void update_display(AquariumDisplay& display) {
    char* buffer = get_buffer();

    ProgramEntry& program = get_program_table().get_program(program_selection);
  
    display.setFont(X11fixed7x14);
    display.setCursor(0, 2);
    sprintf(buffer, "%s   ", program.get_name());
    display.print(buffer);

    display.setInvertMode(active_selection == HOUR);
    display.setCursor(40, 4);
    sprintf(buffer, "%02d", hour);
    display.print(buffer);

    display.setInvertMode(active_selection == MINUTE);
    display.setCursor(70, 4);
    sprintf(buffer, "%02d", minute);
    display.print(buffer);

    display.setFont(X11fixed7x14);

    
    display.setInvertMode(active_selection == SET);
    display.setCursor(10, 6);
    display.print("SET");

    display.setInvertMode(active_selection == NEXT);
    display.setCursor(40, 6);
    display.print("NEXT");

    display.setInvertMode(false);
    display.setCursor(90, 6);
    if (selection_update<ENUM_SIZE) {
      display.print("*");
    } else {
      display.print(" ");
    }  
  }
};

AquariumViewProgramSet __viewProgramSet;

AquariumViewProgramSet* get_view_program_set_ptr() {
  return &__viewProgramSet;
}
