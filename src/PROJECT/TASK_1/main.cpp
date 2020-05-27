//------------ GENERAL INCLUDES -------------- //
#include <stdio.h>
#include <cmath> //FOR SIN
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

void drive_forward(int _speed)
{
  motor_left->setSpeed(_speed);
  motor_right->setSpeed(_speed);
  motor_left->run(FORWARD);
  motor_right->run(FORWARD);
}



void drive_sine_wave(int _base_speed = 70, int _sine_multiplier = 40, int _delay = 7)
{
  motor_left->run(FORWARD);
  motor_right->run(FORWARD);
  for (int i = 0; i < 360*5; i++)
  {
    float s = std::sin(i/100.0)*0.8;

    motor_right->setSpeed(_base_speed + _sine_multiplier * s);
    motor_left->setSpeed(_base_speed  + (1.0-_sine_multiplier) * s);

    delay(_delay);
  }

  motor_left->run(STOP);
  motor_right->run(STOP);
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

  if (!motor_left && !motor_right)
  {
    clear_anything();
    return -1;
  }

  // SET MOTOR TO FORWARD  WITH SPEED 60
  //drive_forward(70);
  //delay(500);
  //drive_rectangle(70);
  //delay(2000);
  //drive_sine_wave(70);
  //delay(2000);

  motor_left->run(AdafruitDCMotor::kRelease);
  motor_right->run(AdafruitDCMotor::kRelease);
  //SOME LED BLINKING
  for (int i = 0; i < 3; i++)
  {
    led_state = !led_state;
    digitalWrite(LED_LEFT, led_state);   // On
    digitalWrite(LED_RIGHT, !led_state); // On
    delay(500);
  }

  drive_sine_wave();
  //CLEANUP ANYTHING
  clear_anything();

  return 0;
}