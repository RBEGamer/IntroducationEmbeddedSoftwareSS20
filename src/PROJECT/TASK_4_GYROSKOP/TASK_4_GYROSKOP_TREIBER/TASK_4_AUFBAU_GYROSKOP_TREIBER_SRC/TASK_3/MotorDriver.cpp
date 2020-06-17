#include "MotorDriver.h"
#include <cmath> //FOR SIN

int sine_wave_step_counter = 0;
MotorDriver::MotorDriver()
{
}
int MotorDriver::init(bool _with_wiring_setup)
{
    //REQUEST MOTORS FROM THE HEAD
    motor_left = hat.getMotor(1);
    motor_right = hat.getMotor(2);

    //CHECK MOTORS INSTANCE IS OK
    if (!motor_left && !motor_right)
    {
        return -1;
    }

    return 0;
}
MotorDriver::~MotorDriver()
{
}



//DIRIVE ON A SINE WAVE CURVE
void MotorDriver::drive_sine_wave_step(int _base_speed, int _sine_multiplier, int _delay)
{
    //BOTH MOTOR FORWARD
    motor_left->run(FORWARD);
    motor_right->run(FORWARD);
    //GET A POINT ON A SIGNWAVE
    float s = std::sin(sine_wave_step_counter / 100.0) * 0.79; //0.70 IS A CORRECT FACTOR FOR MY BUGGY BUILD
    //SET SPEED  TO THE MOTORS ACCORDING THE POINT ON THE SINEWAVE
    motor_right->setSpeed(_base_speed + _sine_multiplier * s);
    motor_left->setSpeed(_base_speed + (1.0 - _sine_multiplier) * s);

    //INCREASE COUNTER FOR NEXT ITERATION
    sine_wave_step_counter++;
    if (sine_wave_step_counter >= 360) {
        sine_wave_step_counter = 0;
    }


}




void MotorDriver::set_motor_speed(int _speed_left, int _speed_right) {
   
    //SET MOTOR SPEED
    motor_left->setSpeed(_speed_left);
    motor_right->setSpeed(_speed_right);
    //SET MOTOR DIRECTION ACCORDING THE SIGN ON THE SPEED VALUES
    if (_speed_left == 0 && _speed_right == 0) {
        motor_left->run(STOP);
        motor_right->run(STOP);
    }

    if (_speed_left < 0) {
        motor_left->run(BACKWARD);
    }
    else {
        motor_left->run(FORWARD);
    }

    if (_speed_right < 0) {
        motor_right->run(BACKWARD);
    }
    else {
        motor_right->run(FORWARD);
    }

}

//SET MOTOR DIRECT BOTH TO FORWARD WITH A GIVEN SPEED
void MotorDriver::drive_forward(int _speed)
{
    set_motor_speed(_speed, _speed);
}

void MotorDriver::drive_backward(int _speed)
{
    set_motor_speed(-_speed, -_speed);
}

void MotorDriver::rotate_left(int _speed)
{
    set_motor_speed(_speed, -_speed);
}
void MotorDriver::rotate_right(int _speed)
{
    set_motor_speed(-_speed, _speed);
}
//STOPS THE MOTORS BUT DOESNT RELEASE THEM
void MotorDriver::drive_stop()
{
    set_motor_speed(0, 0);
}

void MotorDriver::release_motors()
{
    drive_stop();
    motor_left->run(RELEASE);
    motor_right->run(RELEASE);

}
