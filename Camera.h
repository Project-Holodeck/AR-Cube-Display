#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <Glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {

	private:
		float velocity;
		float sensitivity;

		glm::vec3 position;

		glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f);

		GLuint shaderID;

	public:
		
		// Initialize private variable values
		Camera();

		void updateCamera();


		// Setters
		void setVelocity(float velocity);

		void setSensitivity(float sensitivity);

		void setOrientation(glm::vec3 orientation);

		void setShaderID(GLuint ID);

		void setPosition(glm::vec3 position); 
		void setPositionX(float x);
		void setPositionY(float y);
		void setPositionZ(float z);

		void setFOV(float FOVdeg, int width, int height, float nearPlane, float farPlane);

		void setPrevX(int x);
		void setPrevY(int y);

		// Getters
		float getVelocity();
		float getSensitivity();

		float getPositionX();
		float getPositionY();
		float getPositionZ();

		glm::vec3 getOrientation();
		glm::vec3 getUp();
		glm::vec3 getPosition();
};

#endif