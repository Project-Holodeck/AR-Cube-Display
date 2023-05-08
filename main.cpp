#include <iostream>
#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include <chrono>
#include <vector>

#include "Objects.h"
#include "BufferSet.h"

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

	//Cube(center, size, angle)
	Cube cubeOne(objects, { 0, 0, 0 }, 100, { 0, 0 }, { 1.0f, 0.4f, 0.4f }, 0);


	//changeCubeCenter(x, y, z)
	cubeOne.changeCubeCenter(-1920/2, -1080/2, 0);
	cubeOne.updateCubeVertices();
	cubeOne.addCube(vertices, indices);

	// Updating the vertices and indices in the buffers
	mainBuffer.updateArrayBuffer(vertices);
	mainBuffer.updateElementBuffer(indices);

	// Sets the reading data of the vertices array (MUST BE DONE AFTER UPDATING ARRAY BUFFER)
	mainBuffer.linkAttributes(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	mainBuffer.linkAttributes(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	

	auto timeNow = std::chrono::high_resolution_clock::now();


	while (!glfwWindowShouldClose(window)) {
		double deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - timeNow).count();
		
		//Polls to loop ever 1/fps seconds 
		if (deltaTime <= fps) continue;

		timeNow = std::chrono::high_resolution_clock::now();

		// Erases the entire screen
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); 

		shader.Activate();

		/*
		
			Code here to change vertices positions every loop...
			
		*/
		Point updatePoint = cubeOne.getCubeCenter();

		cubeOne.changeCubeCenter(updatePoint.x + 1, updatePoint.y + 1, 0);
		cubeOne.updateCubeVertices();

		cubeOne.updateVertices(cubeOne.vertices, vertices, 6);

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
	mainBuffer.deleteBuffers();
	shader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


