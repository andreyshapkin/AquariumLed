
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
#include "AquariumSetupProgramTable.h"
#include "AquariumViewSetTime.h"
#include "AquariumViewSetDate.h"
#include "AquariumViewSetProgram.h"
#include "AquariumViewProgramList.h"
#include "AquariumViewTestLED.h"
#include "AquariumViewProgramAuto.h"
#include "AquariumViewCustomRGB.h"
#include "AquariumViewUptime.h"
#include "AquariumViewMonitorRGB.h"
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
  
  if (poll_slow_loop.time_ticked(time_millis, 5000)) {
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
