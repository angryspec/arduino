'''
Created on Nov 20, 2016

@author: paul
'''
from time import sleep
import serial
import struct

ser = serial.Serial('COM3',9600,timeout=1) # Establish the connection

menu = {} # Creates a menu list
menu['1'] = "Move Servo To 0" 
menu['2'] = "Move Servo To 45"
menu['3'] = "Move Servo To 90"
menu['4'] = "Move Servo To 135"
menu['5'] = "Move Servo To 180"
menu['9'] = "Exit"

while (1==1):
    options = menu.keys()
    options.sort()
    for entry in options:
        print '{0}:'.format(entry), menu[entry]
    print "-" * 15    
    selection = raw_input("Please Select: ")
    if selection =='1':
        ser.write(struct.pack('>B',0))
        print ser.readline()
    elif selection =='2':
        ser.write(struct.pack('>B',45))
        print ser.readline()
    elif selection =='3':
        ser.write(struct.pack('>B',90))
        print ser.readline()
    elif selection =='4':
        ser.write(struct.pack('>B',135))
        print ser.readline()
    elif selection =='5':
        ser.write(struct.pack('>B',180))
        print ser.readline()
    elif selection == '9':
        break    
    else:
        print ('\nInvalid Entry!\n')

print ('\nExiting...\n')        
