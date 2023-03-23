#ifndef CUBE_H
#define CUBE_H

#include "glad/glad.h"
#include <vector>

class Cube {

	private:
		float center[2];
		float angle;
		float size;
		GLfloat color[3];
		GLuint vertexStart, vertexEnd;
		GLuint indexStart, indexEnd;

	public:
		Cube(float size, float centerX, float centerY);
		void setSize(float size);
		void setCenter(float x, float y);
		void setAngle(float angle);
		void setColor(GLfloat red, GLfloat green, GLfloat blue);
		void addCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
		void renderCube(GLuint& VBO, GLuint& VAO);



};


#endif