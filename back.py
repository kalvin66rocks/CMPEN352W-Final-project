#!/usr/bin/python
import serial
serialport = serial.Serial("/dev/ttyAMA0", 9600, timeout =0.5)
serialport.write("b")
response = serialport.readlines(None)
print response
