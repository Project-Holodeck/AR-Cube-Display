#ifndef LEAP_CONTROLLER_H
#define LEAP_CONTROLLER_H

#include <leap/LeapC.h>
#include "LeapConnection.h"

struct Hand {
	float dx, dy, dz;
	float prevX, prevY, prevZ;
	bool active = false;
	bool grab = false;
};

struct Movement {
	float dx, dy, dz;
};

struct Angle {
	float x, y, z;
};

class LeapController {
	public:

		static Hand leftHand;
		static Hand rightHand;
		static Angle angle;

		static LEAP_CONNECTION* connectionHandle;

		static void setup(void);

		static void OnConnect(void);
		static void OnDevice(const LEAP_DEVICE_INFO* props);
		static void OnFrame(const LEAP_TRACKING_EVENT* frame);
		static void OnImage(const LEAP_IMAGE_EVENT* image);
		static void OnLogMessage(const eLeapLogSeverity severity, const int64_t timestamp,
			const char* message);
		static void OnHeadPose(const LEAP_HEAD_POSE_EVENT* event);
		static void OnPointMappingChange(const LEAP_POINT_MAPPING_CHANGE_EVENT* change);

		static void* allocate(uint32_t size, eLeapAllocatorType typeHint, void* state);
		static void deallocate(void* ptr, void* state);

		static Movement calculateMovement(bool isLeft);
};


#endif