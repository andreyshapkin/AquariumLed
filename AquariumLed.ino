
//#define __DEBUG__

#include <Wire.h>
#include <avr/wdt.h>
#include "AquariumCommon.h"
#include "AquariumRTC.h"
#include "AquariumSaveBootDate.h"
#include "AquariumColorControl.h"
#include "AquariumProgramTable.h"
#include "AquariumColorDriver.h"
#include "AquariumRotaryKnob.h"
#include "AquariumDisplayASCII.h"
#include "AquariumViewSetTime.h"
#include "AquariumViewSetDate.h"
#include "AquariumViewProgramList.h"
#include "AquariumViewTestLED.h"
#include "AquariumViewProgramAuto.h"
#include "AquariumViewCustomRGB.h"
#include "AquariumViewUptime.h"
#include "AquariumViewMain.h"


void setup() {
  //Serial.begin(4800);

  //Serial.println(F("Setup started"));

  //Serial.println(F("Initializing RTC"));
  rtc_init();
  //Serial.println(F("Initializing display"));
  display_init();
  //Serial.println(F("Initializing EEPROM"));
  save_boot_init();
  //Serial.println(F("Initializing program_table"));
  init_program_table();
  //Serial.println(F("Initializing color driver"));
  color_driver_init();
  //Serial.println(F("Initializing rotary knob"));
  rotary_knob_init();
  //Serial.println(F("Initializing view controls"));
  init_views();
  //Serial.println(F("Initializing watchdog"));
  wdt_enable(WDTO_8S);
  //Serial.println(F("All Good!"));
}

AquariumTimerMilliSecond poll_slow_loop;
AquariumTimerMilliSecond poll_color_control_loop;
AquariumTimerMilliSecond poll_display_update;

void loop() {
  // reset watchdog
  wdt_reset();
  
  unsigned time_millis = millis();
  //sprintf(get_str(), "Time millis %u", time_millis);
  //Serial.println(get_str());
  
  if (poll_slow_loop.time_ticked(time_millis, 1000)) {
    //unsigned millis_last = millis();
    rtc_update_time();
    poll_program_update();
    //sprintf(get_str(), "SlowLoop: %u", millis() - millis_last);
    //Serial.println(get_str());
  }

  if (poll_color_control_loop.time_ticked(time_millis,500)) {
    //unsigned millis_last = millis();
    poll_color_control();
    //sprintf(get_str(), "Color: %u", millis() - millis_last);
    //Serial.println(get_str());
  }

  RotaryKnob& knob = rotary_knob_update();
  if (knob.click_count || knob.increment) {
    //sprintf(str, "knob clicked %d %d", knob.click_count, knob.increment);
    //Serial.println(str);  
    get_active_view()->update_control_view(knob);
  }

  if (poll_display_update.time_ticked(time_millis, 250)) {
    //unsigned millis_last = millis();
    get_active_view()->update_display_view(get_display());
    //sprintf(get_str(), "Display: %u", millis() - millis_last);
    //Serial.println(get_str());
  }
}

void poll_program_update() {
  AquariumTime& time_now = rtc_get_time();
  ProgramTime time_p(time_now.hour, time_now.minute);

  ProgramTable& program_table = get_program_table();

  // check if need to switch color program from the table
  program_table.step(time_p);
  bool program_changed = program_table.is_program_changed();
  //sprintf(get_str(), "Active program %s", program_table.get_active_program().get_name());
  //Serial.println(get_str());  
  
  if (program_changed) {
    ColorControl& colorControl = get_color_control();
    colorControl.set_next_program(program_table.get_active_program().get_color_program());
    //sprintf(get_str(), "Active program changed %s", program_table.get_active_program().get_name());
    //Serial.println(get_str());
  }
}

void poll_color_control() {
  ColorControl& colorControl = get_color_control();
  colorControl.step();
  Color& current_color = colorControl.color();
  //sprintf(get_str(), "Current color %d %d %d", current_color.red, current_color.green, current_color.blue);
  //Serial.println(get_str());

  write_rgb(current_color.red, current_color.green, current_color.blue);
}


void init_program_table() {
  int index = 0;
  Color color;
  ColorProgram program_color;
  ProgramTime program_time;

  ProgramTable& program_table = get_program_table();
  ColorControl& colorControl = get_color_control();

  program_time.set(7,0);
  program_color.color_list_reset();
//  program_color.color_list_add(color.get(255,15,192));
//  program_color.color_list_add(color.get(255,15,150));
  program_color.color_list_add(color.get(127,8,50));
  program_color.color_list_add(color.get(127,8,20));
  program_table.set_program(index++, "Sunrise", program_time, program_color);

  program_time.set(8,0);
  program_color.color_list_reset();
  program_color.color_list_add(color.get(128,128,128));
  program_color.color_list_add(color.get(128,100,150));
  program_color.color_list_add(color.get(128,150,100));
  program_table.set_program(index++, "Daytime", program_time, program_color);

  program_time.set(20,0);
  program_color.color_list_reset();
//  program_color.color_list_add(color.get(255,15,192));
//  program_color.color_list_add(color.get(255,15,150));
  program_color.color_list_add(color.get(127,8,50));
  program_color.color_list_add(color.get(127,8,20));
  program_table.set_program(index++, "Sunset", program_time, program_color);

  program_time.set(21,0);
  program_color.color_list_reset();
//  program_color.color_list_add(color.get(100,100,255));
//  program_color.color_list_add(color.get(100,200,255));
  program_color.color_list_add(color.get(5,5,50));
  program_color.color_list_add(color.get(5,25,50));
  program_table.set_program(index++, "Night", program_time, program_color);

  AquariumTime& now = rtc_get_time();
  program_time.set(now.hour, now.minute);
  program_table.activate_program_by_time(program_time);
  
  ProgramEntry& active_program = program_table.get_active_program();
  colorControl.set_next_program(active_program.get_color_program());
}
