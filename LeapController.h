#ifndef LEAP_CONTROLLER_H
#define LEAP_CONTROLLER_H

#include <leap/LeapC.h>
#include "LeapConnection.h"

class LeapController {
	public:

		static LEAP_CONNECTION* connectionHandle;

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

};


#endif