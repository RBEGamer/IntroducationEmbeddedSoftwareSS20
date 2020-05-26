#include <stdio.h>
#include <wiringPi.h>

#include <thread>
#include <chrono>
#include "adafruitmotorhat.h"

#define LED_LEFT 29  //LED LINKS AN PIN WIRING_29
#define LED_RIGHT 28 //LED RECHTS AN PIN WIRING_28

bool led_state = false;

int main()
{
 
  wiringPiSetup();

  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  // connect using the default device address 0x60
  AdafruitMotorHAT hat;

  // get the motor connected to port 1
  auto motor_left = hat.getMotor(1);
  auto motor_right = hat.getMotor(2);

  motor_left->setSpeed(75);
  motor_right->setSpeed(75);

  motor_left->run(AdafruitDCMotor::kForward);
  motor_right->run(AdafruitDCMotor::kBackward);

  for (;;)
  {

    led_state = !led_state;
    digitalWrite(LED_LEFT, led_state);   // On
    digitalWrite(LED_RIGHT, !led_state); // On
    delay(500);

     
    
    // speed must be set before running commands
   

    

    // release the motor after use
   
  }
  motor_left->run(AdafruitDCMotor::kRelease);
  motor_right->run(AdafruitDCMotor::kRelease);
  return 0;
}