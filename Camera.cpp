#include "Camera.h"
#include <iostream>


Camera::Camera() {
	shaderID = NULL;
}

void Camera::updateCamera() {

	glm::mat4 view = glm::mat4(1.0f);
	
	view = glm::lookAt(position, position + orientation, up);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));

}

void Camera::setFOV(float FOVdeg, int width, int height, float nearPlane, float farPlane) {
	glm::mat4 proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
}

// Setters
void Camera::setVelocity(float velocity) { this->velocity = velocity; }
void Camera::setSensitivity(float sensitivity) { this->sensitivity = sensitivity; }

void Camera::setOrientation(glm::vec3 orientation) { this->orientation = orientation; }

void Camera::setPosition(glm::vec3 position) { this->position = position; }

void Camera::setPositionX(float x) { this->position.x = x; }
void Camera::setPositionY(float y) { this->position.y = y; }
void Camera::setPositionZ(float z) { this->position.z = z; }


void Camera::setShaderID(GLuint ID) { this->shaderID = ID; }

// Getters
float Camera::getVelocity() { return velocity; }
float Camera::getSensitivity() { return sensitivity; }

float Camera::getPositionX() { return position.x; }
float Camera::getPositionY() { return position.y; }
float Camera::getPositionZ() { return position.z; }
glm::vec3 Camera::getPosition() { return position; }

glm::vec3 Camera::getOrientation() { return orientation; }
glm::vec3 Camera::getUp() { return up; }