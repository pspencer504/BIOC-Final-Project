#pragma once
#include "USBSerial.h"
#include "LEDS.hpp"

USBSerial serial;



void send_to_python(uint32_t NUM_nums, uint32_t BATCH_SIZE, uint32_t* signal)
{

    uint16_t NUM_BATCHES = NUM_nums / BATCH_SIZE;

    thread_sleep_for(2000);
    uint32_t* signal_copy = signal;
    char go = 'g';
    char GO = 'G';
    char receive;



    int i = 0;
    serial.write(&GO, 1);
    for(i = 0;i < NUM_BATCHES; i++)
    {
        signal_copy = signal + (i*BATCH_SIZE);
        serial.write(&go, 1);
        serial.read(&receive, 1);
        if(receive != 'r')
            break;
        // thread_sleep_for(20);
        serial.write((uint8_t* ) signal_copy, 4*BATCH_SIZE);
        // thread_sleep_for(500);
        serial.read(&receive, 1);
        if(receive != 'r')
            break;
    }
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


    // if(action == 1)
    // {
    //     BLUE_ON();
    // }
    // else if(action == '1')
    // {
    //     GREEN_ON();
    // }
    // else{
    //     RED_ON();
    // }

    return action;
}

void send_message(void)
{
    uint8_t handshake = 'g';
    thread_sleep_for(10);
    handshake = 'g';
    serial.write(&handshake, 1);
    serial.read(&handshake, 1);

    while(handshake != 'r')
    {
        serial.read(&handshake, 1);
    }

    int32_t yup_value = 7532;
    int32_t *yup = &yup_value;
    serial.write((uint8_t*)yup, 4);

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