#include "UnipolarStepper.h"

#define UNIPOLAR_COIL_A_MASK (0x03)
#define UNIPOLAR_COIL_B_MASK (0x0C)
#define UNIPOLAR_COIL_C_MASK (0x30)
#define UNIPOLAR_COIL_D_MASK (0xC0)

// Constructor
UnipolarStepper::UnipolarStepper(
      int coil_a_pin,
      int coil_b_pin,
      int coil_c_pin,
      int coil_d_pin) {
  this->coil_a_pin = coil_a_pin;
  this->coil_b_pin = coil_b_pin;
  this->coil_c_pin = coil_c_pin;
  this->coil_d_pin = coil_d_pin;

  position = UNIPOLAR_COIL_A_MASK;  // Initialize position
  number_of_steps = 0;  // Initialize step counter
  continuous = false; // Initialize to not run in continuous mode

  // Set pin mode for all control pins
  pinMode(coil_a_pin, OUTPUT);
  pinMode(coil_b_pin, OUTPUT);
  pinMode(coil_c_pin, OUTPUT);
  pinMode(coil_d_pin, OUTPUT);

  set_direction(1); // Set default direction
  drive_output(); // Initialize output coils
}


// Set motor direction
void UnipolarStepper::set_direction(int dir) {
  if (dir == 1 || dir == -1) direction = dir;
  else Serial.println("ERROR: Stepper motor invalid direction");
}


// Generate step pulses
void UnipolarStepper::step(unsigned long steps) {
  for (unsigned long i = 0; i < steps; i++) {
    if (direction == 1) {
      position = (position << 1) | (position >> 7);
    }
    else if (direction == -1) {
      position = (position >> 1) | (position << 7);
    }
    
    number_of_steps = number_of_steps + direction;

    drive_output();
    delayMicroseconds(500);
  }
}

// Translate position byte into pin outputs
void UnipolarStepper::drive_output() {
  ((position & UNIPOLAR_COIL_A_MASK) != 0) ? digitalWrite(coil_a_pin, HIGH) : digitalWrite(coil_a_pin, LOW);
  ((position & UNIPOLAR_COIL_B_MASK) != 0) ? digitalWrite(coil_b_pin, HIGH) : digitalWrite(coil_b_pin, LOW); 
  ((position & UNIPOLAR_COIL_C_MASK) != 0) ? digitalWrite(coil_c_pin, HIGH) : digitalWrite(coil_c_pin, LOW); 
  ((position & UNIPOLAR_COIL_D_MASK) != 0) ? digitalWrite(coil_d_pin, HIGH) : digitalWrite(coil_d_pin, LOW);  
}