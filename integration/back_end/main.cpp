/*Integration code for Arduino for BioC final project

Authors: Ian Ortega, Nabil Othman, Paige Spencer

Date: 12-9-2025

*/


#include "mbed.h"
#include "NOthman_binaryutils.hpp"
#include "nrf_pdm.h"
#include "sensor_api.hpp"
#include "mic.hpp"



/*
LED COLORING

LED is Green when Arduino is waiting for a command from Python
LED is Blue when the mic is reading data
LED is Red when the Arduino is sending data serially to Python


*/


/**
 * @brief: main function that runs forever and executes all code
 */
int main()
{

    uint8_t command;
    uint8_t action;


    //initialize i2c, LEDs, and PDM module
    I2C i2c(I2C_SDA1, I2C_SCL1);
    init(i2c);
    init_LEDs();
    init_pdm();


    //white loop to run forever
    while(true)
    {    

        //read command from python. 'e' signals the arduino to begin reading mic data
        command = 'n';
        GREEN_ON();
        serial.read(&command, 1);
        while(command != 'e')
        {
            serial.read(&command, 1);
        }
        GREEN_OFF();

        //read mic and perform the associated action
        action = read_mic(data_buffer,data_buffer2);
        perform_action(action, i2c);
    }    
}