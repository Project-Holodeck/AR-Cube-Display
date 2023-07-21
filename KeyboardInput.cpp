#include "KeyboardInput.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include "ObjectRender.h"


/*
	ForwardKey -> W -> 0x57
	BackwardKey -> S -> 0x53
	LeftKey -> A -> 0x41
	RightKey -> D -> 0x44
	UpKey -> Space -> VK_SPACE or 0x20
	DownKey -> Shift -> VK_SHIFT or 0x10
*/

#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44

UINT KeyboardInputController::forwardKey = 0x0;
UINT KeyboardInputController::backwardKey = 0x0;
UINT KeyboardInputController::leftKey = 0x0;
UINT KeyboardInputController::rightKey = 0x0;
UINT KeyboardInputController::upKey = 0x0;
UINT KeyboardInputController::downKey = 0x0;
UINT KeyboardInputController::holdKey = 0x0;

bool KeyboardInputController::forwardPressed = false;
bool KeyboardInputController::backwardPressed = false;
bool KeyboardInputController::leftPressed = false;
bool KeyboardInputController::rightPressed = false;
bool KeyboardInputController::downPressed = false;
bool KeyboardInputController::upPressed = false;
bool KeyboardInputController::holdPressed = false;

HHOOK KeyboardInputController::keyboardHook = NULL;

UINT* KeyboardInputController::keys[7] = { &forwardKey, &backwardKey, &leftKey, &rightKey, &upKey, &downKey, &holdKey };
bool* KeyboardInputController::keyPressed[7] = { &forwardPressed, &backwardPressed, &leftPressed, &rightPressed, &upPressed, &downPressed, &holdPressed };

int KeyboardInputController::counter = 0;
bool KeyboardInputController::counterReset = false;
int KeyboardInputController::counterResetVal = 0;

KeyboardInputController::KeyboardInputController() {
	forwardKey = VK_W;
	backwardKey = VK_S;
	leftKey = VK_A;
	rightKey = VK_D;
	upKey = VK_SPACE;
	downKey = VK_SHIFT * 10;
	holdKey = VK_LCONTROL;

	forwardPressed = false;
	backwardPressed = false;
	leftPressed = false;
	rightPressed = false;
	downPressed = false;
	upPressed = false;
	holdPressed = false;

	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);

	counter = 0;
	counterReset = false;
	counterResetVal = 0;
}

LRESULT CALLBACK KeyboardInputController::KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		KBDLLHOOKSTRUCT* keyboardInfo = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

		if (wParam == WM_KEYDOWN) {
			UINT keyCode = keyboardInfo->vkCode;
			bool found = false;
			for (int i = 0; i < sizeof(keys) / sizeof(keys[0]) && !found; i++)
				if (keyCode == *keys[i]) {
					*keyPressed[i] = true;
					found = true;
				}

		}

		else if (wParam == WM_KEYUP) {
			UINT keyCode = keyboardInfo->vkCode;
			bool found = false;
			for (int i = 0; i < sizeof(keyPressed) / sizeof(keyPressed[0]) && !found; i++)
				if (keyCode == *keys[i]) {
					*keyPressed[i] = false;
					found = true;
				}
		}

		counterControl();
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Setters
void KeyboardInputController::unHook() { UnhookWindowsHookEx(keyboardHook);  }
void KeyboardInputController::setForwardKey(UINT key) { forwardKey = key; }
void KeyboardInputController::setBackwardKey(UINT key) { backwardKey = key; }
void KeyboardInputController::setLeftKey(UINT key) { leftKey = key; }
void KeyboardInputController::setRightKey(UINT key) { rightKey = key; }
void KeyboardInputController::setUpKey(UINT key) { upKey = key; }
void KeyboardInputController::setDownKey(UINT key) { downKey = key; }
void KeyboardInputController::setHoldKey(UINT key) { holdKey = key; }
void KeyboardInputController::resetCounter(int num) { counterReset = true; counterResetVal = num; }

// Getters
UINT KeyboardInputController::getForwardKey() { return forwardKey; }
UINT KeyboardInputController::getBackwardKey() { return backwardKey; }
UINT KeyboardInputController::getLeftKey() { return leftKey; }
UINT KeyboardInputController::getRightKey() { return rightKey; }
UINT KeyboardInputController::getUpKey() { return upKey; }
UINT KeyboardInputController::getDownKey() { return downKey; }
UINT KeyboardInputController::getHoldKey() { return holdKey; }

bool KeyboardInputController::getForwardPressed() { return forwardPressed; }
bool KeyboardInputController::getBackwardPressed() { return backwardPressed; }
bool KeyboardInputController::getLeftPressed() { return leftPressed; }
bool KeyboardInputController::getRightPressed() { return rightPressed; }
bool KeyboardInputController::getUpPressed() { return upPressed; }
bool KeyboardInputController::getDownPressed() { return downPressed; }
bool KeyboardInputController::getHoldPressed() { return holdPressed; }

int KeyboardInputController::getCounter() { return counter; }

// Updates camera based on key states
void KeyboardInputController::cameraControl(Camera& camera, float deltaTime) {

	float velocity = camera.getVelocity() / 30;

	if (!holdPressed) {
		if (forwardPressed)
			camera.setPosition(camera.getPosition() + velocity * camera.getOrientation());
		if (backwardPressed)
			camera.setPosition(camera.getPosition() + velocity * -camera.getOrientation());
		if (leftPressed)
			camera.setPosition(camera.getPosition() + velocity * -glm::normalize(glm::cross(camera.getOrientation(), camera.getUp())));
		if (rightPressed)
			camera.setPosition(camera.getPosition() + velocity * glm::normalize(glm::cross(camera.getOrientation(), camera.getUp())));
		if (upPressed)
			camera.setPosition(camera.getPosition() + velocity * camera.getUp());
		if (downPressed)
			camera.setPosition(camera.getPosition() + velocity * -camera.getUp());
	}
}

// Updates counter 
void KeyboardInputController::counterControl() {

	// Reset counter if given the signal to. Reset signal.
	if (counterReset) {
		counter = counterResetVal;
		counterReset = false;
	}

	// Increments counter if CTRL + D is pressed. Decrements if CTRL + A is pressed. 
	if (holdPressed) {
		if (leftPressed)
			counter--;
		else if (rightPressed)
			counter++;
	}
}