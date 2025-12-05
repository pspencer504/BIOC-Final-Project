

#import libraries
import serial
import numpy as np
import time
import matplotlib.pyplot as plt



# Open the serial port (Adjust COM port and baud rate)
for i in range(25):
    try:
        if i == 3:
            continue
        COM_num = "COM" + str(i)
        ser = serial.Serial(COM_num, 115200, timeout = 10, stopbits=1)
        break
    except:
        dum=0
        # print(f"No serial port on {COM_num}")

print(f"Connected to serial port on {COM_num}")
# configByte = ser.read(1)

ser.reset_input_buffer()
start_time = time.time()

j = 0



NUM_nums = 40000
BATCH_SIZE = 20000
NUM_BATCHS = NUM_nums // BATCH_SIZE

master_arr = []

ser.read_until(b'G', size=1)
# ser.flush()

print("Loop started...")

start_time = time.time()
for i in range(NUM_BATCHS):
    ser.read_until(b'g')
    ser.write(b'r')
    # print(f"go message {i + 1} received")
    bytes = ser.read(2*BATCH_SIZE)
    arr = np.frombuffer(bytes, dtype='<i2')
    master_arr.extend(arr)
    ser.write(b'r')
end_time = time.time()
elapsed_time = end_time - start_time
print(f"Execution time: {elapsed_time:.4f} seconds")

data = np.array(master_arr, dtype=np.int16)
print(data)

plt.plot(data)
plt.show()

print("All data received!")
