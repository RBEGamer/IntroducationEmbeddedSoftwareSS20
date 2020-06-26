#include "CompassDriver.h"
#include <wiringPi.h>
#include "wiringPiI2C.h"
#include <iostream>
#include <cmath> //FOR SIN


//#include <unistd.h>
//#include <errno.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <linux/i2c-dev.h>
//#include <sys/ioctl.h>
//#include <fcntl.h>


CompassDriver::CompassDriver(){
}
CompassDriver::~CompassDriver() {
}
int CompassDriver::init(bool _with_wiring_setup)
{
//WE EXCEPT THAT WIRING PI IS ALREADY SETUP
 if(_with_wiring_setup){
     std::cout << "QMC5883L::init using wiringpi setup" << std::endl;
     wiringPiSetup(); 
 }

//OPEN THE I2C DEVICE WITH THE SENSOR ADRESS, WE GET A HANDLE FOR THE SENSOR
i2c_file_handle = wiringPiI2CSetup(I2C_ADDR);
if (i2c_file_handle < 0) {
  std::cout << "init wiringpii2csetup failed" << std::endl;
  return -1;
}

//LETS READ THE CHIP ADRESS BECAUSE THERE IS AN OTHER CHIP CALLED HMC5883L WITH A DIFFERENT ID AND DIFFERENT REGISTER MAPPINGS
 if(read_register(REGISTER_CHIPID) != 0xFF){
     std::cout << "wrong chip id maybe a HMC5883L??" << std::endl;
 }

//NOW CHONFIG THE SENSOR
char chip_config =  MODE_CONT | ODR_ODR_50HZ | RANGE_8G | OVERSAMPLING_512;
std::cout << "chip config : " << (int)chip_config << std::endl;
write_register(REGISTER_CONTROL_2, SOFT_RST);// RESET THE SENSOR
write_register(REGISTER_CONTROL_2, INT_ENB);//DISABLE INTERRIPT (NOT NEEDED
write_register(REGISTER_RST_PERIOD, 0x01);//ENABLE THE SENSOR
write_register(REGISTER_CONTROL_1, chip_config);//CONFIG OUR PARAMETERS TO IT - THE MODE_CONT IS THE IMPORTANT BIT, ALL OTHERS ARE DEFAULT VALUES

  return 0;
}



    int CompassDriver::write_register( int _reg_addr, int _value){
        return wiringPiI2CWriteReg8 ( i2c_file_handle,  _reg_addr,  _value);
    }
    
    int CompassDriver::read_register(int _reg_addr){
        return wiringPiI2CReadReg8 (i2c_file_handle,_reg_addr);
    }

    short CompassDriver::read_register_16(int _addr) {
        short high_byte, low_byte, value;
        high_byte = read_register( _addr);
        low_byte = read_register( _addr + 1);
        value = (high_byte << 8) | low_byte;
        return value;
    }


    int CompassDriver::ready(){
        int status = read_register(STATUS_REGISTER);
         if(!status) {
            // std::cout << "ready got sensor status read failed:"<< status << std::endl;
             return -1;
        }

      //   std::cout << "QMC5883L::ready got sensor status "<<status<<" with tmp "<<tmp<< std::endl;
        if(status & STAT_DRDY){
       //     std::cout << "GET SENSOR STATE DATA_READY_TO_READ "<<status<< std::endl;
            return 0; 
        }

        if(status & STAT_DOR){
            std::cout << "GET SENSOR STATE DATA SKIPPED "<<status<< std::endl;
            return 1; 
        }
       
        if(status & STAT_OVL){
            std::cout << "GET SENSOR STATE DATA_OVERFLOW "<<status<< std::endl;
            return 2; 
        }
    }




    CompassDriver::readrawresult CompassDriver::get_heading(CompassDriver::readrawresult _s){
        float heading=std::atan2(_s.y,_s.x);

        if(heading < 0){
            heading += 2.0*M_PI;
        }else if(heading > 2.0 * M_PI){
            heading -= 2.0 * M_PI;
        }
       _s.heading =  heading * 180.0 / M_PI;
       return _s;
    }



    void CompassDriver::prettyprint(const CompassDriver::readrawresult _t){
        std::cout << "COMPASS_SENSOR_READINGS [" << _t.x << "," << _t.y << "," << _t.z << "] HEADING: "<<_t.heading << " VALID:" << _t.result_valid << " SMOOTHING_VALID:" << _t.smoothing_valid << std::endl;
    }


    CompassDriver::readrawresult CompassDriver::smooth_value(const CompassDriver::readrawresult _s) {
        //CHECK IF QUEUE LIMIT REACHED THEN POP A ENTRY AND PUSH THE NEW VALUE ALWAYS
        if (smooth_value_queue.size() > SMOOTH_VALUE_QUEUE_MAX_ENTRIES) {
            smooth_value_queue.pop();
        }
        smooth_value_queue.push(_s);

        CompassDriver::readrawresult tmp_res;
         //SET THE SMOOTHING FLAG TO TRUE IF ENOUGHT ENTRIES ARE IN THE QUEUE
        tmp_res.smoothing_valid = false;
        if (smooth_value_queue.size() >= SMOOTH_VALUE_QUEUE_MAX_ENTRIES-1) {
            tmp_res.smoothing_valid = true;
        }

        //MAKE A COPY OF THE QUEUE FOR COUNTING
        std::queue<CompassDriver::readrawresult> tmp= smooth_value_queue;
         //ADD ALL VALUES FROM EACH QUEUE ENTRY TOGETHER TO GET A MIDDLE VALUE FOR XYZ
        bool tmp_res_valid = true;
        while (!tmp.empty())
        {
            CompassDriver::readrawresult q_element = tmp.front();
          //  prettyprint(q_element);

            tmp_res.x += q_element.x;
            tmp_res.y += q_element.y;
            tmp_res.z += q_element.z;

            if (!q_element.result_valid) {
                tmp_res_valid = false;
            }
            tmp.pop();
        }

        tmp_res.x /= smooth_value_queue.size();
        tmp_res.y /= smooth_value_queue.size();
        tmp_res.z /= smooth_value_queue.size();

        tmp_res.result_valid = tmp_res_valid;
        //RETURN A SMOOTHED VALUE
        return tmp_res;
    }

    

    CompassDriver::readrawresult CompassDriver::readRaw(){
        CompassDriver::readrawresult res;
       
        res.result_valid = false;
        //WAIT FOR SENSOR TO READ DATA TO REGISTER
        int c = 0;
        while(ready() != 0 && ready() != 1){
            delay(10);
            c++;
            //SKIP AFTER X TRY
            if(c >= READ_TIMEOUT){
                std::cout << "SENSOR READ TIMEOUT" << std::endl;
                res.result_valid = false;
                return res;
            }
     
        }

        //READ THE 6 REGISTERS FOR XYZ
            res.x = read_register_16(X_LSB_REGISTER);
            res.y = read_register_16(Y_LSB_REGISTER);
            res.z = read_register_16(Z_LSB_REGISTER);
    
            res.result_valid = true;
          
        return res;
    }

   