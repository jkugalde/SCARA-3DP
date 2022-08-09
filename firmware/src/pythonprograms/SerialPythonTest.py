# Importing Libraries
import serial
import time
arduino = serial.Serial(port='COM6', baudrate=115200, timeout=.1)

f = open("testfile.gcode", "r")

def write_read(x):

    arduino.write(bytes(x, 'utf-8'))
    time.sleep(1)
    data=arduino.readline()
    while data!="next" :
        data = arduino.readline()
        print(data)
    return data

while True:

    for lain in f:
        print(lain) # printing the value
        write_read(lain)

    f.close()
    break
