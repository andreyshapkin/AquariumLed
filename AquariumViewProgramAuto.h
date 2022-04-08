

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

AquariumViewProgramAuto __viewProgramAuto;

AquariumViewProgramAuto* get_view_program_auto_ptr() {
  return &__viewProgramAuto;
}
