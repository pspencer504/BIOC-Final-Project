%author: Nabil Othman
%date: 12-2-2025
%purpose: testing audio of microphone from Arduino Nano 33 BLE 

path = 'C:\Users\nubnu\AppData\Local\teraterm5';
file = 'teraterm.log';

file_path = [path, filesep, file];

raw = readmatrix(file_path);
raw = raw / (2^15);

data_1 = raw(1:length(raw)/2);
data_2 = raw((length(raw)/2) + 1: end);

valid_11 = data_1(1:length(data_1)/2);
valid_12 = data_1((length(data_1)/2) + 1 : end);
valid_21 = data_2(1:length(data_1)/2);
valid_22 = data_2((length(data_1)/2) + 1 : end);

final_sound1 = [valid_11; valid_21];%; valid_12; valid_22];
final_sound2 = [valid_11; valid_21; valid_12; valid_22];

Fs = 16125;
dt = 1/Fs;
L = length(raw)*dt;
t = 0:dt:(L-dt);


% sound(final_sound1, Fs);
sound(final_sound2, Fs);
% sound([valid_12; valid_22], Fs)

figure(1)
subplot(1,3,1)
plot(raw)
xlabel("Time (sec)");
ylabel("Ampltiude")
title("Raw Data")

subplot(1,3,2)
plot(final_sound1)
xlabel("Time (sec)");
ylabel("Ampltiude")
title("Just first two halves")

subplot(1,3,3)
plot(final_sound2)
xlabel("Time (sec)");
ylabel("Ampltiude")
title("Attempt with extra data")
%% 


N = 60000;
t = 2;
Fs = N/t