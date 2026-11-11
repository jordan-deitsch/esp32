#include <DeviceSetup.h>

// Define constructors for stepper motors
BipolarStepper bipolar_stepper(STEP_PIN, DIR_PIN, ENABLE_PIN, MS1_PIN, MS2_PIN);
UnipolarStepper unipolar_stepper(COIL_A_PIN, COIL_B_PIN, COIL_C_PIN, COIL_D_PIN);

// Define constructor for servo motor
Servo myservo;
int servo_angle = 90;      // servo position in degrees