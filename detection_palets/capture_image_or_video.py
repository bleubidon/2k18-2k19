#! /usr/bin/env python3
from picamera import PiCamera
from time import sleep, time
import os; os.chdir('/home/pi/detection_palets')

# Essayer de definir une fonction take_pictures(image_name, duration, delay=.1) qui prendrait des photos de facon repetee et rapide pendant toute la duree du match
# Aucun des essais ci-dessous n'est satisfaisant

'''
# def take_pictures(image_name, duration, delay=.1):
#     with PiCamera() as camera:
#         camera.vflip = True
#         camera.hflip = True
#         camera.resolution = (700, 500)
#         camera.start_preview()
#         camera.start_recording('foo.h264')
#         start_time = time()
#         print('Picamera is now continuously taking pictures')
#         while time() - start_time < duration:
#             camera.capture(image_name)
#             # print('Captured image from picamera')
#             camera.wait_recording(delay)
#         print('Picamera stopped taking pictures')

# def take_pictures(image_name='', duration=60, delay=.1):
#     with PiCamera() as camera:
#         start_time = time()
#         camera.start_preview()
#         try:
#             for _, image_name in enumerate(camera.capture_continuous(image_name)):
#                 sleep(delay)
#                 if time() - start_time >= duration:
#                     break
#         finally:
#             camera.stop_preview()
#             print('Picamera stopped taking pictures')
'''

def take_pictures(image_name, duration, delay=.1):
    start_time = time()
    while time() - start_time < duration:
        take_picture(image_name, logg=True)
        sleep(delay)


def shoot_video(duration=5):
    with PiCamera() as camera:
        camera.vflip = True
        camera.hflip = True
        # camera.resolution = (640, 480)
        # camera.start_recording('video.h264')
        # camera.wait_recording(duration)
        camera.start_preview()
        camera.start_recording('video.h264')
        sleep(duration)
        camera.stop_recording()
        camera.stop_preview()


def take_picture(image_name, logg=False):
    with PiCamera() as camera:
        camera.vflip = True
        camera.hflip = True
        camera.resolution = (700, 500) #64x64 est la resolution minimale autorisee pour la picam
        camera.start_preview()
        # sleep(.2) #La camera se reveille
        camera.capture(image_name)
        if logg:
            print('Captured image from picamera')
