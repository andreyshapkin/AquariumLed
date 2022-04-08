#ifndef AQUARIUM_ROTARY_KNOB
#define AQUARIUM_ROTARY_KNOB

#include <EncoderButton.h>

EncoderButton eb(2, 3, 4);

/**
 * Instatiate an EncoderButton.
 * For Arduino Uno, the hardware interrupts are pins 2 & 3
 * For Teensy, you can use any digital pin.
 * Probably better to pick a more meaningful name than 'eb1'...
 * Encoder+button:
 * EncoderButton(byte encoderPin1, byte encoderPin2, byte switchPin);
 * Encoder only:
 * EncoderButton(byte encoderPin1, byte encoderPin2);
 * Button only:
 * EncoderButton(byte switchPin);
 */

class RotaryKnob {
  public:
    byte click_count;
    int increment;
    //int position;

    RotaryKnob() {
      reset();
    };

    void reset() {
      click_count = 0;
      increment = 0;
      //position = 0;      
    }

    void copy(RotaryKnob& knob) {
      click_count = knob.click_count;
      // inversion changes direction (clockwise: '+', counterclockwise: '-')
      increment = - knob.increment;
      //position = - knob.position;
    }
};

RotaryKnob knob_current_state;
RotaryKnob knob_snapshot;

void onEb1Clicked(EncoderButton& eb) {
  //Serial.print("eb1 clickCount: ");
  //Serial.println(eb.clickCount());
  knob_current_state.click_count += eb.clickCount();
}

void onEb1Encoder(EncoderButton& eb) {
  //Serial.print("eb1 incremented by: ");
  //Serial.println(eb.increment());
  //Serial.print("eb1 position is: ");
  //Serial.println(eb.position());
  knob_current_state.increment += eb.increment();
  //knob_current_state.position = eb.position();
}

void rotary_knob_init() {
  //Link the event(s) to your function
  eb.setClickHandler(onEb1Clicked);
  eb.setEncoderHandler(onEb1Encoder);
  knob_current_state.reset();
  knob_snapshot.reset();
}

RotaryKnob& rotary_knob_update() {
  eb.update();
  
  // disabling interrupts to avoid data corruption
  noInterrupts();
  
  knob_snapshot.copy(knob_current_state);
  knob_current_state.reset();
  
  // re-enabling interrupts
  interrupts();
  
  return knob_snapshot;
}

#endif // AQUARIUM_ROTARY_KNOB
