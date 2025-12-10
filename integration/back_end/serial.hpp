/*
Code for serial communiccation between Python and Arduino 

Authors: Ian Ortega, Nabil Othman, Paige Spencer

Date: 12-9-2025

*/

#pragma once
#include "USBSerial.h"
#include "LEDS.hpp"

USBSerial serial;


//FUNCTION DECLARATIONS

/**
 * @brief function to send an array to python in batches
 * @param NUM_nums the number of numbers that you are sending
 * @param BATCH_SIZE the size of each batch
 * @param signal pointer to the data that you want to send
 */
void send_to_python(uint32_t NUM_nums, uint32_t BATCH_SIZE, uint32_t* signal);


/**
 * @brief function to receive command from python
 * @return the the command
 */
uint8_t receive_command(void);

/**
 * @brief function to send a number to python. Usually this number is the output of a command. 
 * @param num the number you want to send
 */
void send_num(int32_t num);




//FUNCTION DEFINITIONS

void send_to_python(uint32_t NUM_nums, uint32_t BATCH_SIZE, uint32_t* signal)
{

    serial.sync();
    //calculate the number of batches needed
    uint16_t NUM_BATCHES = NUM_nums / BATCH_SIZE;

    //declare variables and wait to ensure syncrhony
    thread_sleep_for(2000);
    uint32_t* signal_copy = signal;
    char go = 'g';
    char GO = 'G';
    char receive;


    //send data
    int i = 0;
    serial.write(&GO, 1);
    for(i = 0;i < NUM_BATCHES; i++)
    {
        //move pointer along
        signal_copy = signal + (i*BATCH_SIZE);

        //handshake with python
        serial.write(&go, 1);
        serial.read(&receive, 1);
        if(receive != 'r')
            break;

        //write data
        serial.write((uint8_t* ) signal_copy, 4*BATCH_SIZE);

        //finish handshake
        serial.read(&receive, 1);
        if(receive != 'r')
            break;
    }

    //flush buffer
    serial.sync();
    thread_sleep_for(1000);
}

uint8_t receive_command(void)
{
    uint8_t receive;
    uint8_t action;
    uint8_t on = 0;
    serial.read(&receive, 1);

    while(receive != 'A')
    {
        serial.read(&receive, 1);
    }

    serial.read(&action, 1);
    return action;
}

void send_num(int32_t num)
{
    uint8_t handshake = 'g';

    thread_sleep_for(10);
    serial.write(&handshake, 1);
    serial.read(&handshake, 1);

    while(handshake != 'r')
    {
        serial.read(&handshake, 1);
    }

    int32_t *yup = &num;
    serial.write((uint8_t*)yup, 4);

}