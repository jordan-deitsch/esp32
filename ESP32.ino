#include "DeviceSetup.h"
#include "Website.h"
#include "src/BipolarStepper/BipolarStepper.h"
#include "src/UnipolarStepper/UnipolarStepper.h"
#include <ESP32Servo.h>

void setup() {
  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(SERVO_PIN, SERVO_MIN_US, SERVO_MAX_US);
  myservo.write(servo_angle);

  // Setup serial port
  Serial.begin(115200);
  Serial.println("Board configured");

  // Setup webserver
  Website_setup();
}

void loop() {
  Website_loop();

  // if (Serial.available() > 0) {
  //   int angle = Serial.parseInt();

  //   // Only update if a valid angle was received
  //   if (angle >= 0 && angle <= 180) {
  //     myservo.write(angle);

  //      Serial.print("Servo angle: ");
  //      Serial.println(angle);
  //   }
  // }

  // // Step unipolar stepper
  // unipolar_stepper.step(1);

  if(bipolar_stepper.continuous) bipolar_stepper.step(1);
  if(unipolar_stepper.continuous) unipolar_stepper.step(1);
  
  delay(10);

}


