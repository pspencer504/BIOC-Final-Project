#include "mbed.h"
#include "NOthman_binaryutils.hpp"
#include "nrf_pdm.h"
#include "nrf_gpio.h"
#include "nrf_clock.h"
#include "USBSerial.h"
// #include "USBCDC.h"
// #include "BufferedSerial.h"
USBSerial serial;

//macros that are so much more intuitive than those functions

#define START_PDM  nrf_pdm_task_trigger(NRF_PDM_TASK_START)
#define MIC_CLK 26
#define MIC_DOUT 25
#define MIC_VDD 17
#define PORT0_OUT (uint32_t*) 0x50000504
#define PORT0_DIR (uint32_t*) 0x50000514
#define LED_BLUE 6
#define LED_ORANGE 13
#define LED_GREEN 16
#define LED_RED 24

/*
Alrighty I gotta learn to do this it actually really seems quite simple to me. Per the PDM module on the NRF data sheet, I think it's quite straightforward.
I'm usually wrong about these things though. 

Basically. We attach the Din and clk pin to the appropriate spots on microphone.

Mic clk: P0 pin 26
Mic Dout: P0 pin 25
Mic Vdd: P0 pin 17
Mic LR: P0 pin 17

We gotta set up the decimation filter.
 - set a down sampling ratio
 - set GAINL and GAINR registers if appropriate
 
Turn on the clk (set frequency???)
Set mono mode (1 mic) in MODE register OPERATION field
Set EasyDMA destination buffer address pointer
Start continuous transfer with a start task

So SURELY the nrf API would not be that complicated. SURELY.

I'm just going to commentate my thoughts here.

Looks like there are enums for everything. Yay :) We lowkey just gotta find the functions that accept our enums, call ALL of those, and bingo bango?

We'll want to clal this one: __STATIC_INLINE void nrf_pdm_task_trigger(nrf_pdm_task_t pdm_task)

This'll be helpful: __STATIC_INLINE bool nrf_pdm_event_check(nrf_pdm_event_t pdm_event)

We may want this: __STATIC_INLINE void nrf_pdm_int_enable(uint32_t pdm_int_mask) and this: __STATIC_INLINE bool nrf_pdm_int_enable_check(uint32_t pdm_int_mask)

They money ball: __STATIC_INLINE void nrf_pdm_enable(void)
Also important: __STATIC_INLINE void nrf_pdm_disable(void)

always helpful: __STATIC_INLINE bool nrf_pdm_enable_check(void)

I think we will need: __STATIC_INLINE void nrf_pdm_mode_set(nrf_pdm_mode_t pdm_mode, nrf_pdm_edge_t pdm_edge)
also will need: __STATIC_INLINE void nrf_pdm_clock_set(nrf_pdm_freq_t pdm_freq)
very necessary: __STATIC_INLINE void nrf_pdm_psel_connect(uint32_t psel_clk, uint32_t psel_din)

just for kicks: __STATIC_INLINE void nrf_pdm_psel_disconnect()
I doubt we'll need: __STATIC_INLINE void nrf_pdm_gain_set(nrf_pdm_gain_t gain_l, nrf_pdm_gain_t gain_r)

muy importante: __STATIC_INLINE void nrf_pdm_buffer_set(uint32_t * p_buffer, uint32_t num)

Bingo bango that's lowkey it??? This should be a bit slight??? What am I missing???









*/

#define BUFF_LENGTH 31000
uint32_t data_buffer[BUFF_LENGTH];
uint32_t data_buffer2[BUFF_LENGTH];

void init_LEDs(void)
{
    //set direction to output and turn off initially
    setbit(PORT0_DIR, LED_GREEN);
    setbit(PORT0_DIR, LED_BLUE);
    setbit(PORT0_DIR, LED_RED);
    setbit(PORT0_DIR, LED_ORANGE);
    setbit(PORT0_OUT, LED_GREEN);
    setbit(PORT0_OUT, LED_BLUE);
    setbit(PORT0_OUT, LED_RED);
    clearbit(PORT0_OUT, LED_ORANGE);
}


void config_pdm(void)
{

    // This'll be helpful: __STATIC_INLINE bool nrf_pdm_event_check(nrf_pdm_event_t pdm_event)

    // We may want this: __STATIC_INLINE void nrf_pdm_int_enable(uint32_t pdm_int_mask) and this: __STATIC_INLINE bool nrf_pdm_int_enable_check(uint32_t pdm_int_mask)




    nrf_pdm_mode_set(NRF_PDM_MODE_MONO, NRF_PDM_EDGE_LEFTRISING);
    // NRF_PDM->MODE = 0x3;//((pdm_mode << PDM_MODE_OPERATION_Pos) & PDM_MODE_OPERATION_Msk) | ((pdm_edge << PDM_MODE_EDGE_Pos) & PDM_MODE_EDGE_Msk);
    // nrf_pdm_clock_set(NRF_PDM_FREQ_1067K);


    nrf_pdm_psel_connect(MIC_CLK, MIC_DOUT);

    // just for kicks: __STATIC_INLINE void nrf_pdm_psel_disconnect()
    // I doubt we'll need: __STATIC_INLINE void nrf_pdm_gain_set(nrf_pdm_gain_t gain_l, nrf_pdm_gain_t gain_r)



    //default downsampling ratio is 64
    /*
    f_clk = 1,032,000
    f_sample = f_clk / 64 = 16125.0 Hz

    for 1 sec of data, we need 16,125 buffers


    */

    nrf_pdm_gain_set(NRF_PDM_GAIN_MAXIMUM, NRF_PDM_GAIN_MAXIMUM);

    nrf_pdm_buffer_set(data_buffer, BUFF_LENGTH);
    nrf_pdm_enable();
    // Also important: __STATIC_INLINE void nrf_pdm_disable(void)

    bool isEnabled= nrf_pdm_enable_check();
    // serial.printf("\tPDM is enabled? %d\r\n", isEnabled);

}

void mic_testing(void)
{
      //TURN POWER ON YOUR MORON    
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

    // serial.printf("Data collection done!\r\n");

    int i = 0;
    for(i = 0; i < BUFF_LENGTH; i++)
    {
        int16_t data_1 = (int16_t) (data_buffer[i] & 0x0000FFFF);
        int16_t data_2 = (int16_t) ((data_buffer[i] & 0xFFFF0000) >> 16);
        // serial.printf("%d\r\n%d\r\n", data_1, data_2);
    }
    for(i = 0; i < BUFF_LENGTH; i++)
    {
        int16_t data_1 = (int16_t) (data_buffer2[i] & 0x0000FFFF);
        int16_t data_2 = (int16_t) ((data_buffer2[i] & 0xFFFF0000) >> 16);
        // serial.printf("%d\r\n%d\r\n", data_1, data_2);
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
    static int16_t signal[NUM_nums];
    int16_t* signal_copy = signal;
    char go = 'g';
    char GO = 'G';
    uint8_t header[2] = {0xAA, 0x55};
    char receive;
    // serial.write(header, 2);



    int i = 0;
    int val = -5000;

    for(i = 0; i < NUM_nums; i++)
    {
        signal[i] = val;
        val += 1;

    }



    while(1)
    {
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
            serial.write((uint8_t* ) signal_copy, 2*BATCH_SIZE);
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

// main() runs in its own thread in the OS
int main()
{

    init_LEDs();
    pySerial_testing(); 
}

