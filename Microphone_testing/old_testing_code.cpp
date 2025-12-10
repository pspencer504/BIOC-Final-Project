void mic_testing(void)
{
    //turn power on
    setbit(PORT0_DIR, MIC_VDD);
    setbit(PORT0_OUT, MIC_VDD);

    setbit(PORT0_DIR, MIC_CLK);
    clearbit(PORT0_DIR, MIC_DOUT);

    config_pdm();

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
    //change out buffer
    if(nrf_pdm_event_check(NRF_PDM_EVENT_STARTED))
    {
        nrf_pdm_buffer_set(data_buffer2 + (BUFF_LENGTH/2), BUFF_LENGTH);
    }

    //wait til done
    while (!nrf_pdm_event_check(NRF_PDM_EVENT_END))
    {
    }
    //stop
    nrf_pdm_task_trigger(NRF_PDM_TASK_STOP);


    int i = 0;
    // uint32_t val = -50;
    // for(i = 0; i < BUFF_LENGTH; i++)
    // {
    //     data_buffer[i] = val;
    //     data_buffer2[i] = 10 + (val++);
    // }

    for(i = 0; i < BUFF_LENGTH; i++)
    {
        int16_t data_1 = (int16_t) (data_buffer[i] & 0x0000FFFF);
        int16_t data_2 = (int16_t) ((data_buffer[i] & 0xFFFF0000) >> 16);
        serial.printf("%d\r\n%d\r\n", data_1, data_2);
    }
    for(i = 0; i < BUFF_LENGTH; i++)
    {
        int16_t data_1 = (int16_t) (data_buffer2[i] & 0x0000FFFF);
        int16_t data_2 = (int16_t) ((data_buffer2[i] & 0xFFFF0000) >> 16);
        serial.printf("%d\r\n%d\r\n", data_1, data_2);
    }
}




// USBCDC serial_raw;
// BufferedSerial serial()
void pySerial_testing(void)
{

    #define NUM_nums 40000
    #define BATCH_SIZE 20000 

    uint16_t NUM_BATCHES = NUM_nums / BATCH_SIZE;

    thread_sleep_for(5000);
    static int32_t signal[NUM_nums];
    int32_t* signal_copy = signal;
    char go = 'g';
    char GO = 'G';
    uint8_t header[2] = {0xAA, 0x55};
    char receive;
    // serial.write(header, 2);



    int i = 0;
    int val = -50;

    for(i = 0; i < NUM_nums; i++)
    {
        signal[i] = val;
        val += 1;

    }



    while(1)
    {
        // int i = 0;
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
            // thread_sleep_for(50);
            serial.read(&receive, 1);
            if(receive != 'r')
                break;
        }
        thread_sleep_for(1000);
    }
}

void receive_serial_message(void)
{

    char target_message = 'r';
    char buffer[10];

    while(1)
    {        
        serial.read(buffer, 1);
        if(buffer[0] == target_message)
        {
            clearbit(PORT0_OUT, LED_BLUE);
        }
        else{
            setbit(PORT0_OUT, LED_BLUE);
        }
    }

}