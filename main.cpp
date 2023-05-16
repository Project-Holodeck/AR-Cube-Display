#include <iostream>
#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <chrono>
#include <vector>

#include "Objects.h"
#include "BufferSet.h"

#include <cmath>

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


	//changeCubeCenter(x, y, z)
	cubeOne.changeCubeCenter(0.0f, 0.0f, 0.0f);
	cubeOne.updateCubeVertices();
	cubeOne.addCube(vertices, indices);
	//cubeOne.updateVertices(cubeOne.vertices, vertices, 6);

	// Updating the vertices and indices in the buffers
	mainBuffer.updateArrayBuffer(vertices);
	mainBuffer.updateElementBuffer(indices);

	// Sets the reading data of the vertices array (MUST BE DONE AFTER UPDATING ARRAY BUFFER)
	mainBuffer.linkAttributes(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	mainBuffer.linkAttributes(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	

	auto timeNow = std::chrono::high_resolution_clock::now();

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		double deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - timeNow).count();
		
		//Polls to loop ever 1/fps seconds 
		if (deltaTime <= fps) continue;

		timeNow = std::chrono::high_resolution_clock::now();

		// Erases the entire screen
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		shader.Activate();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);


		Orientation updateAngle = cubeOne.getDirection();
		double currentTime = glfwGetTime();


		Point updatePoint = cubeOne.getCubeCenter();

		if (currentTime - prevTime >= 1) {
			prevTime = currentTime;
			//updateAngle.theta += 0.01;

			//updateAngle.phi += 0.01;
			//rotation += 0.2f;
			updatePoint.x += sqrt(pow(cubeOne.getSize(),2) / 3);
		}

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f)); 
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 10.0f);

		int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shader.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		/*
		
			Code here to change vertices positions every loop...
			
		*/
		/*
		Point updatePoint = cubeOne.getCubeCenter();

		cubeOne.changeCubeCenter(updatePoint.x + 1, updatePoint.y + 1, updatePoint.z);
		cubeOne.updateCubeVertices();

		*/


		


		cubeOne.changeCubeCenter(updatePoint.x, updatePoint.y, updatePoint.z);
		cubeOne.setDirection(updateAngle);
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


