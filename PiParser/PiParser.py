#!/usr/bin/env python3
'''Permet de controler le robot en wifi par l'intermediaire du raspberry
Redirige simplement vers le Serial Parser de l'arduino
'''

import serial
from time import sleep

my_port = '/dev/ttyACM0'
my_baudrate = 9600

while 1:
    connectionSuccess = False
    while not connectionSuccess:
        try:
            ser = serial.Serial(my_port, my_baudrate)
            connectionSuccess = True
        except (OSError, serial.SerialException):
            sleep(.1)
            pass

    print('Connected')
    ser.flushInput()
    while 1:
        response = input('> ')
        response += '\n'
        print('sending: "{}"'.format(response))
        try:
            ser.write(response.encode('utf-8'))

        except (OSError, serial.SerialException):
            ser.close()
            print('Disconnected\n')
            break
		