/* Copyright (C) 2012-2017 Ultraleap Limited. All rights reserved.
 *
 * Use of this code is subject to the terms of the Ultraleap SDK agreement
 * available at https://central.leapmotion.com/agreements/SdkAgreement unless
 * Ultraleap has signed a separate license agreement with you or your
 * organisation.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "LeapController.h"
#include <cmath>
#include <iostream>

/** Callback for when the connection opens. */

Hand LeapController::leftHand;
Hand LeapController::rightHand;
Angle LeapController::angle;

LEAP_CONNECTION* LeapController::connectionHandle = nullptr;


void LeapController::setup(void) {
    ConnectionCallbacks.on_connection = &(OnConnect);
    ConnectionCallbacks.on_device_found = &(OnDevice);
    ConnectionCallbacks.on_frame = &(OnFrame);
    ConnectionCallbacks.on_image = &(OnImage);
    ConnectionCallbacks.on_point_mapping_change = &(OnPointMappingChange);
    ConnectionCallbacks.on_log_message = &(OnLogMessage);
    ConnectionCallbacks.on_head_pose = &(OnHeadPose);
}

void LeapController::OnConnect(void) {
    printf("Connected.\n");
}

/** Callback for when a device is found. */
void LeapController::OnDevice(const LEAP_DEVICE_INFO* props) {
    printf("Found device %s.\n", props->serial);
}

/** Callback for when a frame of tracking data is available. */
void LeapController::OnFrame(const LEAP_TRACKING_EVENT* frame) {
    bool detectedLeft = false;

    if (frame->info.frame_id % 60 == 0)
        printf("Frame %lli with %i hands.\n", (long long int)frame->info.frame_id, frame->nHands);

    for (uint32_t h = 0; h < frame->nHands; h++) {
        LEAP_HAND* hand = &frame->pHands[h];
        /*
        printf("    Hand id %i is a %s hand with position (%f, %f, %f).\n",
            hand->id,
            (hand->type == eLeapHandType_Left ? "left" : "right"),
            hand->palm.position.x,
            hand->palm.position.y,
            hand->palm.position.z);
        */

        //printf("Left hand (%f, %f, %f)\n", leftHand.dx, leftHand.dy, leftHand.dz);

        if (hand->type == eLeapHandType_Left) {
            detectedLeft = true;

            if (leftHand.active) {
                leftHand.dx = hand->palm.position.x - leftHand.prevX;
                leftHand.dy = hand->palm.position.y - leftHand.prevY;
                leftHand.dz = hand->palm.position.z - leftHand.prevZ;

                leftHand.grab = hand->grab_angle > 2.0;
            }
            else 
                leftHand.active = true;
            

            leftHand.prevX = hand->palm.position.x;
            leftHand.prevY = hand->palm.position.y;
            leftHand.prevZ = hand->palm.position.z;


        }
    }
    if (detectedLeft == false)
        leftHand.active = false;
}

void LeapController::OnImage(const LEAP_IMAGE_EVENT* image) {
    /*
    printf("Image %lli  => Left: %d x %d (bpp=%d), Right: %d x %d (bpp=%d)\n",
        (long long int)image->info.frame_id,
        image->image[0].properties.width, image->image[0].properties.height, image->image[0].properties.bpp * 8,
        image->image[1].properties.width, image->image[1].properties.height, image->image[1].properties.bpp * 8);
    */
}

void LeapController::OnLogMessage(const eLeapLogSeverity severity, const int64_t timestamp,
    const char* message) {
    const char* severity_str;
    switch (severity) {
    case eLeapLogSeverity_Critical:
        severity_str = "Critical";
        break;
    case eLeapLogSeverity_Warning:
        severity_str = "Warning";
        break;
    case eLeapLogSeverity_Information:
        severity_str = "Info";
        break;
    default:
        severity_str = "";
        break;
    }
    printf("[%s][%lli] %s\n", severity_str, (long long int)timestamp, message);
}

void* LeapController::allocate(uint32_t size, eLeapAllocatorType typeHint, void* state) {
    void* ptr = malloc(size);
    return ptr;
}

void LeapController::deallocate(void* ptr, void* state) {
    if (!ptr)
        return;
    free(ptr);
}

void LeapController::OnPointMappingChange(const LEAP_POINT_MAPPING_CHANGE_EVENT* change) {
    if (!connectionHandle)
        return;

    uint64_t size = 0;
    if (LeapGetPointMappingSize(*connectionHandle, &size) != eLeapRS_Success || !size)
        return;

    LEAP_POINT_MAPPING* pointMapping = (LEAP_POINT_MAPPING*)malloc((size_t)size);
    if (!pointMapping)
        return;

    if (LeapGetPointMapping(*connectionHandle, pointMapping, &size) == eLeapRS_Success &&
        pointMapping->nPoints > 0) {
        printf("Managing %u points as of frame %lld at %lld\n", pointMapping->nPoints, (long long int)pointMapping->frame_id, (long long int)pointMapping->timestamp);
    }
    free(pointMapping);
}

void LeapController::OnHeadPose(const LEAP_HEAD_POSE_EVENT* event) {
    printf("Head pose:\n");
    printf("    Head position (%f, %f, %f).\n",
        event->head_position.x,
        event->head_position.y,
        event->head_position.z);
    printf("    Head orientation (%f, %f, %f, %f).\n",
        event->head_orientation.w,
        event->head_orientation.x,
        event->head_orientation.y,
        event->head_orientation.z);
    printf("    Head linear velocity (%f, %f, %f).\n",
        event->head_linear_velocity.x,
        event->head_linear_velocity.y,
        event->head_linear_velocity.z);
    printf("    Head angular velocity (%f, %f, %f).\n",
        event->head_angular_velocity.x,
        event->head_angular_velocity.y,
        event->head_angular_velocity.z);
}
//End-of-Sample


Movement LeapController::calculateMovement(bool isLeft) {
    Hand hand = rightHand;
    if (isLeft) hand = leftHand;

    Movement angledHand;

    // Calculate using z-axis angle
    float angledDy = cos(angle.z) * hand.dy - sin(angle.z) * hand.dx;
    float angledDx = sin(angle.z) * hand.dy + cos(angle.z) * hand.dx;
    float angledDz = hand.dz;

    // Update angled values using x-axis angle
    float nextAngledDy = cos(angle.x) * angledDy + sin(angle.x) * angledDz;
    float nextAngledDx = angledDx;
    float nextAngledDz = -sin(angle.x) * angledDy + cos(angle.x) * angledDz;

    // Update angled values using y-axis angle
    angledHand.dy = nextAngledDy;
    angledHand.dx = -sin(angle.y) * nextAngledDz + cos(angle.y) * nextAngledDx;
    angledHand.dz = cos(angle.y) * nextAngledDz + sin(angle.y) * nextAngledDx;

    std::cout << angledHand.dx << " " << angledHand.dy << " " << angledHand.dz << std::endl;
    return angledHand;
}



