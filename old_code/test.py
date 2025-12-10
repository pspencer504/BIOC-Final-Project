from pySerial import serial_connect, serial_receive_batch
import numpy as np
import matplotlib.pyplot as plt
tmp = 1

ser = serial_connect()

arr1 = serial_receive_batch(ser, 1000, 1000)

plt.plot(arr1)
plt.show()
