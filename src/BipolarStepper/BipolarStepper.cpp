#include "BipolarStepper.h"

// Constructor
BipolarStepper::BipolarStepper(
      int step_pin, 
      int dir_pin, 
      int enable_pin,
      int ms1_pin, 
      int ms2_pin) {
  this->step_pin = step_pin;
  this->dir_pin = dir_pin;
  this->enable_pin = enable_pin;
  this->ms1_pin = ms1_pin;
  this->ms2_pin = ms2_pin;

  // Initialize step counter
  number_of_steps = 0;
  continuous = false;

  // Set pin mode for all control pins
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(enable_pin, OUTPUT);
  pinMode(ms1_pin, OUTPUT);
  pinMode(ms2_pin, OUTPUT);

  set_direction(1); // Set default direction
  set_microstep_mode(e_Stepper_FullStep); // Set default mode
  digitalWrite(enable_pin, LOW);  // Active low enable
}


// Set motor direction
void BipolarStepper::set_direction(int dir) {
  direction = dir;
  if (direction == 1) digitalWrite(dir_pin, HIGH);
  else if (direction == -1) digitalWrite(dir_pin, LOW);
  else Serial.println("ERROR: Stepper motor invalid direction");
}


// Generate step pulses
void BipolarStepper::step(unsigned long steps) {
  for (unsigned long i = 0; i < steps; i++) {
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(500);

    digitalWrite(step_pin, LOW);
    delayMicroseconds(500);

    // Inrement step counter for absolute position measurement
    number_of_steps = number_of_steps + direction;
  }
}


// Set microstep mode
void BipolarStepper::set_microstep_mode(MicrostepMode_t mode) {
    microstep_mode = mode;

  switch(mode) {
    case e_Stepper_FullStep:
      digitalWrite(ms1_pin, LOW);
      digitalWrite(ms2_pin, LOW);
      break;
    
    case e_Stepper_HalfStep:
      digitalWrite(ms1_pin, HIGH);
      digitalWrite(ms2_pin, LOW);
      break;
    
    case e_Stepper_QuarterStep:
      digitalWrite(ms1_pin, LOW);
      digitalWrite(ms2_pin, HIGH);
      break;
    
    case e_Stepper_EighthStep:
      digitalWrite(ms1_pin, HIGH);
      digitalWrite(ms2_pin, HIGH);
      break;
    
    // Deafult ot Full Step mode
    default:
      digitalWrite(ms1_pin, LOW);
      digitalWrite(ms2_pin, LOW);
      microstep_mode = e_Stepper_FullStep;
      break;
  }
}