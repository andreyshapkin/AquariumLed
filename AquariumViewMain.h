
#define MAX_VIEWS 10

char* get_view_name(byte index);

class AquariumViewMain: public AquariumViewBase {
private:
  byte active_selection;
  byte active_view;
  byte last_minute;
public:
  AquariumViewMain() {
    sprintf(name, "Vikusa Project");
    header_offset = 25;
  }
    
  void init() {
    active_selection = 0;
    active_view = 0;
    last_minute = 0xff;
  }
  void complete(bool result) {
    if (active_selection) {
      set_refresh();
    }
    init();
  }

  void update_control(RotaryKnob& knob) {
    if (knob.increment) {
      active_selection = increment_mod(active_selection, knob.increment, MAX_VIEWS);
      set_refresh();
      set_update();
    }
    if (knob.click_count) {
      active_view = active_selection;
      active_selection = 0;
    }
    //sprintf(get_str(), "AquariumViewMain: knob %d %d", active_view, active_selection);
    //Serial.println(get_str());
  }

  void step() {
    AquariumTime& now = rtc_get_time();
    if (last_minute != now.minute) {
      last_minute = now.minute;
      set_update();
    }
  }

  // returns if view need to be switched from main to something else
  byte get_active_view() {
    byte result = active_view;
    active_view = 0;
    return result;
  }
  
  void update_display(AquariumDisplay& display) {
    if (active_selection == 0) {
      display_main_view(display);      
    } else {
      display_menu_activity(display, get_view_name(active_selection));      
    }
  }

  void display_main_view(AquariumDisplay& display) {
    AquariumTime& now = rtc_get_time();
    ProgramEntry& program = get_program_table().get_active_program(); 
    Color& color = get_color_control().color();

    char* buffer = get_buffer();

//    //display.setFont(lcdnums12x16);
//    display.setFont(lcdnums14x24);
//    display.setCursor(10, 2);
//    sprintf(buffer, "%02d", now.hour);
//    display.print(buffer);
//    display.setCursor(50, 2);
//    sprintf(buffer, "%02d", now.minute);
//    display.print(buffer);
//    display.setCursor(90, 2);
//    sprintf(buffer, "%02d", now.second);
//    display.print(buffer);
//  
////    display.setFont(Callibri15);
//    display.setFont(Callibri14);
//    display.setCursor(0, 5);
//    sprintf(buffer, "P: %s             ", program.get_name());
//    display.println(buffer);
//  
//    display.setFont(System5x7);
//    display.setCursor(0, 7);
//    sprintf(buffer, "Color: %02x %02x %02x  ", color.red, color.green, color.blue);
//    display.println(buffer);    
    
    display.setFont(lcdnums14x24);
    display.setCursor(25, 2);
    sprintf(buffer, "%02d", now.hour);
    display.print(buffer);
    display.setCursor(65, 2);
    sprintf(buffer, "%02d", now.minute);
    display.print(buffer);
  
    display.setFont(Callibri15);
    display.setCursor(0, 6);
    sprintf(buffer, "P: %s             ", program.get_name());
    display.println(buffer);  
  }

  void display_menu_activity(AquariumDisplay& display, char* text) {
    char* buffer = get_buffer();

    display.setFont(Callibri15);
    display.setCursor(0, 4);
    sprintf(buffer, "> %s", text);
    display.println(buffer);  
  }
};

AquariumViewMain __viewMain;

AquariumViewMain* get_view_main_ptr() {
  return &__viewMain;
}


AquariumViewBase* viewList[MAX_VIEWS];
byte active_view_last_index = 0xff;

void init_views() {
  viewList[0] = get_view_main_ptr();
  viewList[1] = get_view_time_set_ptr();
  viewList[2] = get_view_date_set_ptr();
  viewList[3] = get_view_program_set_ptr();
  viewList[4] = get_view_program_list_ptr();
  viewList[5] = get_view_program_auto_ptr();
  viewList[6] = get_view_test_led_ptr();
  viewList[7] = get_view_custom_rgb();  
  viewList[8] = get_view_monitor_rgb();  
  viewList[9] = get_view_uptime_ptr();  

  viewList[0]->init_view(get_display());
}

AquariumViewBase* get_active_view() {

  // default is main view
  byte active_view_index = 0;
  
  // see if any other view is active
  for (byte i = 0; i<MAX_VIEWS; i++) {
    if (viewList[i]->active()) {
      active_view_index = i;
    }
  }

  // main view changed what next view should be called
  // this is called when main view was active and main view is changing the view
  byte main_next_view_index = get_view_main_ptr()->get_active_view();
  if (main_next_view_index) {
    active_view_index = main_next_view_index;  
  }
  
  AquariumViewBase* active_view = viewList[active_view_index];

  // active view changed, better init it
  if (active_view_index != active_view_last_index) {
    //sprintf(get_str(), "active view changed: %d => %d %s", active_view_last_index, active_view_index, active_view->name);
    //Serial.println(get_str());
    active_view->init_view(get_display());
    active_view_last_index = active_view_index;
  }
  return active_view;
}

const char* undef_text = "undef";
char* get_view_name(byte index) {
  if (index >= MAX_VIEWS) {
    return (char*) undef_text;
  }
  return viewList[index]->name;
}
