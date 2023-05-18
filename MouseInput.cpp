#include "MouseInput.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

HHOOK MouseInputController::mouseHook = NULL;

MouseInputController::MouseInputController() {
	this->mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseInputController::MouseProc, NULL, 0);
}

LRESULT CALLBACK MouseInputController::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_MOUSEMOVE)
    {
        MSLLHOOKSTRUCT* mouseInfo = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

    }

    // Call the next hook procedure in the hook chain
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

// Setters
void MouseInputController::unHook() {
	UnhookWindowsHookEx(mouseHook);
}

// Getters
int MouseInputController::getLeftPressed() { return this->leftPressed; }
int MouseInputController::getRightPressed() { return this->rightPressed; }

void MouseInputController::cameraControl(Camera& camera, GLFWwindow* window, int width, int height) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);


    float sensitivity = camera.getSensitivity() / 100;

    float deltaX = mouseX - width/2;
    float deltaY = mouseY - height/2;

    deltaX *= sensitivity;

    deltaY *= sensitivity * width / height;


    glm::vec3 newOrientation = glm::rotate(camera.getOrientation(), glm::radians(-deltaY), glm::normalize(glm::cross(camera.getOrientation(), camera.getUp())));

    if (!(glm::angle(newOrientation, camera.getUp()) <= glm::radians(5.0f) or glm::angle(newOrientation, -camera.getUp()) <= glm::radians(5.0f))) {
        camera.setOrientation(newOrientation);
    }

    camera.setOrientation(glm::rotate(camera.getOrientation(), glm::radians(-deltaX), camera.getUp()));
    
    glfwSetCursorPos(window, width / 2, height / 2);
}