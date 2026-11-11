#ifndef DEVICE_SETUP_H
#define DEVICE_SETUP_H

#include "src/BipolarStepper/BipolarStepper.h"
#include "src/UnipolarStepper/UnipolarStepper.h"
#include <ESP32Servo.h>

// ===========================
// Define Bipolar Stepper Motor Driver pins
// ===========================
#define STEP_PIN    (14) 
#define DIR_PIN     (27)
#define MS1_PIN     (26) 
#define MS2_PIN     (25)
#define ENABLE_PIN  (33) 

// ===========================
// Define Unipolar Stepper Motor Driver pins
// ===========================
#define COIL_A_PIN (4)
#define COIL_B_PIN (16)   // RX2
#define COIL_C_PIN (17)   // TX2
#define COIL_D_PIN (18)

// ===========================
// Define Servo pins and timing values
// ===========================
#define SERVO_PIN (32)
#define SERVO_MIN_US (1000)
#define SERVO_MAX_US (2000)

// ===========================
// Declare motor instances
// ===========================
extern BipolarStepper bipolar_stepper;
extern UnipolarStepper unipolar_stepper;
extern Servo myservo;
extern int servo_angle;      // servo position in degrees


#endif