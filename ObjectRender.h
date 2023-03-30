#ifndef OBJECT_RENDER_H
#define OBJECT_RENDER_H

#include <glad/glad.h>
#include "PhysicsEngine.hpp"
#include <vector>

struct Color {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};

class ObjectRender {
	
	protected:
		Color color;
		GLuint vertexStart, vertexEnd;
		GLuint indexStart, indexEnd;

	public:
		ObjectRender();

		// Setters
		void setColor(GLfloat red, GLfloat green, GLfloat blue);
		void setColor(Color color);

		// Universal Object Functions
		void updateVertices(std::vector<Point> positions, std::vector<GLfloat>& vertices, int stride);
		void updateColor(std::vector<GLfloat>& vertices, int stride);
		void deleteObject(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
};

#endif OBJECT_RENDER_H