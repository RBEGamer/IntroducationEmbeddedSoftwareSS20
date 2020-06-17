#include "LedDriver.h"
#include <wiringPi.h>
#include <iostream>


LedDriver::~LedDriver() {
    pinMode(led_pin_left, INPUT);
    pinMode(led_pin_right, INPUT);
}

LedDriver::LedDriver() {

}

int LedDriver::init(int _led_pin_left, int _led_pin_right, bool _with_wiring_setup){
    if (_with_wiring_setup) {
        std::cout << "LedDriver::init using wiringpi setup" << std::endl;
        wiringPiSetup();
    }

    pinMode(led_pin_left, OUTPUT); //SET LED PIN TO OUTPUT
    pinMode(led_pin_right, OUTPUT); //SET LED PIN TO OUTPUT

    //TODO CHECK VALID PINS
    led_pin_right = _led_pin_right;
    led_pin_left = _led_pin_left;
}

void LedDriver::breaking_light(bool _state) {
    digitalWrite(led_pin_left, _state);
    digitalWrite(led_pin_right, _state);
}


void LedDriver::turn_light(bool _state) {
    digitalWrite(led_pin_left, !_state);
    digitalWrite(led_pin_right, _state);
}

void LedDriver::set_led(bool _left, bool _right) {
    digitalWrite(led_pin_left, _left);  
    digitalWrite(led_pin_right, _right); 
}



void LedDriver::blink_leds() {
    bool led_state = false;
    for (int i = 0; i < 3; i++)
    {
        led_state = !led_state; //INVERT STATE
        digitalWrite(led_pin_left, led_state);   // ON
        digitalWrite(led_pin_right, !led_state); // OFF
        delay(500);
    }
    digitalWrite(led_pin_left, 0);   // OFF
    digitalWrite(led_pin_right, 0); // OFF
}
