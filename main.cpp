#include <iostream>
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

#include <cmath>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif


int width = 1920, height = 1080;

float fps = 60.0f;

float handSensitivity = 10;

int main() {
	glfwInit();

	// Enabling OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::vector<GLfloat> vertices = {};
	std::vector<GLuint> indices = {};
	std::vector<ObjectRender*> objects = {};

	GLFWwindow* window = glfwCreateWindow(width, height, "Cube Display", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to open the window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL(); // Loads GLAD to configure OpenGL

	glViewport(0, 0, width, height);

	Shader shader("default.vert", "default.frag");
	shader.Activate();

	// Initialize vertex and element buffers
	BufferSet mainBuffer;
	mainBuffer.generateBuffers();

	// For testing purposes. Format is: Cube(center in absolute coordinate, block_type, angle <Θ,φ>, color, id)
	Cube cubeOne({ 0, 0, 0 }, BLOCK, { 0.0f, 0 }, { 1.0f, 0.4f, 0.4f }, 0);
	cubeOne.addCube(vertices, indices);
	Cube cubeTwo({ 0, 0, 0 }, BLOCK, { 0.0f, 0 }, { 1.0f, 0.0f, 0.4f }, 1);
	cubeTwo.addCube(vertices, indices);

	objects.push_back(&cubeOne);
	objects.push_back(&cubeTwo);

	// Example of changing the cube's absolute positioning in OpenGL space and updating accordingly
	cubeOne.setCubeCenter(0.0f, 0.0f, 0.0f);
	cubeOne.updateCubeVertices();
	cubeOne.updateVertices(cubeOne.vertices, vertices, 6);

	// Example of setting integer coordinates and updating accordingly
	cubeTwo.setCubeCoordinate(1, 0, 0);
	cubeTwo.updateCubeVertices();
	cubeTwo.updateVertices(cubeTwo.vertices, vertices, 6);

	// Updating the vertices and indices in the buffers. Must do this for every vertices and indices update
	mainBuffer.updateArrayBuffer(vertices);
	mainBuffer.updateElementBuffer(indices);

	// Sets the reading data of the vertices array (MUST BE DONE AFTER UPDATING ARRAY BUFFER)
	mainBuffer.linkAttributes(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Vertex, Stride of 6, Offset of 0, Location = 0 in default.vert
	mainBuffer.linkAttributes(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color, Stride of 6, Offset of 3, Location = 1 in default.vert 
	glEnable(GL_DEPTH_TEST);

	// Set up Input Controller (Keyboard)
	KeyboardInputController keyController;
	MouseInputController mouseController;	
	
	// Setting up the Leap Motion Controller
	LeapController leapController;
	leapController.setup();
	leapController.connectionHandle = OpenConnection();

	LEAP_ALLOCATOR allocator = { leapController.allocate, leapController.deallocate, NULL };
	LeapSetAllocator(*(leapController.connectionHandle), &allocator);
	LeapSetPolicyFlags(*(leapController.connectionHandle), eLeapPolicyFlag_Images | eLeapPolicyFlag_MapPoints, 0);
		
	// In Radians
	leapController.angle = {M_PI/4, 0, 0};

	// Configure camera settings 
	Camera camera;
	camera.setVelocity(0.4f);
	camera.setSensitivity(2.0f);
	camera.setShaderID(shader.ID);
	camera.setPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	camera.setFOV(45.0f, width, height, 0.1f, 10.0f);
	camera.updateCamera(); // Updates camera view 
	
	/* Game Loop */
	auto timeNow = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window)) {

		double deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - timeNow).count();

		//Polls to loop over 1/fps seconds 
		if (deltaTime <= 1/fps) continue;

		timeNow = std::chrono::high_resolution_clock::now();

		// Erases the entire screen
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		double currentTime = glfwGetTime();

		// Camera Controls, enable when window is focused
		if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0) {
			keyController.cameraControl(camera, deltaTime); // Updates value of camera position
			mouseController.cameraControl(camera, window, width, height); // Updates value of camera angle
			camera.updateCamera(); // Updates camera view 
		}

		// Cube Updates. Temporary
		Cube* selectedCube = (Cube*)(objects.at(keyController.getCounter() % objects.size()));
		keyController.resetCounter(keyController.getCounter() % objects.size());
		Point updatePoint = selectedCube->getCubeCenter();
		Orientation updateAngle = selectedCube->getDirection();
		if (leapController.leftHand.active && leapController.leftHand.grab) {
			printf("Left hand (%f, %f, %f)\n", leapController.leftHand.dx, leapController.leftHand.dy, leapController.leftHand.dz);
			Movement movement = leapController.calculateMovement(true);

			updatePoint.x += (double)movement.dx * handSensitivity;
			updatePoint.y += (double)movement.dz * handSensitivity;
			updatePoint.z += (double)movement.dy * handSensitivity;
		}

		selectedCube->setCubeCenter(updatePoint.x, updatePoint.y, updatePoint.z);
		selectedCube->setDirection(updateAngle);
		selectedCube->updateCubeVertices();
		selectedCube->updateVertices(selectedCube->vertices, vertices, 6);

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

	mainBuffer.deleteBuffers();
	shader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


