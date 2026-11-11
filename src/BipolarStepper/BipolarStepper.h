#ifndef BIPOLAR_STEPPER_H
#define BIPOLAR_STEPPER_H

#include <Arduino.h>

typedef enum
{
  e_Stepper_FullStep = 0,
  e_Stepper_HalfStep,
  e_Stepper_QuarterStep,
  e_Stepper_EighthStep
} MicrostepMode_t;

class BipolarStepper {
public:
  // Constructor
  BipolarStepper(
    int step_pin, 
    int dir_pin, 
    int enable_pin,
    int ms1_pin, 
    int ms2_pin);

  // Public instance variables
  bool continuous;

  // Public functions
  void step(unsigned long steps);
  void set_direction(int dir);
    

private:
  int step_pin;
  int dir_pin;
  int enable_pin;
  int ms1_pin;
  int ms2_pin;
  int direction;
  signed long number_of_steps;
  MicrostepMode_t microstep_mode;

  void set_microstep_mode(MicrostepMode_t mode);
};

#endif