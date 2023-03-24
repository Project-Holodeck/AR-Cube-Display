#include "cubeRender.h"
#include <iostream>


CubeRender::CubeRender(float size, float centerX, float centerY) {
	this->size = size;
	this->center[0] = centerX / 960 - 1;
	this->center[1] = centerY/ 540 - 1;
}
void CubeRender::setSize(float size) { this->size = size; }

void CubeRender::setAngle(float angle) { this->angle = angle; }

void CubeRender::setCenter(float x, float y) { this->center[0] = x; this->center[1] = y; }

void CubeRender::setColor(GLfloat red, GLfloat green, GLfloat blue) {
	this->color[0] = red;
	this->color[1] = green;
	this->color[2] = blue;
}

void CubeRender::addCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
	float width = (this->size / 2)/1920;
	float height = (this->size / 2)/1080; 


	this->vertexStart = vertices.size() / 6;
	this->vertexEnd = vertexStart + 4;

	float centerPointX = center[0], centerPointY = center[1];
	std::vector<GLfloat> newVertices = { centerPointX - width, centerPointY - height, 0.0f,	color[0], color[1], color[2],
										 centerPointX + width, centerPointY - height, 0.0f,	color[0], color[1], color[2],
										 centerPointX + width, centerPointY + height, 0.0f,	color[0], color[1], color[2],
										 centerPointX - width, centerPointY + height, 0.0f,	color[0], color[1], color[2] };

	this->indexStart = indices.size();
	this->indexEnd = indexStart + 2 * 3;
	
	std::vector<GLuint> newIndices = { vertexStart + 0, vertexStart + 1, vertexStart + 3,
									   vertexStart + 1, vertexStart + 2, vertexStart + 3 };

	vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
	indices.insert(indices.end(), newIndices.begin(), newIndices.end());

}