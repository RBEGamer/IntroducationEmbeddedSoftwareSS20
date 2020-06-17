#ifndef __MotorDriver__
#define __MotorDriver__
#pragma once



#include <wiringPi.h>
#include "adafruitmotorhat.h"

class MotorDriver
{

#define FORWARD AdafruitDCMotor::kForward
#define BACKWARD AdafruitDCMotor::kBackward
#define RELEASE AdafruitDCMotor::kRelease
#define STOP RELEASE

	//---------- FOR THE BREAKING DISTANCE CALCULATION ---------- //
#define MINIMAL_EMG_BREAKING_DISTANCE 15.0
#define MAX_ADDITIVE_BREAKING_DISTANCE 10.0


	AdafruitMotorHAT hat; //INIT MOTORHAT
	
	int sine_wave_step_counter = 0; //counts from 0 to 360
	//POINTERS TO THE MOTOR
	std::shared_ptr<AdafruitDCMotor> motor_left = nullptr;
	std::shared_ptr<AdafruitDCMotor> motor_right = nullptr;

	float minimal_emergency_breaking_distance = 20.0;
public:
	MotorDriver();
	int init(bool _with_wiring_setup);
	~MotorDriver();


	void drive_sine_wave_step(int _base_speed = 70, int _sine_multiplier = 40, int _delay = 7);
	void set_motor_speed(int _speed_left, int _speed_right);
	void drive_forward(int _speed);
	void drive_backward(int _speed);
	void rotate_left(int _speed);
	void rotate_right(int _speed);
	void drive_stop();
	void release_motors();
};
#endif
