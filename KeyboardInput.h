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

		static bool holdPressed;

		static UINT forwardKey;
		static UINT backwardKey;
		static UINT leftKey;
		static UINT rightKey;
		static UINT upKey;
		static UINT downKey;
		static UINT holdKey;

		static UINT* keys[7];
		static bool* keyPressed[7];

		static HHOOK keyboardHook;

		static int counter;
		static bool counterReset;
		static int counterResetVal;

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
		static void setHoldKey(UINT key);
		static void setCounter(int num);
		static void resetCounter(int num);

		// Getters
		static UINT getForwardKey();
		static UINT getBackwardKey();
		static UINT getLeftKey();
		static UINT getRightKey();
		static UINT getUpKey();
		static UINT getDownKey();
		static UINT getHoldKey();

		static bool getForwardPressed();
		static bool getBackwardPressed();
		static bool getLeftPressed();
		static bool getRightPressed();
		static bool getUpPressed();
		static bool getDownPressed();
		static bool getHoldPressed();

		static int getCounter();

		// Controls camera movement
		static void cameraControl(Camera& camera, float deltaTime);

		// Controls counter
		static void counterControl();


};


#endif
