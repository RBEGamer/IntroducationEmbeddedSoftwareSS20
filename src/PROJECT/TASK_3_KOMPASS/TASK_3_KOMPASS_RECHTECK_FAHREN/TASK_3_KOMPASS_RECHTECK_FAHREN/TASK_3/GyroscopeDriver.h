#ifndef __GyroscopeDriver__
#define __GyroscopeDriver__
#pragma once
class GyroscopeDriver
{

#define I2C_ADDR_MPU6050 0x68 // I2C ADDRESS OF THE MPU6050


//------ THE REGISTERS OF THE MPU6050 --------- //
#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47



    int i2c_file_handle; //stores the i2c handle from wiringpi for i2c communication

public:
    typedef struct {
    public:
        float gx;
        float gy;
        float gz;
        float ax;
        float ay;
        float az;
       }readrawresult;


    GyroscopeDriver();
    ~GyroscopeDriver();
    int init(bool _with_wiring_setup);

    int ready();
    readrawresult readRaw();
    void prettyprint(const readrawresult _t);




    int write_register(int _reg_addr, int _value);
    int read_register(int _reg_addr);

    short read_register_16(int _addr);

};

#endif