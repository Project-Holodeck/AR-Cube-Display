﻿#include <iostream>
#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <chrono>
#include <vector>

#include "Cube.h"
#include "BufferSet.h"

#include <cmath>

#include "KeyboardInput.h"
#include "MouseInput.h"
#include "Camera.h"

#include <leap/LeapC.h>

#include "LeapController.h"


int width = 1920, height = 1080;

float fps = (1/60.0f);

int main() {
	glfwInit();

	// Enabling OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	std::vector<GLfloat> vertices = {
	};

	std::vector<GLuint> indices = {
	};

	std::vector<ObjectRender> objects = {
	};

	GLFWwindow* window = glfwCreateWindow(width, height, "Cube Display", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to open the window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);

	gladLoadGL(); // Loads GLAD to configure OpenGL

	glViewport(0, 0, width, height);

	// (Cannot replace char* with variables)
	Shader shader("default.vert", "default.frag");
	shader.Activate();

	BufferSet mainBuffer;

	// Initializing EBO, VBO, and VAO in the BufferSet class
	mainBuffer.generateBuffers();

	//Cube(objectVector, center, size, angle <Θ,φ>, color, id)
	Cube cubeOne(objects, { 0, 0, 0 }, 300, { 0.0f, 0 }, { 1.0f, 0.4f, 0.4f }, 0);
	Cube cubeTwo(objects, { 0, 0, 0 }, 300, { 0.0f, 0 }, { 1.0f, 0.0f, 0.4f }, 1);


	//changeCubeCenter(x, y, z)
	cubeOne.changeCubeCenter(0.0f, 0.0f, 0.0f);
	cubeOne.updateCubeVertices();
	cubeOne.addCube(vertices, indices);

	cubeTwo.changeCubeCenter(173.2f, 0.0f, 0.0f); // Sidelength = Sqrt(Size^2 / 3)
	cubeTwo.updateCubeVertices();
	cubeTwo.addCube(vertices, indices);

	// Updating the vertices and indices in the buffers
	mainBuffer.updateArrayBuffer(vertices);
	mainBuffer.updateElementBuffer(indices);

	// Sets the reading data of the vertices array (MUST BE DONE AFTER UPDATING ARRAY BUFFER)
	mainBuffer.linkAttributes(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Vertex, Stride of 6, Offset of 0, Location = 0 in default.vert
	mainBuffer.linkAttributes(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color, Stride of 6, Offset of 3, Location = 1 in default.vert 
	

	auto timeNow = std::chrono::high_resolution_clock::now();

	float rotation = 0.0f;
	float freeValue = 0.0f;
	double prevTime = glfwGetTime();

	// Set up Input Controller (Keyboard)
	KeyboardInputController keyController;
	MouseInputController mouseController;

	// Set up Camera
	Camera camera;

	camera.setVelocity(0.4f);
	camera.setSensitivity(2.0f);
	camera.setShaderID(shader.ID);
	camera.setPosition(glm::vec3(0.0f, 0.0f, 2.0f));

	glEnable(GL_DEPTH_TEST);

	/*
	LeapController leapController;
	
	ConnectionCallbacks.on_connection = &(leapController.OnConnect);
	ConnectionCallbacks.on_device_found = &(leapController.OnDevice);
	ConnectionCallbacks.on_frame = &(leapController.OnFrame);
	ConnectionCallbacks.on_image = &(leapController.OnImage);
	ConnectionCallbacks.on_point_mapping_change = &(leapController.OnPointMappingChange);
	ConnectionCallbacks.on_log_message = &(leapController.OnLogMessage);
	ConnectionCallbacks.on_head_pose = &(leapController.OnHeadPose);


	leapController.connectionHandle = OpenConnection();
	{
		LEAP_ALLOCATOR allocator = { leapController.allocate, leapController.deallocate, NULL };
		LeapSetAllocator(*(leapController.connectionHandle), &allocator);
	}
	LeapSetPolicyFlags(*(leapController.connectionHandle), eLeapPolicyFlag_Images | eLeapPolicyFlag_MapPoints, 0);
	*/
	while (!glfwWindowShouldClose(window)) {
		double deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - timeNow).count();

		//Polls to loop ever 1/fps seconds 
		if (deltaTime <= fps) continue;

		timeNow = std::chrono::high_resolution_clock::now();

		// Erases the entire screen
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		Orientation updateAngle = cubeOne.getDirection();
		double currentTime = glfwGetTime();

		Point updatePoint = cubeOne.getCubeCenter();

		// Camera Controls
		keyController.cameraControl(camera, deltaTime); // Updates value of camera position
		mouseController.cameraControl(camera, window, width, height); // Updates value of camera angle
		camera.setFOV(45.0f, width, height, 0.1f, 10.0f); // Updates FOV of camera
		camera.updateCamera(); // Updates camera view 

		// Cube Updates
		cubeOne.changeCubeCenter(updatePoint.x, updatePoint.y, updatePoint.z);
		cubeOne.setDirection(updateAngle);
		cubeOne.updateCubeVertices();
		cubeOne.updateVertices(cubeOne.vertices, vertices, 6);


		cubeTwo.updateCubeVertices();
		cubeTwo.updateVertices(cubeTwo.vertices, vertices, 6);
		

		// Updates the array buffer with any new changes to the vertices vector
		mainBuffer.updateArrayBuffer(vertices);
		mainBuffer.updateElementBuffer(indices);
		

		
		// Draws all the triangles from the indices vector
		mainBuffer.bindVertexArray();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		mainBuffer.unbindVertexArray();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	keyController.unHook();
	mouseController.unHook();

	mainBuffer.deleteBuffers();
	shader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


