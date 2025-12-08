

#import libraries
import serial
import numpy as np
import time
import matplotlib.pyplot as plt

def serial_connect():
    """Connect to Arduino and return serial object"""
    for i in range(25):
        try:
            if i == 3:
                continue
            COM_num = "COM" + str(i)
            ser = serial.Serial(COM_num, 115200, timeout=30, stopbits=1)
            print(f"Connected to serial port on {COM_num}")
            return ser
        except:
            continue
    raise Exception("Could not find Arduino on any COM port")

def serial_receive_batch(ser, NUM_nums, BATCH_SIZE):
    """Receive one batch of data from already-connected serial"""
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
        arr = np.frombuffer(bytes, dtype=np.int16)
        master_arr.extend(arr)
        ser.write(b'r')
    return master_arr, arr


def process_data(data_1,data_2):

    data_1 = data_1.astype(np.float64) / 32768.0  
    data_2 = data_2.astype(np.float64) / 32768.0

    L2 = len(data_1)
    split2 = L2 // 2

    # print(f"{L2} {split2}")

    valid_11 = data_1[0:split2]
    valid_12 = data_1[split2:L2]
    valid_21 = data_2[0:split2]
    valid_22 = data_2[split2:L2]

    # print(valid_11.shape)
    # print(valid_12.shape)
    # print(valid_21.shape)
    # print(valid_22.shape)

    final_sound = np.concatenate((valid_11, valid_21, valid_12, valid_22))

    final_sound = final_sound.astype(float)
    # final_sound /= 2**15
    
    plt.plot(final_sound)
    plt.show()

    return final_sound






# master_arr = serial_receive(20000,20000)

# data = np.array(master_arr, dtype=np.uint16)
# print(len(data))

# # data /= 2**15

# plt.plot(data)
# plt.show()

# print("All data received!")
