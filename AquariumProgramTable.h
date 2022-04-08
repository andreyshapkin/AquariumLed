
#ifndef AQUARIUM_PROGRAM_TIME
#define AQUARIUM_PROGRAM_TIME


class ProgramTime {
public:
  int hour;
  int minute;

  ProgramTime() {
    hour = 0;
    minute = 0;
  }

  ProgramTime(int _hour, int _minute) {
    hour = _hour;
    minute = _minute;
  }

  bool match(ProgramTime& time) {
    return (time.hour == hour) && (time.minute == minute);
  }
  
  set(int _hour, int _minute) {
    hour = _hour;
    minute = _minute;
  }

  set(ProgramTime& time) {
    set(time.hour, time.minute);
  }
};



class ProgramEntry {
private:
  bool valid;
  char name[15];
  ProgramTime programTime;
  ColorProgram colorProgram;

public:
  ProgramEntry() {
    valid = false;
    sprintf(name,"undefined");
  }
  bool match(ProgramTime& time) {
    return (valid && programTime.match(time));
  }

  bool is_valid() {
    return valid;
  }

  void set(char* _name, ProgramTime& time, ColorProgram& color) {
    valid = true;
    sprintf(name, "%s", _name);
    programTime.set(time);
    colorProgram.set(color);
    colorProgram.set_name(_name);
  }

  ColorProgram& get_color_program() {
    return colorProgram;
  }

  char* get_name() {
    return name;
  }

  ProgramTime& get_time() {
    return programTime;
  }
  
};

#define PROGRAM_TABLE_NUMBER_ENTRIES 4

class ProgramTable {
private:
  ProgramEntry entries[PROGRAM_TABLE_NUMBER_ENTRIES];
  int active_entry;
  bool program_changed;
  
public:
  ProgramTable() {
    active_entry = 0;
    program_changed = false;
  }

  void step(ProgramTime& current_time) {
    int hit_entry = -1;
    for (int i=0; i < PROGRAM_TABLE_NUMBER_ENTRIES; i++) {
      ProgramEntry& entry = entries[i];
      if (entry.match(current_time)) {
        hit_entry = i;
      }
    }
    if (hit_entry == -1) {
      return;
    }
    if (hit_entry != active_entry) {
      active_entry = hit_entry;
      program_changed = true;
    }
  }

  bool is_program_changed() {
    bool result = program_changed;
    program_changed = false;
    return result;
  }

  void set_program(int index, char* name, ProgramTime& time, ColorProgram& color) {
    if (index >= PROGRAM_TABLE_NUMBER_ENTRIES) {
      return;
    }
    //sprintf(get_str(), "ProgramTable: set program %d %s", index, name);
    //Serial.println(get_str());
    entries[index].set(name, time, color);
  }

  void force_program(byte index) {
    if (index<PROGRAM_TABLE_NUMBER_ENTRIES && entries[index].is_valid()) {
      if (active_entry != index) {
        active_entry = index;
        program_changed = true;
        //sprintf(get_str(), "ProgramTable: forcing program %d %s", active_entry, entries[active_entry].get_name());
        //Serial.println(get_str());
      }
    }
  }

  // replay 24 hours of time to active current program according to schedule
  void activate_program_by_time(ProgramTime& current_time) {
    byte hour = current_time.hour;
    byte minute = current_time.minute;
    for (byte hour_i=0; hour_i<=24; hour_i++) {
      for (byte minute_i=0; minute_i<=60; minute_i++) {
        ProgramTime time((hour + hour_i) % 24, (minute + minute_i) % 60);
        step(time);
      }
    }
    
  }

  ProgramEntry& get_active_program() {
    return entries[active_entry];
  }

  byte get_active_program_index() {
    return active_entry;
  }

  ProgramEntry& get_program(byte index) {
    return entries[index];
  }
  
};

ProgramTable __program_table;

ProgramTable& get_program_table() {
  return __program_table;
}


#endif // AQUARIUM_PROGRAM_TIME
