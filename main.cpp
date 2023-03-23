#include <iostream>
#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include <chrono>
#include <vector>

#include "cube.h"
#include "bufferSet.h"

int width = 1920, height = 1080;

const char* vertexFilename = "default.vert";
const char* fragmentFilename = "fragment.vert";

float speed = 0.001f;
float fps = 0.001f;
int main() {
	glfwInit();

	// Enabling OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	std::vector<GLfloat> vertices;

	std::vector<GLuint> indices;



	GLFWwindow* window = glfwCreateWindow(width, height, "Cube Display", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to open the window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);

	gladLoadGL(); //Loads GLAD to configure OpenGL

	glViewport(0, 0, width, height);

	Shader shader("default.vert", "default.frag");
	shader.Activate();

	BufferSet mainBuffer;

	mainBuffer.generateBuffers();

	Cube cubeOne(1000, 0, 0);
	cubeOne.setColor(0.1f, 0.1f, 0.1f);
	cubeOne.addCube(vertices, indices);

	Cube cubeTwo(100, 1000, 500);
	cubeTwo.setColor(0.5f, 0.1f, 0.1f);
	cubeTwo.addCube(vertices, indices);

	mainBuffer.updateArrayBuffer(vertices);
	mainBuffer.updateElementBuffer(indices);

	//Sets the reading data of the vertices array (MUST BE DONE AFTER UPDATING ARRAY BUFFER)
	mainBuffer.linkAttributes(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	mainBuffer.linkAttributes(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	float dx = speed;

	float timeTrack = 0.0f;
	auto timeNow = std::chrono::high_resolution_clock::now();


	while (!glfwWindowShouldClose(window)) {
		if (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - timeNow).count() <= fps) {
			continue;
		}
		timeNow = std::chrono::high_resolution_clock::now();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Erases the entire screen
		shader.Activate();

		for (int i = 0; i < 4 * 6; i += 6) {

			vertices[i] += dx;
		}
		if (vertices[0] >= 0.8f) {
			dx = -speed;
		}
		else if (vertices[0] <= -1.0f) {
			dx = speed;
		}

		mainBuffer.updateArrayBuffer(vertices);

		mainBuffer.bindVertexArray();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		mainBuffer.unbindVertexArray();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	mainBuffer.deleteBuffers();
	shader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


