#include "ObjectRender.h"
#include <iostream>


ObjectRender::ObjectRender() {
	color = { 0, 0, 0 };
	vertexStart = 0;
	vertexEnd = 0;
	indexStart = 0;
	indexEnd = 0;
}

ObjectRender::ObjectRender(Color _color, int _objectID) : color(_color), objectID(_objectID) {

}


void ObjectRender::setColor(GLfloat red, GLfloat green, GLfloat blue) { color = { red, green, blue }; }

void ObjectRender::setColor(Color color) { this->color = color; }

void ObjectRender::updateVertices(Point positions[], std::vector<GLfloat>& vertices, int stride) {
	int screenWidth = 1920;
	int screenHeight = 1080;

	for (int i = 0; i < (this->vertexEnd - this->vertexStart)/stride; i++ ) {
		vertices[vertexStart + i*stride] = positions[i].x / screenWidth;
		vertices[vertexStart + i * stride + 1] = positions[i].y / screenWidth;
		vertices[vertexStart + i * stride + 2] = positions[i].z / screenHeight;
		//std::cout << positions[i].x << " " << positions[i].y << " " << positions[i].z << std::endl;
	}
	//std::cout << "----------------" << std::endl;
}


void ObjectRender::updateColor(std::vector<GLfloat>& vertices, int stride) {

	for (int i = vertexStart; i < vertexEnd; i += stride) {
		vertices[i + 3] = color.red;
		vertices[i + 4] = color.green;
		vertices[i + 5] = color.blue;
	}

}


void ObjectRender::deleteObject(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, std::vector<ObjectRender>& objectVector) {

	std::vector<GLfloat> newVertices;
	std::vector<GLuint> newIndices;

	int vertexSize = vertexEnd - vertexStart;
	int indicesSize = indexEnd - indexStart;
	
	newVertices.insert(newVertices.end(), vertices.begin(), vertices.begin() + vertexStart);
	newVertices.insert(newVertices.end(), vertices.begin() + vertexEnd, vertices.end());

	for (int i = 0; i < newVertices.size(); i++)
		std::cout << newVertices[i] << " ";
	std::cout << std::endl;
	vertices.clear();

	vertices = newVertices;

	newIndices.insert(newIndices.end(), indices.begin(), indices.begin() + indexStart);
	newIndices.insert(newIndices.end(), indices.begin() + indexEnd, indices.end());
	
	indices.clear();
	indices = newIndices;

	for (int i = 0; i < objectVector.size(); i++) {
		if (i == this->objectID || i < this->objectID)
			continue;

		objectVector[i].vertexStart -= vertexSize;
		objectVector[i].vertexEnd -= vertexSize;

		objectVector[i].indexStart -= indicesSize;
		objectVector[i].indexEnd -= indicesSize;

		objectVector[i].objectID--;

	}

	objectVector.erase(objectVector.begin() + this->objectID);
}
