#include "GyroscopeDriver.h"

#include "CompassDriver.h"
#include <wiringPi.h>
#include "wiringPiI2C.h"
#include <iostream>
#include <cmath> //FOR SIN


GyroscopeDriver::GyroscopeDriver() {

}

GyroscopeDriver::~GyroscopeDriver()
{
}

int GyroscopeDriver::init(bool _with_wiring_setup) {
    //WE EXCEPT THAT WIRING PI IS ALREADY SETUP
    if (_with_wiring_setup) {
        std::cout << "GyroscopeDriver::init using wiringpi setup" << std::endl;
        wiringPiSetup();
    }

    //OPEN THE I2C DEVICE WITH THE SENSOR ADRESS, WE GET A HANDLE FOR THE SENSOR
    i2c_file_handle = wiringPiI2CSetup(I2C_ADDR_MPU6050);
    if (i2c_file_handle < 0) {
        std::cout << "init wiringpii2csetup failed" << std::endl;
        return -1;
    }



    write_register(SMPLRT_DIV, 0x07);	/* Write to sample rate register */
    write_register(PWR_MGMT_1, 0x01);	//CONITOUS-MODE
    write_register(CONFIG, 0);		// +-250 DEGREE/SECOND MAX
    write_register(GYRO_CONFIG, 24);	// +-250 DEGREE/SECOND MAX
    write_register(INT_ENABLE, 0x01);	/*Write to interrupt enable register */


}

void GyroscopeDriver::prettyprint(const GyroscopeDriver::readrawresult _t) {
    std::cout << "GYROSCOPE [" << _t.gx << "," << _t.gy << "," << _t.gz << "]"<< " ACCELLEROMETER [" << _t.ax << "," << _t.ay << "," << _t.az << "]" << std::endl;
}

GyroscopeDriver::readrawresult GyroscopeDriver::readRaw() {

    GyroscopeDriver::readrawresult tmp_res;
    
    short ax = read_register_16(ACCEL_XOUT_H);
    short ay = read_register_16(ACCEL_YOUT_H);
    short az = read_register_16(ACCEL_ZOUT_H);

    short gx = read_register_16(GYRO_XOUT_H);
    short gy = read_register_16(GYRO_YOUT_H);
    short gz = read_register_16(GYRO_ZOUT_H);

    /* Divide raw value by sensitivity scale factor */
    tmp_res.ax = ax / 16384.0;
    tmp_res.ay = ay / 16384.0;
    tmp_res.az = az / 16384.0;

    tmp_res.gx = gx / 131.0;
    tmp_res.gy = gy / 131.0;
    tmp_res.gz = gz / 131.0;

   
    return tmp_res;

}

int GyroscopeDriver::write_register(int _reg_addr, int _value) {
    return wiringPiI2CWriteReg8(i2c_file_handle, _reg_addr, _value);
}

int GyroscopeDriver::read_register(int _reg_addr) {
    return wiringPiI2CReadReg8(i2c_file_handle, _reg_addr);
}



short GyroscopeDriver::read_register_16(int _addr) {
    short high_byte, low_byte, value;
    high_byte = wiringPiI2CReadReg8(i2c_file_handle, _addr);
    low_byte = wiringPiI2CReadReg8(i2c_file_handle, _addr + 1);
    value = (high_byte << 8) | low_byte;
    return value;
}