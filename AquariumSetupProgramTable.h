
#include <EEPROM.h>


#define EEPROM_START_ADDRESS 256
#define EEPROM_HEADER_SIZE 8
#define EEPROM_DATA_SIZE 2
#define EEPROM_DATA_ADDRESS (EEPROM_START_ADDRESS + EEPROM_HEADER_SIZE)

enum SelectionType {
  PROGRAM_SUNRISE,
  PROGRAM_DAYTIME,
  PROGRAM_SUNSET,
  PROGRAM_NIGHT,
  PROGRAM_ENUM_SIZE
};

ProgramTime _temp_time;

void eeprom_save_program_time(byte entry_num, ProgramTime& time) {
  EEPROM.write(EEPROM_DATA_ADDRESS + (EEPROM_DATA_SIZE * entry_num), time.hour);
  EEPROM.write(EEPROM_DATA_ADDRESS + (EEPROM_DATA_SIZE * entry_num) + 1, time.minute);
}

void eeprom_save_program_time(byte entry_num, byte hour, byte minute) {
  _temp_time.set(hour, minute);
  eeprom_save_program_time(entry_num, _temp_time);
}

ProgramTime& eeprom_read_program_time(byte entry_num) {
  _temp_time.hour = EEPROM.read(EEPROM_DATA_ADDRESS + (EEPROM_DATA_SIZE * entry_num));
  _temp_time.minute = EEPROM.read(EEPROM_DATA_ADDRESS + (EEPROM_DATA_SIZE * entry_num) + 1);
  return _temp_time;
}

void eeprom_init_if_empty() {
  // checking for magic signature,data 0->7
  bool check_passed = true;
  for (byte i=0; i<EEPROM_HEADER_SIZE; i++) {
    byte data = EEPROM.read(EEPROM_START_ADDRESS + i);
    if (data != i) {
      check_passed = false;
    }
  }
  if (check_passed) {
    return;
  }

  // initialize header
  for (byte i=0; i<EEPROM_HEADER_SIZE; i++) {
    EEPROM.write(EEPROM_START_ADDRESS + i, i);
  }

  // initialize_time
  eeprom_save_program_time(PROGRAM_SUNRISE, 7, 0);
  eeprom_save_program_time(PROGRAM_DAYTIME, 8, 0);
  eeprom_save_program_time(PROGRAM_SUNSET, 20, 0);
  eeprom_save_program_time(PROGRAM_NIGHT, 21, 0);
}


void init_program_table() {
  eeprom_init_if_empty();

  Color color;
  ColorProgram program_color;
  ProgramTime program_time;

  ProgramTable& program_table = get_program_table();
  ColorControl& colorControl = get_color_control();

  program_time.set(eeprom_read_program_time(PROGRAM_SUNRISE));
  program_color.color_list_reset();
//  program_color.color_list_add(color.get(127,8,50));
//  program_color.color_list_add(color.get(127,8,20));
  program_color.color_list_add(color.get(127,8,50));
  program_color.color_list_add(color.get(127,8,20));
  program_table.set_program(PROGRAM_SUNRISE, "Sunrise", program_time, program_color);

  program_time.set(eeprom_read_program_time(PROGRAM_DAYTIME));
  program_color.color_list_reset();
//  program_color.color_list_add(color.get(128,128,128));
//  program_color.color_list_add(color.get(128,100,150));
//  program_color.color_list_add(color.get(128,150,100));
  program_color.color_list_add(color.get(100,80,120));
  program_color.color_list_add(color.get(100,120,80));
  program_table.set_program(PROGRAM_DAYTIME, "Daytime", program_time, program_color);

  program_time.set(eeprom_read_program_time(PROGRAM_SUNSET));
  program_color.color_list_reset();
  program_color.color_list_add(color.get(127,8,50));
  program_color.color_list_add(color.get(127,8,20));
  program_table.set_program(PROGRAM_SUNSET, "Sunset", program_time, program_color);

  program_time.set(eeprom_read_program_time(PROGRAM_NIGHT));
  program_color.color_list_reset();
  program_color.color_list_add(color.get(5,5,50));
  program_color.color_list_add(color.get(5,25,50));
  program_table.set_program(PROGRAM_NIGHT, "Night", program_time, program_color);

  AquariumTime& now = rtc_get_time();
  program_time.set(now.hour, now.minute);
  program_table.activate_program_by_time(program_time);
  
  ProgramEntry& active_program = program_table.get_active_program();
  colorControl.set_next_program(active_program.get_color_program());
}
