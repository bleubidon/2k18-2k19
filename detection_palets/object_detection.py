#!/usr/bin/env python3
'''Detection des centres des palets et calcul de leur coordonnees polaires'''

from time import time
debug = False
if debug:
    start_time = time()
from math import sqrt, acos, degrees, copysign
from platform import system
import cv2
if debug:
    print('import cv2: {}'.format(round(time()-start_time, 2)))
import numpy as np


'''def get_first_frame_from_video(video_path):
    video = cv2.VideoCapture(video_path)
    ok, frame = video.read()
    if ok:
        cv2.imwrite(r'frame.jpg', frame)'''


major_version, minor_version, _ = cv2.__version__.split(".")

working_os = system()
if working_os == 'Linux':
    from picamera import PiCamera
    from capture_image_or_video import *
    def raspberry_action():
        image_name = 'image.jpg'
        take_picture(image_name)
        # image = cv2.imread(image_name)
        success = False
        while not success:
            try:
                image = homography(image_name, image_name)
                success = True
            except:
                pass
        centers, dists, rots = detect_objects(image, targets, shift, gui=False, logg=False)
        return dists, rots

else:
    import matplotlib.pyplot as plt


def detect_objects(image, targets, shift, gui=False, logg=False):
    def get_centers(M):
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
        else:
            cX, cY = 0, 0
        return (cX, cY)

    def remove_null_contours(contours):
        topop = []
        for _ in range(len(contours)):
            M = cv2.moments(contours[_])
            if M["m00"] == 0:
                topop.append(_)
        contours = np.delete(contours, topop, 0)
        return contours

    def remove_tiny_contours(contours):
        topop = []
        for _ in range(len(contours)):
            contour = contours[_]
            contour = contour.tolist()
            contour = [_[0] for _ in contour]
            normes_contour = [sqrt(point[0] ** 2 + point[1] ** 2) for point in contour]
            contour_radius = max(normes_contour) - min(normes_contour)
            # if logg:
            #     print('Contour radius: {}'.format(contour_radius))
            if contour_radius < min_contour_radius:
                topop.append(_)
        contours = np.delete(contours, topop, 0)
        return contours

    image_height, image_width, _ = image.shape
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    hsv_image = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)
    centers, dists, rots = [], [], []

    for target in targets:
        if logg:
            print('***Processing target: {}***'.format(target))
        ### Compute the mask for the current target
        target_hsv = cv2.cvtColor(np.uint8([[target]]),cv2.COLOR_RGB2HSV)[0][0]
        # print('target_hsv: {}'.format(target_hsv))
        lower = np.uint8((target_hsv[0] - shift, 190, 100))
        higher = np.uint8((target_hsv[0] + shift, 255, 255))
        # print(lower)
        # print(higher)

        mask = cv2.inRange(hsv_image, lower, higher)
        mask = cv2.GaussianBlur(mask, (5, 5), 0)
        # Smooth mask
        '''
        # mask = cv2.blur(mask, (5, 5))
        # kernel = np.ones((5, 5), np.uint8)
        # opening = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
        # print(type(opening))
        # Gaussian blur ?
        # mask = cv2.erode(mask, None, iterations=2)
        # mask = cv2.dilate(mask, None, iterations=2)
        '''

        if gui:
            plt.title('Mask: {}'.format(target)), plt.imshow(mask), plt.show()

        # Show bitwise image
        '''if gui:
            result = cv2.bitwise_and(image, image, mask=mask)
            plt.title('Bitwise mask')
            plt.imshow(result)
            plt.show()
        '''

        ### Compute the contours for the current mask
        findContours_param1 = cv2.RETR_EXTERNAL
        findContours_param2 = cv2.CHAIN_APPROX_SIMPLE
        if major_version == '4':
            contours, hierarchy = cv2.findContours(mask, findContours_param1, findContours_param2)
        else:
            _, contours, hierarchy = cv2.findContours(mask, findContours_param1, findContours_param2)

        contours = remove_null_contours(contours)
        contours = remove_tiny_contours(contours)

        if logg:
            print('Number of contours: {}'.format(len(contours)))

        if gui:
            for _ in range(len(contours)):
                cv2.drawContours(image, contours, _, (0, 255, 0), 3)
                plt.title('Contours: {}'.format(target)), plt.imshow(image), plt.show()

        ### Compute the centers for the current contours
        centers_of_mask, dists_of_mask, rots_of_mask = [], [], []
        for c in contours:
            M = cv2.moments(c)
            raw_center = get_centers(M)  # Coordinates relative to cv2 image referential
            center_real = (raw_center[0] - image_width // 2, - raw_center[1] + image_height)  # Coordinates relative to camera orthogonal projection to the ground
            x, y = center_real[0], center_real[1]
            dist = round(sqrt(x ** 2 + y ** 2))
            rot = round(degrees(- copysign(acos(y / dist), x))) + angle_offset
            dist_real = round(dist * (43/500))

            if logg:
                print('raw_center: {}'.format(raw_center))
                print('center_real: {}'.format(center_real))
                print('dist: {}'.format(round(dist)))
                print('dist real: {}'.format(dist_real))
                print('rot: {}'.format(rot))
                print('\n')

            centers_of_mask.append(center_real)
            dists_of_mask.append(dist_real)
            rots_of_mask.append(rot)

            if gui:
                cv2.circle(image, (center_real[0], center_real[1]), 7, (0, 255, 0), -1)
                plt.title('Centers: {}'.format(target)), plt.imshow(image), plt.show()

        centers.append(centers_of_mask)
        dists.append(dists_of_mask)
        rots.append(rots_of_mask)
        if logg:
            print('\n')

    return centers, dists, rots


def homography(image_name_1, image_name_2, gui=False):
    im_src = cv2.imread(image_name_1)
    # Four corners of the object in source image
    pts_src = np.array([[122, 2], [586, 2], [698, 357], [2, 357]])
    im_dst = cv2.imread(image_name_2)
    # Four corners of the object in destination image.
    pts_dst = np.array([[2, 2], [698, 2], [698, 357], [2, 357]])
    h, status = cv2.findHomography(pts_src, pts_dst)
    # Warp source image to destination based on homography
    im_out = cv2.warpPerspective(im_src, h, (im_dst.shape[1], im_dst.shape[0]))

    if gui:
        cv2.imshow("Source Image", im_src)
        # cv2.imshow("Destination Image", im_dst)
        cv2.imshow("Warped Source Image", im_out)
        cv2.waitKey(0)

        # cv2.imwrite('warped_source.jpg', im_out)

    return im_out


# targets is the list of the RGB colors of the objects to look for
targets = [
    (209, 72, 30),  # Orange palet
    ]
shift = 7  # Ecart tolere a la couleur de target
min_contour_radius = 50  # Rayon minimal pour qu'on contour soit pris en compte
angle_offset = 3  # Compense le fait que la cam n'est pas parfaitement au centre de la longueur de la plaque composants

if __name__ == "__main__":
    if working_os == 'Linux':
        pass

    else:
        # image = cv2.imread('image.jpg')
        image = homography('image.jpg', 'image.jpg', gui=False)  # Corrige la distortion de l'image
        centers, dists, rots = detect_objects(image, targets, shift, gui=True, logg=True)



    print('--> Centers: {}'.format(centers[0]))
