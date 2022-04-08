
class AquariumViewProgramList: public AquariumViewBase {
private:
  byte active_selection;
  bool selection_changed;
  AquariumTimerAlert timer_alert;
public:
  AquariumViewProgramList() {
    sprintf(name, "Program List");
  }
  
  void init(AquariumDisplay& display) {
    activate();
    active_selection = get_program_table().get_active_program_index();
    selection_changed = false;
    timer_alert.reset(120);

    display.clear();
    display_header(display);
  }
  
  void update_control(RotaryKnob& knob) {
    timer_alert.reset(120);
    
    if (knob.click_count) {
      complete_view();
    }
    if (knob.increment) {
      selection_changed = true;
      active_selection = increment_mod(active_selection, knob.increment, PROGRAM_TABLE_NUMBER_ENTRIES);
    }
  }

  void complete_view() {
    deactivate();
    
    if (selection_changed) {
      get_program_table().force_program(active_selection);
    }
    active_selection = 0;
    selection_changed = false;
  }
  
  void update_display(AquariumDisplay& display) {
    if (timer_alert.is_expired()) {
      active_selection = 0;
      selection_changed = false;
      complete_view();
    }
    
    char* buffer = get_buffer();

    display.setFont(System5x7);
    for (byte i=0; i<PROGRAM_TABLE_NUMBER_ENTRIES; i++) {
      ProgramEntry& program = get_program_table().get_program(i);
      display.setCursor(0, 2 + i);
      sprintf(buffer, "   %s %02d:%02d    ", program.get_name(), program.get_time().hour, program.get_time().minute);
      display.println(buffer);    
    }  
    display.setCursor(0, 2 + active_selection);
    display.print("->");    

  }
};

AquariumViewProgramList __viewProgramList;

AquariumViewProgramList* get_view_program_list_ptr() {
  return &__viewProgramList;
}
