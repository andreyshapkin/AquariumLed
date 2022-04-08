#ifndef AQUARIUM_SAVE_BOOT
#define AQUARIUM_SAVE_BOOT

#include <EEPROM.h>

AquariumTime eeprom_data_temp;

#define SAVE_DATE_MAX_ENTRY_NUM 7

void eeprom_save_boot_time(int entry_num, AquariumTime& boot_time) {
  if (entry_num > SAVE_DATE_MAX_ENTRY_NUM) {
    return;
  }
  byte* data_pointer = (byte*) &boot_time;
  const int object_size = sizeof(eeprom_data_temp);
  
  int address_offset = entry_num * object_size;
  for (int i=0; i < object_size; i++) {
    EEPROM.write(address_offset + i, data_pointer[i]);
  }
}

AquariumTime& eeprom_read_boot_time(int entry_num) {
  if (entry_num > 7) {
    return eeprom_data_temp;
  }
  byte* data_pointer = (byte*) &eeprom_data_temp;
  const int object_size = sizeof(eeprom_data_temp);

  int address_offset = entry_num * object_size;
  for (int i=0; i < object_size; i++) {
    data_pointer[i] = EEPROM.read(address_offset + i);
  }
  return eeprom_data_temp;
}

void eeprom_push_boot_time(AquariumTime& boot_time) {
  for (int i = SAVE_DATE_MAX_ENTRY_NUM - 1; i>=0; i--) {
    AquariumTime& eeprom_temp = eeprom_read_boot_time(i);
    eeprom_save_boot_time(i + 1, eeprom_temp);
  }
  eeprom_save_boot_time(0, boot_time);
}

void save_boot_init() {
  eeprom_push_boot_time(rtc_get_start_time());
}

#endif // AQUARIUM_SAVE_BOOT
