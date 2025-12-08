

#import libraries
import serial
import numpy as np
import time
import matplotlib.pyplot as plt

def serial_receive(NUM_nums, BATCH_SIZE):
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
        bytes = ser.read(4*BATCH_SIZE)
        time.sleep(0.5)
        print(f"There are {len(bytes)} bytes")
        arr = np.frombuffer(bytes, dtype=np.uint32)
        master_arr.extend(arr)
        ser.write(b'r')
    return master_arr


master_arr = serial_receive(20000,20000)

data = np.array(master_arr, dtype=np.uint32)

plt.plot(data)
plt.show()

print("All data received!")
