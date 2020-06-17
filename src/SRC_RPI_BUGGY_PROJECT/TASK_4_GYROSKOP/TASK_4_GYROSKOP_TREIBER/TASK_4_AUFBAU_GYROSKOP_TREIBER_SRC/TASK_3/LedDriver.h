#ifndef __LedDriver__
#define __LedDriver__
#pragma once
class LedDriver
{

#define LOW 0
#define HIGH 1

    int led_pin_left;
    int led_pin_right;
public:

    LedDriver();
    int init(int _led_pin_left,int _led_pin_right, bool _with_wiring_setup);
    void breaking_light(bool _state);
    void turn_light(bool _state);
    void blink_leds();
    void set_led(bool _left, bool _right);
    ~LedDriver();

}; 
#endif

