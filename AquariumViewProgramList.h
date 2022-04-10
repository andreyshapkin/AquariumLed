
class AquariumViewProgramList: public AquariumViewBase {
private:
  byte active_selection;
  bool selection_changed;
public:
  AquariumViewProgramList() {
    sprintf(name, "Program List");
  }
  
  void init() {
    activate();
    active_selection = get_program_table().get_active_program_index();
    selection_changed = false;
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      complete_view();
    }
    if (knob.increment) {
      selection_changed = true;
      active_selection = increment_mod(active_selection, knob.increment, PROGRAM_TABLE_NUMBER_ENTRIES);
    }
  }

  void complete(bool result) {
    deactivate();
    
    if (selection_changed) {
      get_program_table().force_program(active_selection);
    }
    active_selection = 0;
    selection_changed = false;
  }
  
  void update_display(AquariumDisplay& display) {
    char* buffer = get_buffer();

    display.setFont(System5x7);
    for (byte i=0; i<PROGRAM_TABLE_NUMBER_ENTRIES; i++) {
      display.setCursor(0, 2 + i);
      if (i == active_selection) {
        display.print("->");    
      } else {
        display.print("  ");            
      }
    }  
  }

  void display_static(AquariumDisplay& display) {
    char* buffer = get_buffer();

    display.setFont(System5x7);
    for (byte i=0; i<PROGRAM_TABLE_NUMBER_ENTRIES; i++) {
      ProgramEntry& program = get_program_table().get_program(i);
      display.setCursor(0, 2 + i);
      sprintf(buffer, "   %s %02d:%02d    ", program.get_name(), program.get_time().hour, program.get_time().minute);
      display.println(buffer);    
    }  
  }

};

AquariumViewProgramList __viewProgramList;

AquariumViewProgramList* get_view_program_list_ptr() {
  return &__viewProgramList;
}
