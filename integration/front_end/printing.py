"""
Code for large print statements and processing commands

Authors: Ian Ortega, Nabil Othman, Paige Spencer

Date: 12-9-2025

"""

from enum import Enum
import time

ERROR_VAL = -24176




"""

@brief function to print what the commands are to the user

"""
def print_commands():
    print("\n\n\n\nThe voice command options are...")
    print("\t1. Read temperature in Fahrenheit")
    print("\t2. Read temperature in Celsius")
    print("\t3. Read percent humidity")
    print("\t4. Read Pressure")
    print("\t5. Pressure low power mode on")
    print("\t6. Pressure low power mode off")
    print("\t7. Pressure filter on")
    print("\t8. Pressure filter off")
    print("\t9. Pressure who am I")
    print("\t10. Toggle sampling frequency\n\t\t This toggles between 10 Hz and 75 Hz.")


"""
@brief enum used for convenience when coding

"""

class Commands(Enum):
    TEMPF = 1
    TEMPC = 2
    HUMIDITY = 3
    PRESSURE = 4
    LOW_POWER_ON = 5
    LOW_POWER_OFF = 6
    FILTER_ON = 7
    FILTER_OFF = 8
    WHOAMI = 9
    FS = 10
 
 
"""

@brief function to process text into commands
@param text the message received from azure
@return the command index

"""
  
def calculate_command(text):
    text = text.lower()
    if("temperature" in text):
        if("fahrenheit" in text):
            return 1
        elif("celsius" in text):
            return 2
    elif ("humidity" in text):
        return 3
    elif("read pressure" in text):
        return 4
    elif("power mode" in text):
        if("on" in text):
            return 5
        elif("off in text"):
            return 6
    elif("filter" in text):
        if("on" in text):
            return 7
        elif("off" in text):
            return 8
    elif("who" in text):
        return 9
    elif("frequency" in text):
        return 10
    return 99 #ERROR
        
    
    
  
"""

@brief function to print results of whatever command to terminal
@param command the command index
@val the value received from Arduino
@return nothing


"""
def command_result(command, val):
    
    if(val == ERROR_VAL):
        print("ERROR: sensor received invalid command or other issue. Please try again.")
        time.sleep(3)
        return    
    
    match command:
        case 1:
            print(f"Temperature = {val} degrees Fahrenheit")
            time.sleep(3)
            return
        case 2:
            print(f"Temperature = {val} degrees Celsius")
            time.sleep(3)
            return
        case 3:
            print(f"Humidity = {val}%")
            time.sleep(3)
            return
        case 4:
            print(f"Pressure = {val} hPa")
            time.sleep(3)
            return
        case 5:
            if(val):
                print("Low power mode of pressure sensor: ON")
                time.sleep(3)
                return
            else:
                print("ERROR in sensor I2C: Low power mode NOT set")
                time.sleep(3)
                return
        case 6:
            if(val):
                print("Low power mode of pressure sensor: OFF")
                time.sleep(3)
                return
            else:
                print("ERROR in sensor I2C: Low power mode NOT turned off")
                time.sleep(3)
                return
        case 7:
            if(val):
                print("Pressure Low Pass Filter: ON")
                time.sleep(3)
                return
            else:
                print("ERROR in sensor I2C. LPF NOT turned on")
                time.sleep(3)
                return
        case 8:
            if(val):
                print("Pressure Low Pass Filter: OFF")
                time.sleep(3)
                return
            else:
                print("ERROR in sensor I2C. LPF NOT turned off")
                time.sleep(3)
                return
        case 9:
            if(val):
                print(f"Pressure Who Am I register value: 0xB1")
                time.sleep(3)
                return
            else:
                print("ERROR in sensor I2C. Who Am I register not read")
                time.sleep(3)
                return
        case 10: 
            print(f"Pressure Sampling Frequency Changed to {val * 100} Hz")
            time.sleep(3)
            return
        case _:  
            print(f"Unknown command: {command}")
            