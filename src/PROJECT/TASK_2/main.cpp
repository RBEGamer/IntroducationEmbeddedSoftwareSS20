//------------ GENERAL INCLUDES -------------- //
#include <stdio.h>
#include <cmath> //FOR SIN
#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>
//------------ INCLUDES WIRING PI ------------ //
#include <wiringPi.h>
#define LOW 0
#define HIGH 1

//------------ INCLUDES MOTORHAT- ------------ //
#include "adafruitmotorhat.h"
#define FORWARD AdafruitDCMotor::kForward
#define BACKWARD AdafruitDCMotor::kBackward
#define RELEASE AdafruitDCMotor::kRelease
#define STOP RELEASE
//----------------- CONSTANT VARS AND DEFINES ---------- //
#define LED_LEFT 29  //LED LINKS AN PIN WIRING_29
#define LED_RIGHT 28 //LED RECHTS AN PIN WIRING_28


#define GPIO_ULTRASONIC_TRIGGER 0
#define GPIO_ULTRASONIC_ECHO 2
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
  clear_anything(); //CLEAR ANYTHING AFTER SIGNAL TO STOP MOTORS AND SENSORS
  exit(signum);
}



//SET MOTOR DIRECT BOTH TO FORWARD WITH A GIVEN SPEED
void drive_forward(int _speed)
{
  motor_left->setSpeed(_speed);
  motor_right->setSpeed(_speed);
  motor_left->run(FORWARD);
  motor_right->run(FORWARD);
}

void rotate_left(int _speed)
{
  motor_left->setSpeed(_speed);
  motor_right->setSpeed(_speed);
  motor_left->run(FORWARD);
  motor_right->run(BACKWARD);
}
void rotate_right(int _speed)
{
  motor_left->setSpeed(_speed);
  motor_right->setSpeed(_speed);
  motor_left->run(BACKWARD);
  motor_right->run(FORWARD);
}
//STOPS THE MOTORS BUT DOESNT RELEASE THEM
void drive_stop()
{
  motor_left->setSpeed(0);
  motor_right->setSpeed(0);
}


void breaking_light(bool _state){
   digitalWrite(LED_LEFT, _state);   
    digitalWrite(LED_RIGHT,_state);
}

//RETURNS THE DISTANCE TO A OBJECT IN CM USING THE ULTRASONIC SENSOR
float measure_distance(){
  //TRIGGER TRIGGER PIN = SEND AN ULTRASONIC SIGNAL
  digitalWrite(GPIO_ULTRASONIC_TRIGGER,HIGH);
  delay(0.1); //sleep .1ms
  digitalWrite(GPIO_ULTRASONIC_TRIGGER,LOW);

   while ( digitalRead(GPIO_ULTRASONIC_ECHO) == LOW );
  //SAVE START TRIGGER TIMER
  auto start = std::chrono::high_resolution_clock::now();
  //WAIT FOR ECHO TO BECOME HIGH
  while ( digitalRead(GPIO_ULTRASONIC_ECHO) == HIGH );
  //SAVE THE TIME OF THE RECEIEVED ECHO
  auto stop = std::chrono::high_resolution_clock::now();

//CALUCATE DELTA TIME IN MS
std::chrono::duration<double> TimeElapsed_s = stop - start;
double TimeElapsed_s_us = TimeElapsed_s.count()*1.0;
std::cout << "delattime : " <<TimeElapsed_s_us  << std::endl;
//CALCULCATE DISTANCE
float distanz = (17015.0*TimeElapsed_s_us); // 34400 is the speed of sound in cm/s; div by 2 = 17015
std::cout << "distanz :" << distanz << std::endl;

return distanz;
}


int main()
{
  
  //REGISTER SIGNAL HANDLER
  signal(SIGINT, signalHandler);
  //WIRING PI SETUP AND GPIO SETUP
  wiringPiSetup(); //SETUP WIRING PI
  pinMode(LED_LEFT, OUTPUT); //SET LED PIN TO OUTPUT
  pinMode(LED_RIGHT, OUTPUT); //SET LED PIN TO OUTPUT


  //ULTRASONIC SENSOR GPIO SETUP
  pinMode(GPIO_ULTRASONIC_ECHO, INPUT);
  pinMode(GPIO_ULTRASONIC_TRIGGER,OUTPUT);
  digitalWrite(GPIO_ULTRASONIC_TRIGGER, LOW);


  //MOTORHAT SETUP
  AdafruitMotorHAT hat; //INIT MOTORHAT
  //REQUEST MOTORS FROM THE HEAD
  motor_left = hat.getMotor(1);
  motor_right = hat.getMotor(2);

  //INIT MOTORS
  motor_left->setSpeed(0);
  motor_right->setSpeed(0);
  //CHECK MOTORS INSTANCE IS OK
  if (!motor_left && !motor_right)
  {
    clear_anything();
    return -1;
  }

  
 


  //SOME LED BLINKING AS START
  for (int i = 0; i < 3; i++)
  {
    led_state = !led_state;
    digitalWrite(LED_LEFT, led_state);   // On
    digitalWrite(LED_RIGHT, !led_state); // On
    delay(500);
  }


  for(;;){
  
  
      breaking_light(measure_distance() < 20.0);
    
    delay(100);
  }

  
  drive_stop();
  //CLEANUP ANYTHING
  clear_anything();

  return 0;
}