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
		int objectID;

	public:
		ObjectRender();
		ObjectRender(Color _color, int _objectID);

		// Setters
		void setColor(GLfloat red, GLfloat green, GLfloat blue);
		void setColor(Color color);

		// Universal Object Functions
		void updateVertices(Point positions[], std::vector<GLfloat>& vertices, int stride);
		void updateColor(std::vector<GLfloat>& vertices, int stride);
		void deleteObject(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<ObjectRender>& objectVector);
};

#endif OBJECT_RENDER_H