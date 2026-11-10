#include "DeviceSetup.h"
#include "src/BipolarStepper/BipolarStepper.h"
#include "src/UnipolarStepper/UnipolarStepper.h"
#include <ESP32Servo.h>


// Servo control: published values for SG90 servos, adjust if needed
Servo myservo;
int minUs = 1000;
int maxUs = 2000;
int pos = 0;      // servo position in degrees

BipolarStepper bipolar_stepper(STEP_PIN, DIR_PIN, ENABLE_PIN, MS1_PIN, MS2_PIN);
UnipolarStepper unipolar_stepper(COIL_A_PIN, COIL_B_PIN, COIL_C_PIN, COIL_D_PIN);

void setup() {
  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(SERVO_PIN, minUs, maxUs);

  // Setup serial port
  Serial.begin(9600);
  Serial.println("Board configured");
}

void loop() {

  if (Serial.available() > 0) {
    int angle = Serial.parseInt();

    // Only update if a valid angle was received
    if (angle >= 0 && angle <= 180) {
      myservo.write(angle);

       Serial.print("Servo angle: ");
       Serial.println(angle);
    }
    
    bipolar_stepper.step(500);
  }

  // Step unipolar stepper
  unipolar_stepper.step(1);
  delay(1000);

}


