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


  //------------ TASK_2 1 ABSTANDSMESSUNG LED --------------------- //

  for (;;) {
	  float abstand = ultraschall.measure_distance(); //MESSE DISTANZ
	  std::cout << abstand << "cm" << std::endl; //GEBE ABSTAND AUS

	  if (abstand < 20) {
		  leds.breaking_light(true); //ABSTAND < 20cm = AKTIVIERE BREMSLICHTER (= beide LEDS)
	  }
	  else {
		  leds.breaking_light(false);
	  }
	  delay(100);
  }









  //for (int i = 0; i < 360 * 5; i++) {
//	  motors.drive_sine_wave_step(70, 40); //70 ist die Geschwindikeit und 40 die Amplitude der Sinuswelle
//	  delay(7);
 // }





  
  motors.drive_stop();


  return 0;
}



