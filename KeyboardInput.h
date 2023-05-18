#pragma once

#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <Windows.h>
#include "Camera.h"

class KeyboardInputController {

	private:
		static bool forwardPressed;
		static bool backwardPressed;

		static bool leftPressed;
		static bool rightPressed;

		static bool upPressed;
		static bool downPressed;

		static UINT forwardKey;
		static UINT backwardKey;
		static UINT leftKey;
		static UINT rightKey;
		static UINT upKey;
		static UINT downKey;

		static UINT* keys[6];
		static bool* keyPressed[6];

		static HHOOK keyboardHook;



	public:

		// Initialize the private variables
		KeyboardInputController();

		// Updates key states via hook
		static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

		// Setters
		void unHook();

		static void setForwardKey(UINT key);
		static void setBackwardKey(UINT key);
		static void setLeftKey(UINT key);
		static void setRightKey(UINT key);
		static void setUpKey(UINT key);
		static void setDownKey(UINT key);

		// Getters
		static UINT getForwardKey();
		static UINT getBackwardKey();
		static UINT getLeftKey();
		static UINT getRightKey();
		static UINT getUpKey();
		static UINT getDownKey();

		static bool getForwardPressed();
		static bool getBackwardPressed();
		static bool getLeftPressed();
		static bool getRightPressed();
		static bool getUpPressed();
		static bool getDownPressed();

		// Controls camera movement
		void cameraControl(Camera& camera, float deltaTime);

};


#endif
