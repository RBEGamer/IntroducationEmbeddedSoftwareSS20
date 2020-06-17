#ifndef __UltrasonicDriver__
#define __UltrasonicDriver__
#pragma once



class UltrasonicDriver
{
	int trigger_pin;
	int echo_pin;

public:
	UltrasonicDriver();
	~UltrasonicDriver();
	float measure_distance();
	int init(int _echo_pin, int _trigger_pin, bool _with_wiring_setup);
	float map(float x, float in_min, float in_max, float out_min, float out_max);
};

#endif