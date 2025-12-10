"""
Integration  code for BIOC final project

Authors: Ian Ortega, Nabil Othman, Paige Spencer

Date: 12-9-2025

"""


import numpy as np
import matplotlib.pyplot as plt
from playsound import playsound

from printing import print_commands, command_result, calculate_command
from pySerial import serial_connect, serial_receive_batch, process_data, process_data_simple
from wav_file import data_to_wav_soundfile
from Azure import recognize_from_microphone

BUFF_LENGTH = 20000

ser = serial_connect()

count = 0

while(True):
    
    #clear serial buffers
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    
    print_commands()
    input("\n\nPress enter and begin talking! ")
    ser.write(b'e')
    
    print("Receiving data...")
    arr1 = serial_receive_batch(ser, BUFF_LENGTH,BUFF_LENGTH)
    ser.flush()
    arr2 = serial_receive_batch(ser, BUFF_LENGTH,BUFF_LENGTH)

    data1 = np.array(arr1, dtype=np.int16)
    data2 = np.array(arr2, dtype=np.int16)
    sound = process_data(data1,data2, count)

    data_to_wav_soundfile(sound, "omgIfThisWorks.wav", 16125)
    text = recognize_from_microphone("omgIfThisWorks.wav")

    command = calculate_command(text)
    
    ser.write(b'A')
    ser.write(bytes([command]))

    byte = ser.read_until(b'g', 1)
    ser.write(b'r')
    val = ser.read(4)
    num_raw = np.frombuffer(val, dtype=np.int32)

    val = float(num_raw[0]/100)
    
    command_result(command, val)
    count = count + 1
