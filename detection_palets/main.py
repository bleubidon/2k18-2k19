#!/usr/bin/env python3
'''Pas encore le script principal'''

if __name__ == "__main__":
    print("PLEASE USE detection_palet.py !!")

    # Connect to arduino via serial
    import serial

    port = '/dev/ttyACM0'
    baudrate = 9600

    ser = wait_until_connected_to_serial(my_port, my_baudrate)
    ser = None
    while ser is None:
        try:
            ser = serial.Serial(my_port, my_baudrate)
        except (OSError, serial.SerialException):
            sleep(.1)
            pass
    print('Connected')
    ser.flushInput()


    # Main loop
    while 1:
        cmd = ser.readline().decode('utf-8')[:-1]

        if cmd[0] in ["request", "request_2"]:
            answer = detect_atoms();
            ser.write((answer + "\n").encode("utf-8"))

        elif cmd[0] == "send":
            from subprocess import call
            args[0] = "../nrf24L01+/nrf24-raspberry/send"
            print(args)
            call(args)

        elif cmd[0] == "ping"
            print("Pinged")
            ser.write("pong\n".encode('utf-8'))

