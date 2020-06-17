// Einführung Embedded Software - SS2020 - 55646
//ProjektPhase Buggy - Gruppe G
//Marcel Ochsendorf 11.06.2020
//marcelochsendorf.com




//------------ GENERAL INCLUDES -------------- //
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <thread>


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
//---------------- ULTRASONIC DRIVER INCLUDE ----- //
#include "UltrasonicDriver.h"
UltrasonicDriver ultraschall;
//------------ INCLUDES MOTORHAT- ------------ //



//----------------- CONSTANT VARS AND DEFINES ---------- //
#define LED_LEFT 29  //LED LINKS AN PIN WIRING_29
#define LED_RIGHT 28 //LED RECHTS AN PIN WIRING_28

#define GPIO_ULTRASONIC_TRIGGER 0 // GPIO ULTRASCONIC SENSOR TRIGGER PIN
#define GPIO_ULTRASONIC_ECHO 2// GPIO ULTRASCONIC SENSOR ECHO PIN




/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
  std::cout << "Strg-C Programmende" << std::endl;
  exit(signum);
}









int main()
{
  
  //REGISTER SIGNAL HANDLER
  signal(SIGINT, signalHandler);
  //WIRING PI SETUP 
  wiringPiSetup();
  //INIT COMPASS SENSOR
  compass.init(false);
  //INIT THE GYROSCOPE
  gyroscope.init(false);
  //INIT THE LEDS
  leds.init(LED_LEFT, LED_RIGHT, false);
  //INIT MOTOR_DRIVER
  motors.init(false);
  //INIT ULTRASONIC SENSOR
  ultraschall.init(GPIO_ULTRASONIC_ECHO, GPIO_ULTRASONIC_TRIGGER, false);

  std::cout << "---- SETUP COMPLETE ----" << std::endl;


  //------------ TASK_1 1 LEDs --------------------- //
  leds.blink_leds();





  
  //motors.drive_stop();


  return 0;
}



