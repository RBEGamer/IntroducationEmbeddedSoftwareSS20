
//------------ GENERAL INCLUDES -------------- //
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm> //for std::max
//------------ INCLUDES WIRING PI ------------ //


//---------- DRIVER FOR COMPASS QMC5883L ------//
#include "CompassDriver.h"
CompassDriver compass;
//--------- DRIVER FOR GYROSCOPE MPI6050 ------//
#include "GyroscopeDriver.h"
GyroscopeDriver gyroscope;
// ---------------- DRIVER FOR LEDS ------- //
#include "LedDriver.h"
LedDriver leds;
// ---------------- MOTOR DRIVER INCLUDE ------ //
#include "MotorDriver.h"
MotorDriver motors;


//------------ INCLUDES MOTORHAT- ------------ //

//----------------- CONSTANT VARS AND DEFINES ---------- //
#define LED_LEFT 29  //LED LINKS AN PIN WIRING_29
#define LED_RIGHT 28 //LED RECHTS AN PIN WIRING_28

#define GPIO_ULTRASONIC_TRIGGER 0 // GPIO ULTRASCONIC SENSOR TRIGGER PIN
#define GPIO_ULTRASONIC_ECHO 2// GPIO ULTRASCONIC SENSOR ECHO PIN


//--------------- GLOBALS ------------------ //



//SHUTDOWN MOTORS, FREE ALL STUFF, RELEASE SENSORS ETC
void clear_anything()
{
  //------------- GPIO RELEASE --------------- //
  //note wiring pi has no cleanup -> so change all pins back to input
  
   pinMode(GPIO_ULTRASONIC_TRIGGER,INPUT);


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













//TASK_4 -------- DRIVE SPECIFIC DEGREE -----------------
void rotate_degree( int _speed, float _degree) {
    
    float temp = 0.0;
    _degree = _degree * (M_PI / 180.0)*2*0.8; // 0.8 IST DER ERMITTELTE KORREKTURFAKTOR BASIEREND AUF DEM 10ms DELAY
    while (temp < _degree) {

        GyroscopeDriver::readrawresult gyres = gyroscope.readRaw(); //READ VALUES FROM GYROSCOPE
        gyroscope.prettyprint(gyres);
        std::cout << temp << std::endl;
        motors.set_motor_speed(-_speed, _speed);
        //INTEGRATE
        temp += abs(gyres.gz) / (float)100.0;
        delay(10);

    }

    motors.set_motor_speed(0, 0);
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
  //WIRING PI SETUP 
  wiringPiSetup(); //SETUP WIRING PI
  //INIT COMPASS SENSOR
  compass.init(false);
  //INIT THE GYROSCOPE
  gyroscope.init(false);
  //INIT THE LEDS
  leds.init(LED_LEFT, LED_RIGHT, false);
  //INIT MOTOR_DRIVER
  motors.init(false);
  

  //ULTRASONIC SENSOR GPIO SETUP
  pinMode(GPIO_ULTRASONIC_ECHO, INPUT);
  pinMode(GPIO_ULTRASONIC_TRIGGER, OUTPUT);
  digitalWrite(GPIO_ULTRASONIC_TRIGGER, LOW);

 
 


  
 


  //SOME LED BLINKING AS START
  leds.blink_leds();



  //TASK_4 ------ GYROSCOPE PRINT--------------------
  for (;;) {
      gyroscope.prettyprint(gyroscope.readRaw());
      delay(500);
  }
  

  
  motors.drive_stop();
  //CLEANUP ANYTHING
  clear_anything();

  return 0;
}



