// Einführung Embedded Software - SS2020 - 55646
//ProjektPhase Buggy - Gruppe G
//Marcel Ochsendorf 11.06.2020
//marcelochsendorf.com




//------------ GENERAL INCLUDES -------------- //
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <chrono>

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


float current_gyroscope_degree = 0.0f;
auto start = std::chrono::high_resolution_clock::now(); //WANN WURDE DIE FUNKTION  get_gyroscope_degree_step zuletzt aufgerufen ? 

float get_gyroscope_degree_step() {
            

    //BERECHNE DAS DELTA DER ZEIT WIE LANGE ES GEDAUERT HAT BIS DIE FUNKTION AUFGERUFEN WURDE
    //DIES IST NÖTIG DA HIER DER GYRO WERT INTEGRIERT WERDEN MUSS UND DIES GEHT NUR MIT DEM DELTA DER ZEIT
    //BEI DER RECHECK FAHREN AUFGABE (umsetzung mit gyro) WAR DIE DELTAZEIT IMMER FEST 10ms
    //HIER IST ER DYNAMISCH FÜR DIE KONTINULIERLICHE REGELUNG

         std::chrono::duration<double> TimeElapsed_s = std::chrono::high_resolution_clock::now() - start;
         start = std::chrono::high_resolution_clock::now();
         double TimeElapsed_s_us = TimeElapsed_s.count() * 1.0;
         
        GyroscopeDriver::readrawresult gyres = gyroscope.readRaw(); //READ VALUES FROM GYROSCOPE
       // gyroscope.prettyprint(gyres);
        std::cout << TimeElapsed_s_us << std::endl;
       
        //INTEGRATE
        current_gyroscope_degree += gyres.gz * TimeElapsed_s_us;


        return current_gyroscope_degree;

    

  
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








  //------------ TASK_4 3 LINIE FAHREN GYROSKOP GESTÜTZT-------------------- //

  

  const int BASE_MOTOR_SPEED = 100; //BASIS SPEED
  const float CONTROL_ATTACK_MULTIPLIER = 15.0f; //WIE STARK SOLL DIE REGELUNG GREIGEN
  for (;;) {

      float curr_degree = get_gyroscope_degree_step(); //HOLE AKTUELLE AUSRICHTUNG
      std::cout << "GYRO DEGREE " << curr_degree << std::endl;

      
      motors.set_motor_speed(BASE_MOTOR_SPEED - curr_degree * CONTROL_ATTACK_MULTIPLIER, BASE_MOTOR_SPEED + curr_degree * CONTROL_ATTACK_MULTIPLIER);
      delay(10);
  
  }








  
  motors.drive_stop();


  return 0;
}



