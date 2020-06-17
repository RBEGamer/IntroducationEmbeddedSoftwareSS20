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
  motors.drive_stop();
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

  leds.blink_leds();
  std::cout << "---- SETUP COMPLETE ----" << std::endl;


  //------------ TASK_3 1 S KOMPASS_WERTE LESEN-------------------- //

  //LIESST DEN COMPASS SENSOR AUS -> BILDET DEN MITTELWERT AUS 50 WERTEN -> RECHNET DAS HEADING AUS
  for (;;)
  {
      CompassDriver::readrawresult  tmp = compass.readRaw(); //READ RAW SENSOR VALUE -> check .valid_result
      if (tmp.result_valid) {
          CompassDriver::readrawresult tmp_smooth = compass.smooth_value(tmp); //SMOOTH THEM ->check .smoothing_valid
          //CHECK IF SENSOR RESULT
          if (tmp_smooth.smoothing_valid) {
              //-------- HERE WE HAVE A GOOD SENSOR REULT WITH  NO CALIBRATION------- //
              CompassDriver::readrawresult tmp_heading = compass.get_heading(tmp_smooth); //CALCULATE THE HEADING WITH NO CALIBRATION
              compass.prettyprint(tmp_heading); //PRINT THE RESULTS


          }
      }
      delay(100);
  }










  
  motors.drive_stop();


  return 0;
}



