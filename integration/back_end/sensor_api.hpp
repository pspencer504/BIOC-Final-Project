#ifndef SENSOR_API_HPP
#define SENSOR_API_HPP

#include "mbed.h"

//sensor addresses
#define HS3_ADDR 0x44 << 1
#define LPS22HB_ADDR (0x5C << 1)

//registers in temp sensor
#define LPS22HB_WHO_AM_I 0x0F
#define LPS22HB_CTRL_REG1 0x10
#define LPS22HB_RES_CONF 0x1A
#define LPS22HB_PRESS_OUT_XL 0x28
#define LPS22HB_TEMP_OUT_L 0x2B

// Other macros for GPIO 
#define GPIOBASE 0x50000000
#define OUTSET   0x508
#define OUTCLR   0x50C
#define DIR      0x514
#define gpio_set ((uint32_t*) (GPIOBASE + OUTSET)) 
#define gpio_clr ((uint32_t*) (GPIOBASE + OUTCLR))
#define gpio_dir ((uint32_t*) (GPIOBASE + DIR))

//enums for configuring filters and odr
typedef enum {
    ODR_1HZ = 0x10,
    ODR_10HZ = 0x20,
    ODR_25HZ = 0x30,
    ODR_50HZ = 0x40,
    ODR_75HZ = 0x50
} OutputDataRate;

typedef enum {
    LPF_DISABLED = 0x00,
    LPF_ODR_DIV_9 = 0x08,
    LPF_ODR_DIV_20 = 0x0C
} LowPassFilter;

//APIS


/**
 * @brief Initialize both sensors
 * @param i2c Reference to I2C object
 * @return true if successful, false otherwise
 */
bool initialize_sensors(I2C &i2c);

/**
 * Read temperature in Fahrenheit
 * @param i2c Reference to I2C object
 * @return Temperature in Fahrenheit * 100; fixed point math
 */
int32_t read_temperature_f(I2C &i2c);

/**
 * Read temperature in Celsius
 * @param i2c Reference to I2C object
 * @return Temperature in Celsius * 100; fixed point math
 */
int32_t read_temperature_c(I2C &i2c);

/**
 * Read humidity percentage
 * @param i2c Reference to I2C object
 * @return Humidity in percent * 100; fixed point math
 */
int32_t read_humidity(I2C &i2c);

/**
 * Read atmospheric pressure
 * @param i2c Reference to I2C object
 * @return Pressure in hPa * 100; fixed point math
 */
int32_t read_pressure(I2C &i2c);

/**
 * Set output data rate for pressure sensor
 * @param i2c Reference to I2C object
 * @param odr odr enum setting
 * @return true if successful, false otherwise
 */
bool set_output_data_rate(I2C &i2c, OutputDataRate odr);

/**
 * Configure low-pass filter for pressure sensor
 * @param i2c Reference to I2C object
 * @param lpf Low-pass filter enum setting
 * @return true if successful, false otherwise
 */
bool set_low_pass_filter(I2C &i2c, LowPassFilter lpf);

/**
 * Enable or disable low-power mode for pressure sensor
 * @param i2c Reference to I2C object
 * @param enable true for low-power mode, false for normal mode
 * @return true if successful, false otherwise
 */
bool set_low_power_mode(I2C &i2c, bool enable);


I2C init(void);


//FUNCTION DEFINITIONS

void init(I2C &i2c)
{

    setbit(gpio_dir, 22);

    DigitalOut i2cbuspull(P1_0);
    i2cbuspull.write(1);
    thread_sleep_for(250);

    DigitalOut humidtemp_sense(P0_22);
    humidtemp_sense.write(1);
    thread_sleep_for(100);

    initialize_sensors(i2c);
    
}

bool initialize_sensors(I2C &i2c) {
    char cmd[2];
    
    // Initialize pressure sensor
    thread_sleep_for(10);
    
    // Check WHO_AM_I
    cmd[0] = LPS22HB_WHO_AM_I;
    if (i2c.write(LPS22HB_ADDR, cmd, 1) != 0) {
        return false;
    }
    
    char who_am_i;
    if (i2c.read(LPS22HB_ADDR, &who_am_i, 1) != 0) {
        return false;
    }
    
    if (who_am_i != 0xB1) {
        return false;
    }
    
    // Configure CTRL_REG1: ODR=10Hz, BDU=1
    cmd[0] = LPS22HB_CTRL_REG1;
    cmd[1] = 0x24;
    if (i2c.write(LPS22HB_ADDR, cmd, 2) != 0) {
        return false;
    }
    
    thread_sleep_for(10);
    return true;
}

bool read_who_am_i(I2C &i2c){
    
    char cmd[2];
    
    // Initialize pressure sensor
    thread_sleep_for(10);
    
    // Check WHO_AM_I
    cmd[0] = LPS22HB_WHO_AM_I;
    if (i2c.write(LPS22HB_ADDR, cmd, 1) != 0) {
        return false;
    }
    
    char who_am_i;
    if (i2c.read(LPS22HB_ADDR, &who_am_i, 1) != 0) {
        return false;
    }
    
    if (who_am_i != 0xB1) {
        return false;
    }

    return true;
}

int32_t read_temperature_f(I2C &i2c) {
    char buff[4] = {0};
    
    if (i2c.write(HS3_ADDR, 0, 0) != 0) {
        return -99900;
    }
    
    thread_sleep_for(50);
    
    if (i2c.read(HS3_ADDR, buff, 4) != 0) {
        return -99900;
    }
    
    // Extract temperature from bytes 2 and 3
    uint16_t temp_byte = buff[2] << 8;
    uint16_t raw_temp = ((temp_byte) | (buff[3] & 0xFC)) >> 2;
    
    // Convert to Celsius * 100
    int32_t temp_c = ((int32_t)raw_temp * 16500 / 16383) - 4000;
    
    // Convert to Fahrenheit * 100
    int32_t temp_f = (temp_c * 9 / 5) + 3200;
    
    return temp_f;
}

int32_t read_temperature_c(I2C &i2c) {
    char buff[4] = {0};
    
    if (i2c.write(HS3_ADDR, 0, 0) != 0) {
        return -99900;
    }
    
    thread_sleep_for(50);
    
    if (i2c.read(HS3_ADDR, buff, 4) != 0) {
        return -99900;
    }
    
    // Extract temperature from bytes 2 and 3
    uint16_t temp_byte = buff[2] << 8;
    uint16_t raw_temp = ((temp_byte) | (buff[3] & 0xFC)) >> 2;
    
    // Convert to Celsius * 100
    int32_t temp_c = ((int32_t)raw_temp * 16500 / 16383) - 4000;
    
    return temp_c;
}

int32_t read_humidity(I2C &i2c) {
    char buff[4] = {0};
    
    if (i2c.write(HS3_ADDR, 0, 0) != 0) {
        return -100;
    }
    
    thread_sleep_for(50);
    
    if (i2c.read(HS3_ADDR, buff, 4) != 0) {
        return -100;
    }
    
    // Extract humidity from bytes 0 and 1
    uint16_t humid_byte = buff[0] & 0x3F;
    uint16_t raw_humidity = (humid_byte << 8) | buff[1];
    
    // Convert to percentage * 100
    int32_t humidity = (int32_t)raw_humidity * 10000 / 16383;
    
    return humidity;
}

int32_t read_pressure(I2C &i2c) {
    char cmd = LPS22HB_PRESS_OUT_XL;
    char data[3] = {0};
    
    if (i2c.write(LPS22HB_ADDR, &cmd, 1) != 0) {
        return -99900;
    }
    
    if (i2c.read(LPS22HB_ADDR, data, 3) != 0) {
        return -99900;
    }
    
    // Combine 24-bit value
    int32_t raw_pressure = (data[2] << 16) | (data[1] << 8) | data[0];
    
    // Sign extend if negative
    if (raw_pressure & 0x800000) {
        raw_pressure |= 0xFF000000;
    }
    
    // Convert to hPa * 100
    int32_t pressure = (raw_pressure * 25) / 1024;
    
    return pressure;
}

bool set_output_data_rate(I2C &i2c, OutputDataRate odr) {
    char cmd[2];
    
    // Read current CTRL_REG1
    cmd[0] = LPS22HB_CTRL_REG1;
    if (i2c.write(LPS22HB_ADDR, cmd, 1) != 0) {
        return false;
    }
    
    char ctrl_reg1;
    if (i2c.read(LPS22HB_ADDR, &ctrl_reg1, 1) != 0) {
        return false;
    }
    
    // Clear ODR bits and set new value
    ctrl_reg1 = (ctrl_reg1 & 0x8F) | odr;
    
    // Write back
    cmd[0] = LPS22HB_CTRL_REG1;
    cmd[1] = ctrl_reg1;
    if (i2c.write(LPS22HB_ADDR, cmd, 2) != 0) {
        return false;
    }
    
    return true;
}

bool set_low_pass_filter(I2C &i2c, LowPassFilter lpf) {
    char cmd[2];
    
    // Read current CTRL_REG1
    cmd[0] = LPS22HB_CTRL_REG1;
    if (i2c.write(LPS22HB_ADDR, cmd, 1) != 0) {
        return false;
    }
    
    char ctrl_reg1;
    if (i2c.read(LPS22HB_ADDR, &ctrl_reg1, 1) != 0) {
        return false;
    }
    
    // Clear filter bits and set new value
    ctrl_reg1 = (ctrl_reg1 & 0xF3) | lpf;
    
    // Write back
    cmd[0] = LPS22HB_CTRL_REG1;
    cmd[1] = ctrl_reg1;
    if (i2c.write(LPS22HB_ADDR, cmd, 2) != 0) {
        return false;
    }
    
    return true;
}

bool set_low_power_mode(I2C &i2c, bool enable) {
    char cmd[2];
    
    // Read current RES_CONF
    cmd[0] = LPS22HB_RES_CONF;
    if (i2c.write(LPS22HB_ADDR, cmd, 1) != 0) {
        return false;
    }
    
    char res_conf;
    if (i2c.read(LPS22HB_ADDR, &res_conf, 1) != 0) {
        return false;
    }
    
    // Set or clear LC_EN bit
    if (enable) {
        res_conf |= 0x01;
    } else {
        res_conf &= ~0x01;
    }
    
    // Write back
    cmd[0] = LPS22HB_RES_CONF;
    cmd[1] = res_conf;
    if (i2c.write(LPS22HB_ADDR, cmd, 2) != 0) {
        return false;
    }
    
    return true;
}

#endif // SENSOR_API_HPP