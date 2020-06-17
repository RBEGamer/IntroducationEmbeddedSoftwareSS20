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







//GIBT DAS AKTUELLE HEADING SMOOTHED AUS
//BLOCK BIS ES EINEN MITTELWERT AUS 50 MESSUNGEN GIBT VIA FUNTION compass.smooth_value();
float get_current_compass_heading() {
    for (;;) {
        CompassDriver::readrawresult  tmp = compass.readRaw(); //READ RAW SENSOR VALUE -> check .valid_result
        if (tmp.result_valid) {
            CompassDriver::readrawresult tmp_smooth = compass.smooth_value(tmp); //SMOOTH THEM ->check .smoothing_valid
            //CHECK IF SENSOR RESULT
            if (tmp_smooth.smoothing_valid) {
                //-------- HERE WE HAVE A GOOD SENSOR REULT WITH  NO CALIBRATION------- //
                CompassDriver::readrawresult tmp_heading = compass.get_heading(tmp_smooth); //CALCULATE THE HEADING WITH NO CALIBRATION
                compass.prettyprint(tmp_heading); //PRINT THE RESULTS
                return tmp_heading.heading;
            }
        }
        delay(10);
    }
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


  //------------ TASK_3 3 RECHTECKFAHRENT-------------------- //
//DREHT SICH IM RECHTECK MIT KOMPASS UNTERSTÜZUNG
  

  //RECHTECK HAT VIER ECKEN
  for (int i = 0; i < 4; i++) {
      motors.drive_forward(100);
      delay(1000);

      //SPEICHERE DEN STARTPUNKT BEVOR DIE DREHUNG ANFÄNGT
      float degree_startpunkt_der_drehung = get_current_compass_heading();
        //BESTIMME DEN ZIELWINKEL MIT OVERVLOW > 360 GRAD
      float destination_degree = degree_startpunkt_der_drehung + 90.0;
      if (destination_degree > 360.0f) {
          destination_degree -= 360.0f;
      }
      if (destination_degree < 0.0f) {
          destination_degree += 360.0;
      }



      motors.rotate_left(75); //STARTE DREHUNG
      for (;;) {
          
          float tmp_heading = get_current_compass_heading();
          float already_roated_degree = std::abs(tmp_heading - destination_degree);
          std::cout << "--- ALREADY ROTATED DEGREE " << already_roated_degree <<  " -> "<< tmp_heading << " ---" <<std::endl;
          //DREHE SOLANGE BIS STAET UND ZIELWINKEL (FAST GLEICH SIND)
          //DIE 10.0 GRAD SIND WEGEN DER GROBEN UNGENAUIGKEIT DES SENSORS EINGEPLANT DA 1 GRAD ODER ÄHNLICH IN MEINEM TESTAUFBAUE NICHT ERRECHT WERDEN KONNTEN
          if (already_roated_degree < 10.0) {
              motors.drive_stop();
              break;
          }
      }
     
      delay(10);
  }











  
  motors.drive_stop();


  return 0;
}



