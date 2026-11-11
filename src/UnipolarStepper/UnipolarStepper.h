#ifndef UNIPOLAR_STEPPER_H
#define UNIPOLAR_STEPPER_H

#include <Arduino.h>

class UnipolarStepper {
public:
  // Constructor
  UnipolarStepper(
    int coil_a_pin, 
    int coil_b_pin, 
    int coil_c_pin,
    int coil_d_pin);

  // Public instance variables
  bool continuous;

  // Public functions
  void step(unsigned long steps);
  void set_direction(int dir);
    

private:
  int coil_a_pin;
  int coil_b_pin;
  int coil_c_pin;
  int coil_d_pin;
  int direction;
  uint8_t position;  // DD_CC_BB_AA
  signed long number_of_steps;

  void drive_output();
};

#endif