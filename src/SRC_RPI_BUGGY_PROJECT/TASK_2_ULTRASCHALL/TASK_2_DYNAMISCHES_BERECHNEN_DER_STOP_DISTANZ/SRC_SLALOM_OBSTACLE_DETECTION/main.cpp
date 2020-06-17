//------------ GENERAL INCLUDES -------------- //
#include <stdio.h>
#include <cmath> //FOR SIN
#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm> //for std::max
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

#define GPIO_ULTRASONIC_TRIGGER 0 // GPIO ULTRASCONIC SENSOR TRIGGER PIN
#define GPIO_ULTRASONIC_ECHO 2// GPIO ULTRASCONIC SENSOR ECHO PIN

//---------- FOR THE BREAKING DISTANCE CALCULATION ---------- //
#define MINIMAL_EMG_BREAKING_DISTANCE 15.0
#define MAX_ADDITIVE_BREAKING_DISTANCE 10.0
//--------------- GLOBALS ------------------ //

std::shared_ptr<AdafruitDCMotor> motor_left = nullptr;
std::shared_ptr<AdafruitDCMotor> motor_right = nullptr;

float minimal_emergency_breaking_distance = 20.0;

//SHUTDOWN MOTORS, FREE ALL STUFF, RELEASE SENSORS ETC
void clear_anything()
{
  //------------- GPIO RELEASE --------------- //
  //note wiring pi has no cleanup -> so change all pins back to input
  pinMode(LED_LEFT, INPUT);
  pinMode(LED_RIGHT, INPUT);
   pinMode(GPIO_ULTRASONIC_TRIGGER,INPUT);

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




float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//CALCULATES
float calculate_minimal_emergency_breaking_distance(int _speed_left, int _speed_right){

if(_speed_left < 0 && _speed_right < 0){
  _speed_left  = _speed_left * -1;
  _speed_right = _speed_right * -1;

}else if(_speed_left >= 0 && _speed_right >= 0){

}else{
  minimal_emergency_breaking_distance = MINIMAL_EMG_BREAKING_DISTANCE;
  return minimal_emergency_breaking_distance;
}


int max = std::max(_speed_left,_speed_right); //GET THE FASTER MOTOR OF BOTH MOTORS

minimal_emergency_breaking_distance = MINIMAL_EMG_BREAKING_DISTANCE + map(max,0.0,255.0,0.0,MAX_ADDITIVE_BREAKING_DISTANCE); //ADD UP TO 10CM ACCORDING THE SPEED TO THE 
return minimal_emergency_breaking_distance;
}




void set_motor_speed(int _speed_left, int _speed_right){
  calculate_minimal_emergency_breaking_distance(_speed_left,_speed_right);

   motor_left->setSpeed(_speed_left);
  motor_right->setSpeed(_speed_right);

  if(_speed_left == 0 && _speed_right == 0){
     motor_left->run(STOP);
      motor_right->run(STOP);
  }

  if(_speed_left < 0){
    motor_left->run(BACKWARD);
  }else{
     motor_left->run(FORWARD);
  }
 
  if(_speed_right < 0){
    motor_right->run(BACKWARD);
  }else{
     motor_right->run(FORWARD);
  }

}

//SET MOTOR DIRECT BOTH TO FORWARD WITH A GIVEN SPEED
void drive_forward(int _speed)
{
 set_motor_speed(_speed,_speed);
}
void drive_backward(int _speed)
{
 set_motor_speed(-_speed,-_speed);
}

void rotate_left(int _speed)
{
  set_motor_speed(_speed,-_speed);
}
void rotate_right(int _speed)
{
   set_motor_speed(-_speed,_speed);
}
//STOPS THE MOTORS BUT DOESNT RELEASE THEM
void drive_stop()
{
   set_motor_speed(0,0);
}


void breaking_light(bool _state){
   digitalWrite(LED_LEFT, _state);   
    digitalWrite(LED_RIGHT,_state);
}

void blink_leds(){
   bool led_state = false;
  for (int i = 0; i < 3; i++)
  {
    led_state = !led_state;
    digitalWrite(LED_LEFT, led_state);   // On
    digitalWrite(LED_RIGHT, !led_state); // On
    delay(500);
  }
}



void drive_obstacle_avoidance(){
  const int basic_speed = 110;
    rotate_left(basic_speed);
    delay(1200);
    drive_forward(basic_speed);
     delay(1200);
      rotate_right(basic_speed);
       delay(1200);
      
    drive_forward(basic_speed);
      delay(1200*2);
      rotate_right(basic_speed);
      delay(1200);
       drive_forward(basic_speed);
      delay(1200);
      rotate_left(basic_speed);
        delay(1200);
    drive_forward(basic_speed);
}


int sine_wave_step_counter = 0; //counts from 0 to 360
void drive_sine_wave_step(int _base_speed = 70, int _sine_multiplier = 40, int _delay = 7)
{
    motor_left->run(FORWARD);
    motor_right->run(FORWARD);
  
    float s = std::sin(sine_wave_step_counter/100.0)*0.8;

    motor_right->setSpeed(_base_speed + _sine_multiplier * s);
    motor_left->setSpeed(_base_speed  + (1.0-_sine_multiplier) * s);

   
  sine_wave_step_counter++;
  if(sine_wave_step_counter >= 360){
    sine_wave_step_counter = 0;
  }

  
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
  blink_leds();


  for(;;){
  
      drive_sine_wave_step();
      if(measure_distance() < minimal_emergency_breaking_distance){
          breaking_light(true);
          drive_obstacle_avoidance();
      }else{
        breaking_light(false);
      }
     
      delay(100);
  }

  
  drive_stop();
  //CLEANUP ANYTHING
  clear_anything();

  return 0;
}