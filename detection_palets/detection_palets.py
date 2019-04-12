#!/usr/bin/env python3
'''Script principal'''

from time import sleep, time
import serial
import os
from threading import Thread
from platform import system
from object_detection import raspberry_action

my_port = '/dev/ttyACM0'
my_baudrate = 9600
expected_request = 'request'


def do_action():
    output = ''
    dists, rots = raspberry_action()
    dists, rots = dists[0], rots[0]  # Consider only first mask because we have only one target colour
    output += str(rots[0]//2) if len(rots) > 0 else ''
    print('Detected centers: {}'.format(output))
    return 'rpi_response ' + output + '\n'


# TODO Get the camera to continuously take picture
''''
image_name = 'image.jpg'
duration = 1000
Thread(target=take_pictures, args=(image_name, duration, 0)).start()
'''

while 1:
    # Attempt to establish serial connection
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
        try:
            if ser.inWaiting():
                received = ser.readline().decode('utf-8')[:-2]
                print('received: "{}"'.format(received))
                # print(repr(received))

                if received == expected_request:
                    response = do_action()
                    print('sending: "{}"\n'.format(response))
                    ser.write(response.encode('utf-8'))

        except (OSError, serial.SerialException):
            ser.close()
            print('Disconnected\n')
            break
