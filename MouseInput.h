#pragma once
#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H

#include <Windows.h>
#include "Camera.h"
#include <GLFW/glfw3.h>

class MouseInputController {
	private:
		static HHOOK mouseHook;

		bool leftPressed;
		bool rightPressed;


	public:
		// Set up mouseInput
		MouseInputController();

		static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

		// Set up
		void unHook();
		static void setCursorX(int x);
		static void setCursorY(int y);

		// Getters
		static int getCursorX();
		static int getCursorY();
		int getLeftPressed();
		int getRightPressed();

		// Controls camera rotation via x and y movement
		void cameraControl(Camera& camera, GLFWwindow* window, int width, int height);
		

};


#endif MOUSE_INPUT_H