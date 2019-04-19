#!/usr/bin/env python3
'''Script principal'''

import numpy as np
if __name__ == "__main__":
    from time import sleep
    import serial
    import io
    from PIL import Image
    import cv2
    from picamera import PiCamera
    from object_detection import detect_objects


# --------------------------------------------------------------------------------------------------------------
# Serial parameters
my_port = '/dev/ttyACM0'
my_baudrate = 9600
expected_requests = ['request', 'request_2']

# Camera parameters
camera_flip = {'vflip': True, 'hflip': True}
camera_mode = 4
resolution = (820, 616)  # 64x64 est la resolution minimale autorisee pour la picam; max: 3280, 2464
framerate = 100
use_video_port = True

# Paramètres relatifs aux palets
color_params = {
    'target_colors': [
        (209, 72, 30),  # Orange palet
        ],
    'hue_tolerated_gap': 7,  # Ecart tolere a la couleur de target
    'lower_hsv_saturation': 130,  # 190 -> 100
    'lower_hsv_value': 100,  # 100 -> 50
    'higher_hsv_saturation': 255,
    'higher_hsv_value': 255,
}

min_contour_radius = 50  # Rayon minimal pour qu'on contour soit pris en compte

homography_params = {
    'my_homography': np.array([[1.16815537e+00, 4.24852493e-01, -1.17816623e+02],
                               [-3.44687599e-02, 1.77790327e+00, -6.61138447e+01],
                               [-3.95294993e-05, 1.10598942e-03, 1.00000000e+00]],
                              dtype='float32'),
    'my_homography_horiz_offset': 43,
}

# Autres
robot_width = 24.8  # in cm
CM2PX = 19.22  # length in cm * CM2PX = length in px
PX2CM = 1 / CM2PX

image_name = 'image.jpg'
warped_image_name = 'warped_image.jpg'
# --------------------------------------------------------------------------------------------------------------


if __name__ == "__main__":
    open_image_linux = lambda stream: cv2.cvtColor(np.array(Image.open(stream)), cv2.COLOR_BGR2RGB)

    def wait_until_connected_to_serial(my_port, my_baudrate):
        # Attempt to establish serial connection
        connection_success = False
        while not connection_success:
            try:
                ser = serial.Serial(my_port, my_baudrate)
                connection_success = True
            except (OSError, serial.SerialException):
                sleep(.1)
                pass
        print('Connected')
        ser.flushInput()
        return ser


    def setup_camera(camera_flip, camera_mode, resolution, framerate):
        camera.vflip = camera_flip['vflip']
        camera.hflip = camera_flip['hflip']
        camera.sensor_mode = camera_mode
        camera.resolution = resolution
        camera.framerate = framerate


    def handle_image_capture(stream, logg=False):
        def build_serial_response(stream, received):
            image = open_image_linux(stream)
            cv2.imwrite(image_name, image)
            if received == 'request':
                mode = 1
            elif received == 'request_2':
                mode = 2
            centers, dists, rots = detect_objects(image, color_params, min_contour_radius, homography_params, mode, gui=False, logg=True)
            dists, rots = dists[0], rots[0]  # Consider only first mask because we have only one target colour for now
            output = ''
            # Consider the contour the most on the left only
            output += str(round(rots[0], 2)) + ' ' if len(rots) > 0 else ''
            output += str(round(dists[0], 2)) if len(dists) > 0 else ''
            if logg:
                print('Detected position: {}'.format(output))
            return 'rpi_response ' + output + '\n'

        if ser.inWaiting():
            received = ser.readline().decode('utf-8')[:-2]
            if logg:
                print('_____________________________')
                print('received: "{}"'.format(received))
                # print(repr(received))

            if received in expected_requests:
                response = build_serial_response(stream, received)
                if logg:
                    print('sending: "{}"\n'.format(response))
                ser.write(response.encode('utf-8'))


    while 1:
        ser = wait_until_connected_to_serial(my_port, my_baudrate)
        while 1:
            try:
                with PiCamera() as camera:
                    setup_camera(camera_flip, camera_mode, resolution, framerate)
                    stream = io.BytesIO()
                    for _ in camera.capture_continuous(stream, 'jpeg', use_video_port=use_video_port):
                        stream.truncate()
                        stream.seek(0)
                        if handle_image_capture(stream, logg=True):
                            break

            except (OSError, serial.SerialException):
                ser.close()
                print('Disconnected\n')
                break
