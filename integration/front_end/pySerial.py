"""
Code for serial communiccation between Python and Arduino 

Authors: Ian Ortega, Nabil Othman, Paige Spencer

Date: 12-9-2025

"""
#import libraries
import serial
import time
import numpy as np
import matplotlib.pyplot as plt


"""

    @brief: establsih a serial connection
    @return serial object

"""

def serial_connect():
    for i in range(25):
        try:
            COM_num = "COM" + str(i)
            ser = serial.Serial(COM_num, 115200, timeout=30, stopbits=1)
            print(f"Connected to serial port on {COM_num}")
            return ser
        except:
            continue
    raise Exception("Could not find Arduino on any COM port")



"""
@brief receive serial data in batches
@param ser serial object
@param NUM_nums the number of numbers
@param BATCH_size the number of batches

@note: these params must agree with the Arduino code that is sending the data

@return an array of data


"""
def serial_receive_batch(ser, NUM_nums, BATCH_SIZE):
    ser.reset_input_buffer()
    NUM_BATCHS = NUM_nums // BATCH_SIZE
    master_arr = []

    ser.read_until(b'G', size=1)
    # ser.flush()


    for i in range(NUM_BATCHS):
        gobyte = ser.read_until(b'g', 1)
        ser.write(b'r')
        bytes = ser.read(4*BATCH_SIZE)
        time.sleep(0.5)
        arr = np.frombuffer(bytes, dtype=np.int16)
        master_arr.extend(arr)
        ser.write(b'r')
    return master_arr


"""
@brief function to process data arrays into one continuous sound array. Data is processed according to how it is split during collection
@param data_1 first data array
@param data_2 second data array

@return final data array


"""

def process_data(data_1,data_2, count):

    data_1 = data_1.astype(np.float64) / 32768.0  
    data_2 = data_2.astype(np.float64) / 32768.0

    L2 = len(data_1)
    split2 = L2 // 2

    #move around data according to how it was collected
    valid_11 = data_1[0:split2]
    valid_12 = data_1[split2:L2]
    valid_21 = data_2[0:split2]
    valid_22 = data_2[split2:L2]

    if(count == 0):
        final_sound = np.concatenate((valid_11, valid_21, valid_12, valid_22)) 
    else:
        final_sound = np.concatenate((valid_11, valid_12, valid_22)) #NOTE: valid_21 is supposed to be second in line, but do to errors in the Arduino buffers that I do not understand, it is a bunch of zeros after iteration 1

    final_sound = final_sound.astype(float)
    
    return final_sound



"""

@brief same as process_data EXCEPT it assumes that data_1 and data_2 are two arrays that should be concatenated without any reording

"""
def process_data_simple(data_1,data_2):

    data_1 = data_1.astype(np.float64) / 32768.0  
    data_2 = data_2.astype(np.float64) / 32768.0

    final_sound = np.concatenate((data_1, data_2))
    final_sound = final_sound.astype(float)
    
    return final_sound



"""

@brief process data function that just normalizes a single data array


"""
def process_data_simpler(data_1):
    
    data_1 = data_1.astype(np.float64) / 32768.0  
    final_sound = data_1.astype(float)
    return final_sound



