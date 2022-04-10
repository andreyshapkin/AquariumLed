class AquariumViewMonitorRGB: public AquariumViewBase {
private:
  byte active_selection;
public:
  AquariumViewMonitorRGB() {
    sprintf(name, "Monitor RGB");
  }
  
  void update_control(RotaryKnob& knob) {
    if (knob.click_count) {
      complete_view();
    }
  }

  void step() {
    set_update();
  }

  void update_display(AquariumDisplay& display) {
    Color& color = get_color_control().color();
    char* buffer = get_buffer();

    display.setCursor(25, 5);
    display.setFont(X11fixed7x14);
    sprintf(buffer, "%02x %02x %02x", color.red, color.green, color.blue);
    display.println(buffer); 
  }
};

AquariumViewMonitorRGB __viewMonitorRGB;

AquariumViewMonitorRGB* get_view_monitor_rgb() {
  return &__viewMonitorRGB;
}
