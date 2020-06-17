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

  leds.blink_leds();
  std::cout << "---- SETUP COMPLETE ----" << std::endl;






  bool did_first_time_reading = false;
  float first_time_reading = 0.0;

  const float MAX_DEGREE_OFFSET = 0.5f;
  const int correction_multipliert = 10.0;
  const int MAX_CORRECTION_MULTIPLIER = 20.0;
  const int basespeed = 100;
  int c = 0;


  //------------ TASK_3 2 LINIE FAHREN KOMPASS ÜBERWACHT-------------------- //
  //LIESST DEN COMPASS SENSOR AUS -> BILDET DEN MITTELWERT AUS 50 WERTEN -> RECHNET DAS HEADING AUS
  //DANACH WIRD
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


              //---------------- MOTOR REGELUNG ------------------------ //
              //SAVE THE STARTING DEGREE AFTER 10 READS TO PREVET DISTORTIONS WITH THE JUMPSTARTING MOTORS
              if (!did_first_time_reading && c > 10) {
                  did_first_time_reading = true;
                  first_time_reading = tmp_heading.heading;
                  std::cout << "STORED INITAL DEGREE  " << first_time_reading << std::endl;
              }
              else {
                  c++;
              }


              //THE COTNROL LOOP 1. IF HEADING IS DRIFING OVER threshold (MAX_DEGREE_OFFSET)
              float offset_from_ideal = fabsf(first_time_reading - tmp_heading.heading);
              std::cout << "CORRECTION OFFSET " << offset_from_ideal << std::endl;
              if (did_first_time_reading && offset_from_ideal > MAX_DEGREE_OFFSET) {
                  float tmm = first_time_reading - tmp_heading.heading;
                  std::cout << "tmm OFFSET " << tmm << std::endl;

                  //IF THE OFFSET DEGREE IS POTIVE THE LEFT MOTOR HA TO BE SLOWER
                  if (tmm > 0.0) {
                      std::cout << ">" << std::endl;
                      motors.set_motor_speed(basespeed, basespeed - (int)std::fminf(offset_from_ideal * correction_multipliert, MAX_CORRECTION_MULTIPLIER));
                      leds.turn_light(true);

                      //IF THE OFFSET DEGREE IS NEGATIVE THE LEFT MOTOR HA TO BE FASTER
                  }else if (tmm < 0.0) {
                      leds.turn_light(false);
                      std::cout << "<" << std::endl;
                      motors.set_motor_speed(basespeed, basespeed + (int)std::fminf(offset_from_ideal * correction_multipliert, MAX_CORRECTION_MULTIPLIER));
                  }


              }
              else {
                  motors.set_motor_speed(basespeed, basespeed);
                  leds.turn_light(true);
              }


          }
      }
      delay(10);
  }










  
  motors.drive_stop();


  return 0;
}



