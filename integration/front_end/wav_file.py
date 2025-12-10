import soundfile as sf
import numpy as np
import matplotlib.pyplot as plt

def data_to_wav_soundfile(input_data, output_wav_file, sample_rate):
    """
    Writes audio data (e.g., NumPy array) to a WAV file using soundfile.

    Args:
        input_data (np.ndarray): NumPy array containing audio samples.
        output_wav_file (str): Path to the output WAV file.
        sample_rate (int): Sampling rate of the audio.
    """
    try:
        sf.write(output_wav_file, input_data, sample_rate)
        # print(f"Successfully wrote audio data to '{output_wav_file}'")
    except Exception as e:
        print(f"Error during conversion to wav file: {e}")



# file_path = r'C:\Users\nubnu\AppData\Local\teraterm5\teraterm.log'

# import numpy as np

# # Example log file content:
# # 1.0 2.5 3.1
# # 4.2 5.8 6.0
# # 7.1 8.9 9.3

# try:
#     raw = np.loadtxt(file_path)
#     # print(raw)
# except FileNotFoundError:
#     print("Error: 'mylog.log' not found.")
# except Exception as e:
#     print(f"An error occurred: {e}")

# # print(len(raw))

# L = len(raw)
# split = L // 2

# # print(L)
# # print(split)


# data_1 = raw[0:split]
# data_2 = raw[split:L]

# # print(f"{len(data_1)}, {len(data_2)}")

# L2 = len(data_1)
# split2 = L2 // 2

# # print(f"{L2} {split2}")

# valid_11 = data_1[0:split2]
# valid_12 = data_1[split2:L2]
# valid_21 = data_2[0:split2]
# valid_22 = data_2[split2:L2]

# # print(valid_11.shape)
# # print(valid_12.shape)
# # print(valid_21.shape)
# # print(valid_22.shape)

# final_sound = np.concatenate((valid_11, valid_21, valid_12, valid_22))

# final_sound /= 2**15

# print(final_sound.shape)


# Fs = 16125

# plt.plot(final_sound)
# plt.show()

# data_to_wav_soundfile(final_sound, "my_sound.wav", Fs)


