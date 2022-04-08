
#ifndef AQUARIUM_RTC
#define AQUARIUM_RTC

#include <DS3231.h>
#include <Wire.h>

DS3231 rtc;

AquariumTime startTime;
AquariumTime current_time;

bool century = false;
bool h12Flag = false;
bool pmFlag;

void rtc_update_time() {
  current_time.year = rtc.getYear();
  current_time.month = rtc.getMonth(century);
  current_time.date = rtc.getDate();
  current_time.day = rtc.getDoW();
  current_time.hour = rtc.getHour(h12Flag, pmFlag);
  current_time.minute = rtc.getMinute();
  current_time.second = rtc.getSecond();  
}

void rtc_init() {
  Wire.begin();
  rtc_update_time();
  startTime.set(current_time);
}

AquariumTime& rtc_get_time() {
  return current_time;
}

AquariumTime& rtc_get_start_time() {
  return startTime;
}

void rtc_set_time(byte hour, byte minute, byte second) {
  rtc.setHour(hour);
  rtc.setMinute(minute);
  rtc.setSecond(second);
}

void rtc_set_date(byte month, byte date, byte year) {
  rtc.setMonth(month);
  rtc.setDate(date);
  rtc.setYear(year);
}

class AquariumTimerAlert {
private:
  int last_tick_second;
  int timer_second;
public:
  AquariumTimer() {
    last_tick_second = 0;
    timer_second = 0;
  }

  void reset(int seconds) {
    AquariumTime& now = rtc_get_time();
    last_tick_second = (now.minute * 60) + now.second;
    timer_second = seconds;
  }
  
  bool is_expired() {
    AquariumTime& now = rtc_get_time();
    int current_second = (now.minute * 60) + now.second;
    if (current_second < last_tick_second) {
      current_second += 3600;
    }
    int time_delta = current_second - last_tick_second;
    if (time_delta >= timer_second) {
      return true;
    }
    return false;
  }
};

class AquariumTimerMilliSecond {
private:
  unsigned last_tick_milli_second;
public:
  AquariumTimerMilliSecond() {
    last_tick_milli_second = 0;
  }
  
  bool time_ticked(unsigned current_millis_time, unsigned delta_milli_seconds) {
    
    unsigned current_milli_second_mod = current_millis_time % 10000;
    unsigned current_milli_second = current_milli_second_mod;

    if (current_milli_second < last_tick_milli_second) {
      current_milli_second += 10000;
    }
    unsigned time_delta = current_milli_second - last_tick_milli_second;
    
    //sprintf(get_str(), "DDD c1=%u c2=%u l=%u d=%u r=%u", current_milli_second_mod, current_milli_second, last_tick_milli_second, time_delta, delta_milli_seconds);
    //Serial.println(get_str());

    if (time_delta >= delta_milli_seconds) {
      last_tick_milli_second = current_milli_second_mod;
      return true;
    }
    return false;
  }
};

#endif // AQUARIUM_RTC
