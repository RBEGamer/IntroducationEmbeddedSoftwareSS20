//------------ GENERAL INCLUDES -------------- //
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>
//------------ INCLUDES WIRING PI ------------ //
#include <wiringPi.h>
//------------ INCLUDES MOTORHAT- ------------ //
#include "adafruitmotorhat.h"
#define FORWARD AdafruitDCMotor::kForward
#define BACKWARD AdafruitDCMotor::kBackward
#define RELEASE AdafruitDCMotor::kRelease
#define STOP RELEASE
//----------------- CONSTANT VARS AND DEFINES ---------- //
#define LED_LEFT 29  //LED LINKS AN PIN WIRING_29
#define LED_RIGHT 28 //LED RECHTS AN PIN WIRING_28

//--------------- GLOBALS ------------------ //
bool led_state = false;

std::shared_ptr<AdafruitDCMotor> motor_left = nullptr;
std::shared_ptr<AdafruitDCMotor> motor_right = nullptr;

//SHUTDOWN MOTORS, FREE ALL STUFF, RELEASE SENSORS ETC
void clear_anything()
{
  //------------- GPIO RELEASE --------------- //
  //note wiring pi has no cleanup -> so change all pins back to input
  pinMode(LED_LEFT, INPUT);
  pinMode(LED_RIGHT, INPUT);

  //------------ MOTOR RELEASE ------------- //
  if (motor_left)
  {
    motor_left->run(AdafruitDCMotor::kRelease); //STOPS LEFT MOTOR IF USED
  }
  if (motor_right)
  {
    motor_right->run(AdafruitDCMotor::kRelease); //STOPS RIGHT MOROE IF USED
  }
}
/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
  std::cout << "Strg-C Programmende" << std::endl;

  clear_anything();

  exit(signum);
}

int main()
{
  //REGISTER SIGNAL HANDLER
  signal(SIGINT, signalHandler);
  //WIRING PI SETUP AND GPIO SETUP
  wiringPiSetup();
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  //MOTORHAT SETUP
  AdafruitMotorHAT hat;

  //REQUEST MOTORS FROM THE HEAD
  motor_left = hat.getMotor(1);
  motor_right = hat.getMotor(2);


  //INIT MOTORS
  motor_left->setSpeed(0);
  motor_right->setSpeed(0);

  if(!motor_left && !motor_right){
    clear_anything();
    return -1;
  }

  // SET MOTOR TO FORWARD  WITH SPEED 60
  motor_left->setSpeed(60);
  motor_right->setSpeed(60);
  motor_left->run(FORWARD);
  motor_right->run(FORWARD);

  //SOME LED BLINKING
  for (;;)
  {
    led_state = !led_state;
    digitalWrite(LED_LEFT, led_state);   // On
    digitalWrite(LED_RIGHT, !led_state); // On
    delay(500);
  }

  //CLEANUP ANYTHING
  clear_anything();
  return 0;
}