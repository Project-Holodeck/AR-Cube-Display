#include "MouseInput.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <cmath>

/*
* TODO: Split the cameraControl function into two functions.
* First function should track deltaX and deltaY in a thread
* Second function should get deltaX and deltaY and reset their values (indirectly)
*/
void MouseInputController::cameraControl(Camera& camera, GLFWwindow* window, int width, int height) {

  
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    // Get the change in x-y mouse translation
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float sensitivity = camera.getSensitivity() / 100;

    float deltaX = mouseX - width/2;
    float deltaY = mouseY - height/2;

    deltaX *= sensitivity;

    deltaY *= sensitivity * width / height; // Multiplies deltaY with a ratio to maintain consistency with deltaX

    // Update the camera angles based on the deltaX and deltaY. In case of large deltas, set a limit to deltaX and deltaY 
    if (abs(deltaX) < 10 && abs(deltaY) < 10) {

        glm::vec3 newOrientation = glm::rotate(camera.getOrientation(), glm::radians(-deltaY), glm::normalize(glm::cross(camera.getOrientation(), camera.getUp())));

        if (!(glm::angle(newOrientation, camera.getUp()) <= glm::radians(5.0f) or glm::angle(newOrientation, -camera.getUp()) <= glm::radians(5.0f))) 
            camera.setOrientation(newOrientation);
        
        camera.setOrientation(glm::rotate(camera.getOrientation(), glm::radians(-deltaX), camera.getUp()));
        
    }
    
    // Move the cursor back to center for tracking deltas. 
    glfwSetCursorPos(window, width / 2, height / 2);
}