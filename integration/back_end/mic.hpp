    /*
    Code for interacting with Arduino microphone

    Authors: Ian Ortega, Nabil Othman, Paige Spencer

    Date: 12-9-2025

    */

    #pragma once

    #include "nrf_pdm.h"
    #include "mbed.h"
    #include "LEDS.hpp"
    #include "serial.hpp"

    #define START_PDM  nrf_pdm_task_trigger(NRF_PDM_TASK_START)
    #define MIC_CLK 26
    #define MIC_DOUT 25
    #define MIC_VDD 17
    #define BUFF_LENGTH 20000
    #define ERROR_VAL -24176

    //data buffers 
    uint32_t data_buffer[BUFF_LENGTH];
    uint32_t data_buffer2[BUFF_LENGTH];
    uint8_t current_odr = 0;


    //FUNCTION DECLARATIONS


    /**
    * @brief function to configure the pdm module of the chip
    */
    void config_pdm(void);

    /**
    * @brief function to initialize the pdm module of the chip and turn it on
    */
    void init_pdm(void);

    /** 
    * @brief function to aquire microphone data and send it to python via seria
    * @param data_buffer first buffer to store microphone data
    * @param data_buffer2 second buffer to store microphone data
    * @return the command processed by Azure in Python
    */
    uint8_t read_mic(uint32_t* data_buffer, uint32_t* data_buffer2);

    /**
    * @brief perform the command specified by the processed speech in Python 
    * @param action the action index (from python) 
    * @param i2c I2C object
    */
    void perform_action(uint8_t action, I2C &i2c);



    //FUNCTION DEFINITIONS


    void config_pdm(void)
    {
        nrf_pdm_mode_set(NRF_PDM_MODE_MONO, NRF_PDM_EDGE_LEFTRISING);
        nrf_pdm_psel_connect(MIC_CLK, MIC_DOUT);
        nrf_pdm_gain_set(NRF_PDM_GAIN_MAXIMUM, NRF_PDM_GAIN_MAXIMUM);
        nrf_pdm_buffer_set(data_buffer, BUFF_LENGTH);
        nrf_pdm_enable();
    }

    void init_pdm(void)
    {
        //turn power on
        setbit(PORT0_DIR, MIC_VDD);
        setbit(PORT0_OUT, MIC_VDD);

        setbit(PORT0_DIR, MIC_CLK);
        clearbit(PORT0_DIR, MIC_DOUT);

        config_pdm();
    }

    uint8_t read_mic(uint32_t* data_buffer, uint32_t* data_buffer2)
    {

        //flush serial buffer before enabling PDM
        serial.sync();

        //reset memory
        memset(data_buffer, 0, BUFF_LENGTH * sizeof(uint32_t));
        memset(data_buffer2, 0, BUFF_LENGTH * sizeof(uint32_t));

        //reset PDM
        nrf_pdm_disable();
        thread_sleep_for(10);
        nrf_pdm_enable();
        
        // Clear all buffers
        memset(data_buffer, 0, BUFF_LENGTH * sizeof(uint32_t));
        memset(data_buffer2, 0, BUFF_LENGTH * sizeof(uint32_t));

        BLUE_ON();

        //set initial buffer
        nrf_pdm_buffer_set(data_buffer, BUFF_LENGTH);

        //start recording
        nrf_pdm_task_trigger(NRF_PDM_TASK_START);

        //change out buffer
        if(nrf_pdm_event_check(NRF_PDM_EVENT_STARTED))
        {
            nrf_pdm_buffer_set(data_buffer2, BUFF_LENGTH);
        }
        //wait til done
        while (!nrf_pdm_event_check(NRF_PDM_EVENT_END))
        {
        }

        //reset end flag
        nrf_pdm_event_clear(NRF_PDM_EVENT_END);

        //change out buffer
        if(nrf_pdm_event_check(NRF_PDM_EVENT_STARTED))
        {
            nrf_pdm_buffer_set(data_buffer + (BUFF_LENGTH/2), BUFF_LENGTH);
        }

        //wait til done
        while (!nrf_pdm_event_check(NRF_PDM_EVENT_END))
        {
        }

        //reset end flag
        nrf_pdm_event_clear(NRF_PDM_EVENT_END);
        // //change out buffer
        if(nrf_pdm_event_check(NRF_PDM_EVENT_STARTED))
        {
            nrf_pdm_buffer_set(data_buffer2 + (BUFF_LENGTH/2), BUFF_LENGTH);
        }

        //wait til done
        while (!nrf_pdm_event_check(NRF_PDM_EVENT_END))
        {
        }
        nrf_pdm_event_clear(NRF_PDM_EVENT_END);

        //wait til done
        while (!nrf_pdm_event_check(NRF_PDM_EVENT_END))
        {
        }
        nrf_pdm_event_clear(NRF_PDM_EVENT_END);


        //stop
        nrf_pdm_task_trigger(NRF_PDM_TASK_STOP);
        BLUE_OFF();

        //send data serially
        RED_ON();
        send_to_python(BUFF_LENGTH, BUFF_LENGTH, data_buffer);
        send_to_python(BUFF_LENGTH, BUFF_LENGTH, data_buffer2);
        RED_OFF();

        return receive_command();

    }

    void perform_action(uint8_t action, I2C &i2c)
    {

        //make LED purple to indicate action processing mode
        RED_ON();
        BLUE_ON();

        //declare variables used in switch case statement
        int32_t tempF, tempC, humidity, pressure, dummmy_Var;
        bool result;
        OutputDataRate odr = ODR_10HZ;


        //for each action/command, perform the specified task and send the necessary data to python.
        switch(action)
        {
            case 1:
                tempF = read_temperature_f(i2c);
                send_num(tempF);
                break;
            case 2:
                tempC = read_temperature_c(i2c);
                send_num(tempC);
                break;
            case 3:
                humidity = read_humidity(i2c);
                send_num(humidity);        
                break;
            case 4:
                pressure = read_pressure(i2c);
                send_num(pressure);
                break;
            case 5:
                result = set_low_power_mode(i2c, true);
                send_num(result);
                break;
            case 6:
                result = set_low_power_mode(i2c, false);
                send_num(result);
                break;
            case 7:
                result =  set_low_pass_filter(i2c, LPF_ODR_DIV_9);
                send_num(result);
                break;
            case 8:
                result =  set_low_pass_filter(i2c, LPF_DISABLED); 
                send_num(result);
                break;
            case 9:
                result = read_who_am_i(i2c);
                send_num(result);
                break;
            case 10:
                if(current_odr == 0)
                {
                    odr = ODR_10HZ;
                    current_odr = 10;
                }
                else if (current_odr == 10)
                {
                    odr = ODR_75HZ;
                    current_odr = 75;
                }
                else if(current_odr == 75)
                {
                    odr = ODR_10HZ;
                    current_odr = 10;
                }
                else{
                    send_num(ERROR_VAL);
                }
                result = set_output_data_rate(i2c, odr);
                send_num(current_odr);
                break;
            default:
                send_num(ERROR_VAL); 
        }
        RED_OFF();
        BLUE_OFF();


    }