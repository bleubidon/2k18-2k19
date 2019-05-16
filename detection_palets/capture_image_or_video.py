#! /usr/bin/env python3
from picamera import PiCamera
from time import sleep, time
import os; os.chdir('/home/pi/2k19/detection_palets')

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
        # camera.sensor_mode = 4
        camera.resolution = (820, 616) #64x64 est la resolution minimale autorisee pour la picam; max: 3280, 2464; 820, 616
        camera.start_preview()
        sleep(.2) #La camera se reveille
        camera.capture(image_name)
        if logg:
            print('Captured image from picamera')


if __name__ == "__main__":
    take_picture('image.jpg')
