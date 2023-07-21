#pragma once
#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H

#include "Camera.h"
#include <GLFW/glfw3.h>



class MouseInputController {

	public:
		// Controls camera rotation via x and y movement
		void cameraControl(Camera& camera, GLFWwindow* window, int width, int height);
		
};


#endif MOUSE_INPUT_H