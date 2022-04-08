
#ifndef AQUARIUM_COLOR_CONTROL
#define AQUARIUM_COLOR_CONTROL


#define TRANSITION_ITERATIONS_NUM 16

class ColorTransition {
private:
  int current_red;
  int current_green;
  int current_blue;

  int target_red;
  int target_green;
  int target_blue;

  int delta_red;
  int delta_green;
  int delta_blue;

  bool in_transition;
  int transition_iteration;

  Color current_color;

public:
  
  ColorTransition() {
    current_red = 0;
    current_green = 0;
    current_blue = 0;

    delta_red = 0;
    delta_green = 0;
    delta_blue = 0;

    in_transition = false;
    transition_iteration = 0;
  }

//  void set_color(int r, int g, int b) {
//    // shifting is done for poor man floating point solution
//    current_red = r << 4;
//    current_green = g << 4;
//    current_blue = b << 4;
//  }
//
//  void set_color(Color& color) {
//    set_color(color.red, color.green, color.blue);
//  }

  void set_next_color(int r, int g, int b) {
    target_red = r << 4;
    target_green = g << 4;
    target_blue = b << 4;

    in_transition = true;
    transition_iteration = 0;

    delta_red = (target_red - current_red) / TRANSITION_ITERATIONS_NUM;
    delta_green = (target_green - current_green) /  TRANSITION_ITERATIONS_NUM;
    delta_blue = (target_blue - current_blue) /  TRANSITION_ITERATIONS_NUM;

    //sprintf(get_str(), "ColorTransition: next color %d %d %d", r, g, b);
    //Serial.println(get_str());
    //sprintf(get_str(), "ColorTransition: target %x %x %x ", target_red, target_green, target_blue);
    //Serial.println(get_str());
    //sprintf(get_str(), "ColorTransition: delta %x %x %x ", delta_red, delta_green, delta_blue);
    //Serial.println(get_str());
  }

  void set_next_color(Color& color) {
    set_next_color(color.red, color.green, color.blue);
  }

  bool is_active_transition() {
    return in_transition;
  }

  void step() {
    //sprintf(get_str(), "ColorTransition: step [%d] (%d) %x %x %x ", transition_iteration, in_transition, current_red, current_green, current_blue);
    //Serial.println(get_str());
    //sprintf(get_str(), "ColorTransition: target %x %x %x ", target_red, target_green, target_blue);
    //Serial.println(get_str());
    //sprintf(get_str(), "ColorTransition: delta %x %x %x ", delta_red, delta_green, delta_blue);
    //Serial.println(get_str());
    
    if (! in_transition) {
      return;
    }

    if ( transition_iteration == TRANSITION_ITERATIONS_NUM) {
      current_red = target_red;
      current_green = target_green;
      current_blue = target_blue;

      in_transition = false;
      transition_iteration = 0;

      delta_red = 0;
      delta_green = 0;
      delta_blue = 0;
      
      return;
    }
    
    current_red += delta_red;
    current_green += delta_green;
    current_blue += delta_blue;
    
    transition_iteration ++;
  }

  Color& color() {
    current_color.set(current_red >> 4, current_green >> 4, current_blue >> 4);
    return current_color;
  }
};

#define COLOR_PROGRAM_MAX_COLORS 3

const char* undef_str = "undef";

class ColorProgram {
private:
  char* name;
  Color color_list[COLOR_PROGRAM_MAX_COLORS];
  int number_of_colors;
  int current_color;
public:
  ColorProgram() {
    number_of_colors = 0;
    current_color = 0;
    name = (char*) undef_str;
  }

  void set(ColorProgram& program) {
    current_color = 0;
    name = program.name;
    number_of_colors = program.number_of_colors;
    for(int i=0; i < COLOR_PROGRAM_MAX_COLORS; i++) {
      color_list[i].set(program.color_list[i]);
    }
  }

  void set_name(char* _name) {
    name = _name;
  }
  
  char* get_name() {
    return name;
  }

  void color_list_reset(void) {
    number_of_colors = 0;
    current_color = 0;
    for(int i=0; i<COLOR_PROGRAM_MAX_COLORS; i++) {
      color_list[i].set(0, 0, 0);
    }
  }
  
  void color_list_add(Color& color) {
    if (number_of_colors >= COLOR_PROGRAM_MAX_COLORS) {
      return;
    }
    color_list[number_of_colors].set(color);
    number_of_colors++;
    current_color = 0;
  }

  Color& next_color(void) {
    current_color = (current_color + 1) % number_of_colors;
    return color_list[current_color];
  }

  bool changes_color(void) {
    return (number_of_colors>1);
  }

  Color& initial_color() {
    return color_list[0];
  }
};


class ColorControl {
private:
  ColorProgram color_program_current;
  ColorProgram color_program_next;

  bool program_in_transition;

  ColorTransition color_transition;

public:
  ColorControl() {
    program_in_transition = false;
  }

//  void init_program(ColorProgram& program) {
//      color_program_current.set(program);
//      color_transition.set_next_color(program.initial_color());
//  }

  void set_next_program(ColorProgram& program) {
//    sprintf(get_str(), "ColorControl: next program %s", program.get_name());
//    Serial.println(get_str());
    
    // transition between programs now
    program_in_transition = true;
    color_program_next.set(program);
    color_transition.set_next_color(color_program_next.initial_color());
  }

  void step() {
    color_transition.step();

    // if we are transitioning between programs
    if (program_in_transition) {
      if (!color_transition.is_active_transition()) {
        program_in_transition = false;
        color_program_current.set(color_program_next);
      }
      return;
    }
    
    // now changing colors in the same program if any colors to change
    if (!color_transition.is_active_transition() && color_program_current.changes_color()) {
      color_transition.set_next_color(color_program_current.next_color());
    }
  }

  bool program_transitioning() {
    return program_in_transition;
  }

  Color& color(void) {
    return color_transition.color();
  }
  
};

ColorControl __color_control;

ColorControl& get_color_control() {
  return __color_control;
}

#endif // AQUARIUM_COLOR_CONTROL
