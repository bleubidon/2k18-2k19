#!/usr/bin/env python3
'''Detection des centres des palets, calcul de leurs coordonnees polaires, calcul des consignes robot dist et rot.'''

from math import sqrt, acos, atan, degrees, copysign
import numpy as np
import cv2
from imutils.contours import sort_contours
from detection_palets import color_params, min_contour_radius, homography_params, image_name, warped_image_name, robot_width, CM2PX, PX2CM

major_version, minor_version, _ = cv2.__version__.split(".")


def detect_objects(image, color_params, min_contour_radius, homography_params, mode=1, gui=False, logg=False):
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

    def compute_mask(image, hue, shift):
        if hue - shift >= 0 and hue + shift <= 255:
            lower = np.uint8((target_hsv[0] - shift, color_params['lower_hsv_saturation'], color_params['lower_hsv_value']))
            higher = np.uint8((target_hsv[0] + shift, color_params['higher_hsv_saturation'], color_params['higher_hsv_value']))
            # print('lower: {}, higher: {}'.format(lower, higher))
            mask = cv2.inRange(image, lower, higher)
        elif hue - shift < 0:
            lower1 = np.uint8((0, color_params['lower_hsv_saturation'], color_params['lower_hsv_value']))
            higher1 = np.uint8((hue + shift, color_params['higher_hsv_saturation'], color_params['higher_hsv_value']))
            lower2 = np.uint8((255 + hue - shift, color_params['lower_hsv_saturation'], color_params['lower_hsv_value']))
            higher2 = np.uint8((255, color_params['higher_hsv_saturation'], color_params['higher_hsv_value']))
            # print('lower1: {}, higher1: {}'.format(lower1, higher1))
            # print('lower2: {}, higher2: {}'.format(lower2, higher2))
            mask1 = cv2.inRange(image, lower1, higher1)
            mask2 = cv2.inRange(image, lower2, higher2)
            mask = cv2.add(mask1, mask2)
        elif hue + shift > 255:
            lower1 = np.uint8((hue - shift, color_params['lower_hsv_saturation'], color_params['lower_hsv_value']))
            higher1 = np.uint8((255, color_params['higher_hsv_saturation'], color_params['higher_hsv_value']))
            lower2 = np.uint8((0, color_params['lower_hsv_saturation'], color_params['lower_hsv_value']))
            higher2 = np.uint8(((hue + shift) % 255, color_params['higher_hsv_saturation'], color_params['higher_hsv_value']))
            # print('lower1: {}, higher1: {}'.format(lower1, higher1))
            # print('lower2: {}, higher2: {}'.format(lower2, higher2))
            mask1 = cv2.inRange(image, lower1, higher1)
            mask2 = cv2.inRange(image, lower2, higher2)
            mask = cv2.add(mask1, mask2)
        else:
            return 'Invalid shift (must be < 128)'

        return mask

    def get_center_in_camera_reference(contour):
        center_raw = [[]]  # Coordinates relative to cv2 image referential
        M = cv2.moments(contour)
        center_raw[0].append(get_centers(M))
        center_raw = np.array(center_raw, dtype='float32')
        center_warped = cv2.perspectiveTransform(center_raw, homography_params['my_homography'])[0][
            0]  # Center corrected with homography
        center_warped = center_warped.tolist()
        center_warped[0] += homography_params['my_homography_horiz_offset']
        center_camera_reference = (center_warped[0] - image_width / 2, - center_warped[1] + image_height)  # Coordinates relative to camera first sight on the ground
        return center_warped, center_camera_reference[0], center_camera_reference[1]

    def get_dist_and_rot(x, y):
        dist_in_pixels = sqrt(x ** 2 + y ** 2)
        dist_real = dist_in_pixels * PX2CM
        rot = degrees(- copysign(atan(x / ((robot_width * CM2PX) / 2 + y)), x))
        return dist_real, rot

    image_height, image_width, _ = image.shape
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    hsv_image = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)
    centers_all_targets, dists_all_targets, rots_all_targets = [], [], []

    for target in color_params['target_colors']:
        if logg:
            print('***Processing RGB target: {}***'.format(target))

        ## Compute the mask for the current target
        target_hsv = cv2.cvtColor(np.uint8([[target]]), cv2.COLOR_RGB2HSV)[0][0]
        # if logg:
        #     print('target_hsv: {}'.format(target_hsv))
        mask = compute_mask(hsv_image, hue=target_hsv[0], shift=color_params['hue_tolerated_gap'])
        # Smooth mask
        mask = cv2.GaussianBlur(mask, (5, 5), 0)
        mask = cv2.erode(mask, None, iterations=25)
        if gui:
            cv2.imshow('Mask for target: {}'.format(target), mask), cv2.waitKey(0)
        # Show bitwise image
        '''if gui:
            bitwise_mask = cv2.bitwise_and(image, image, mask=mask)
            cv2.imshow('Bitwise mask', bitwise_mask), cv2.waitKey(0)'''

        ## Compute the contours for the current mask
        findContours_param1 = cv2.RETR_EXTERNAL
        findContours_param2 = cv2.CHAIN_APPROX_SIMPLE
        if major_version == '4':
            contours, hierarchy = cv2.findContours(mask, findContours_param1, findContours_param2)
        else:
            _, contours, hierarchy = cv2.findContours(mask, findContours_param1, findContours_param2)
        contours = remove_null_contours(contours)
        contours = remove_tiny_contours(contours)
        if mode == 1:
            if len(contours):
                contours = sort_contours(contours, method="bottom-to-top")[0]  # the contours are now ordered from left to right

        if logg:
            print('Number of contours: {}'.format(len(contours)))
        if gui:
            for _ in range(len(contours)):
                cv2.drawContours(image, contours, _, (0, 255, 0), 3)
                cv2.imshow('Contours: {}'.format(target), image), cv2.waitKey(0)

        ## Compute the centers and (dist, rot) commands for each contour
        centers_of_target, dists_of_target, rots_of_target = [], [], []
        for contour in contours:
            center_warped, x, y = get_center_in_camera_reference(contour)
            dist_real, rot = get_dist_and_rot(x, y)
            if logg:
                print('(x, y) camera_reference: ({}, {})'.format(round(x, 2), round(y, 2)))
                print('dist_real: {}'.format(round(dist_real, 2)))
                print('rot: {}'.format(round(rot, 2)))
                print('\n')
            centers_of_target.append((x, y))
            dists_of_target.append(dist_real)
            rots_of_target.append(rot)
            if gui:
                warped_image = cv2.imread(warped_image_name)
                cv2.circle(warped_image, (round(center_warped[0]), round(center_warped[1])), 7, (0, 255, 0), -1)
                cv2.imshow('Centers: {}'.format(target), warped_image), cv2.waitKey(0)
        if mode == 2:
            minimal_rot_contour_index = [abs(_) for _ in rots_of_target].index(min([abs(_) for _ in rots_of_target]))
            #mettre en première position de contours le contour qui a un rot minimal
            if minimal_rot_contour_index > 0:
                rots_of_target[minimal_rot_contour_index], rots_of_target[0] = rots_of_target[0], rots_of_target[minimal_rot_contour_index]

        centers_all_targets.append(centers_of_target)
        dists_all_targets.append(dists_of_target)
        rots_all_targets.append(rots_of_target)

    return centers_all_targets, dists_all_targets, rots_all_targets


get_homography_matrix = lambda pts_src, pts_dst: cv2.findHomography(pts_src, pts_dst)[0]


def write_warped_image_to_file(image_name, warped_image_name, homography):
    image = cv2.imread(image_name)
    im_out = cv2.warpPerspective(image, homography, (image.shape[1], image.shape[0]))
    cv2.imwrite(warped_image_name, im_out)


if __name__ == "__main__":
    write_warped_image_to_file(image_name, warped_image_name, homography_params['my_homography'])
    image = cv2.imread(image_name)
    centers, dists, rots = detect_objects(image, color_params, min_contour_radius, homography_params, mode=1, gui=False, logg=True)
