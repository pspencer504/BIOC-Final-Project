import numpy as np
import matplotlib.pyplot as plt
from playsound import playsound
import time
import os

from pySerial import serial_connect, serial_receive_batch, process_data
from wav_file import data_to_wav_soundfile
from azure_m2 import recognize_from_microphone

print("Say Bismillah")

BUFF_LENGTH = 20000

ser = serial_connect()

arr1, tmp1 = serial_receive_batch(ser, BUFF_LENGTH,BUFF_LENGTH)
ser.flush()
arr2, tmp2 = serial_receive_batch(ser, BUFF_LENGTH,BUFF_LENGTH)

print("WOW our data has been received, about to process")

data1 = np.array(arr1, dtype=np.int16)
data2 = np.array(arr2, dtype=np.int16)

sound = process_data(data1, data2)
data_to_wav_soundfile(sound, "omgIfThisWorks.wav", 16125)

time.sleep(5) 

# playsound(os.path.abspath('omgIfThisWorks.wav'))
recognize_from_microphone("omgIfThisWorks.wav")