#ifndef __CompassDriver__
#define __CompassDriver__
#pragma once


#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#include <queue>
#include <deque>


#define READ_TIMEOUT 10 //TRY TO WAIT 10*100ms to read a value from the sensor


class CompassDriver
{
    int i2c_file_handle; //stores the i2c handle from wiringpi for i2c communication

    //---- DEFAULT REGISTER DEFINITION
#define I2C_ADDR 0x0D // DEFAULT I2C ADDRESS FOR THE SENSOR


#define REGISTER_CONTROL_1 0x09 // Control Register #1.
#define REGISTER_CONTROL_2 0x0A  // Control Register #2.
#define REGISTER_RST_PERIOD 0x0B //SET/RESET MODE
#define REGISTER_CHIPID 0x0D
#define STATUS_REGISTER 0x06 //STATUS REGISTER ->


#define STAT_DRDY 0b00000001  //# Data Ready.
#define STAT_OVL 0b00000010   //# Overflow flag.
#define STAT_DOR 0b00000100   //# Data skipped for reading.

//---- REGISTERS FOR X SENSOR VALUE---- //
#define X_LSB_REGISTER 0x00
#define X_MSB_REGISTER 0x01
#define Y_LSB_REGISTER 0x02
#define Y_MSB_REGISTER 0x03
#define Z_LSB_REGISTER 0x04
#define Z_MSB_REGISTER 0x05


#define MODE_CONT 0b00000001
#define MODE_ODR_10HZ 0b00000000
#define ODR_ODR_50HZ 0b00000100
#define RANGE_8G 0b00010000     
#define OVERSAMPLING_512 0b00000000    // Over Sample Rate 512: less noise, more power.
#define INT_ENB 0b00000001 //ENABLE INERRUPT
#define SOFT_RST 0b10000000 //TRIGGER A  Soft Reset.



public:

    typedef struct {
    public:
        short  x;
        short  y;
        short  z;
        bool result_valid;
        bool smoothing_valid;
        float heading;
    }readrawresult;



    CompassDriver();
    ~CompassDriver();
    int init(bool _with_wiring_setup);

    int ready();
    readrawresult readRaw();
    void prettyprint(const readrawresult _t);

    int write_register(int _reg_addr, int _value);
    int read_register(int _reg_addr); //READS 8 BIT FROM A REGISTER
    short read_register_16(int _addr);
    readrawresult get_heading( readrawresult _s);

    readrawresult smooth_value(const readrawresult _s);

private:
    const int SMOOTH_VALUE_QUEUE_MAX_ENTRIES = 20;
    std::queue<readrawresult>  smooth_value_queue;
};


#endif