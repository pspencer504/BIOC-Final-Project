
import numpy as np
import matplotlib.pyplot as plt
from playsound import playsound
import time
import os

from pySerial import serial_connect, serial_receive_batch, process_data
from wav_file import data_to_wav_soundfile
from azure_m2 import recognize_from_microphone

raw = np.loadtxt(r"C:\Users\nubnu\AppData\Local\teraterm5\teraterm.log")

L = len(raw)

data1 = raw[0:L//2]
data2 = raw[L//2:L]


sound = process_data(data1, data2)
data_to_wav_soundfile(sound, "pipelineTest.wav", 16125)

# time.sleep(5) 

# # playsound(os.path.abspath('omgIfThisWorks.wav'))
# recognize_from_microphone("omgIfThisWorks.wav")