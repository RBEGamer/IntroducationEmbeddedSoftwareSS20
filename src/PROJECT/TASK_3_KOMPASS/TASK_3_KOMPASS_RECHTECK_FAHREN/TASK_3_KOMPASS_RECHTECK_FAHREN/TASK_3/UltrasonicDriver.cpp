#include "UltrasonicDriver.h"
#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <algorithm> //for std::max




UltrasonicDriver::UltrasonicDriver() {

}

UltrasonicDriver::~UltrasonicDriver(){
     pinMode(trigger_pin,INPUT);
}

int UltrasonicDriver::init(int _echo_pin, int _trigger_pin ,bool _with_wiring_setup) {
    if (_with_wiring_setup) {
        std::cout << "LedDriver::init using wiringpi setup" << std::endl;
        wiringPiSetup();
    }
    trigger_pin = _trigger_pin;
    echo_pin = _echo_pin;
    //ULTRASONIC SENSOR GPIO SETUP
    pinMode(_echo_pin, INPUT);
    pinMode(_trigger_pin, OUTPUT);
    digitalWrite(_trigger_pin, LOW);
}



//RETURNS THE DISTANCE TO A OBJECT IN CM USING THE ULTRASONIC SENSOR
float UltrasonicDriver::measure_distance() {
    //TRIGGER TRIGGER PIN = SEND AN ULTRASONIC SIGNAL
    digitalWrite(trigger_pin, HIGH);
    delay(0.1); //sleep .1ms
    digitalWrite(trigger_pin, LOW);

    while (digitalRead(echo_pin) == LOW);
    //SAVE START TRIGGER TIMER
    auto start = std::chrono::high_resolution_clock::now();
    //WAIT FOR ECHO TO BECOME HIGH
    while (digitalRead(echo_pin) == HIGH);
    //SAVE THE TIME OF THE RECEIEVED ECHO
    auto stop = std::chrono::high_resolution_clock::now();

    //CALUCATE DELTA TIME IN MS
    std::chrono::duration<double> TimeElapsed_s = stop - start;
    double TimeElapsed_s_us = TimeElapsed_s.count() * 1.0;
    std::cout << "delattime : " << TimeElapsed_s_us << std::endl;
    //CALCULCATE DISTANCE
    float distanz = (17015.0 * TimeElapsed_s_us); // 34400 is the speed of sound in cm/s; div by 2 = 17015
    std::cout << "distanz :" << distanz << std::endl;

    return distanz;
}





float UltrasonicDriver::map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}