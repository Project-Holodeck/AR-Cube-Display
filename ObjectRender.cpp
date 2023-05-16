#include "ObjectRender.h"
#include "Objects.h"
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



void Cube::addCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
	int screenWidth = 1920;
	int screenHeight = 1080;

	// Sets index of vertices for cube (Total of 4 vertices)
	this->vertexStart = vertices.size();
	this->vertexEnd = vertexStart + 8 * 6;

	// Sets positions of each vertex (stride of 3) and color (stride of 3)
	std::vector<GLfloat> newVertices;

	for (int i = 0; i < 8; i++) {

		// Positions with stride of 3
		newVertices.push_back(this->vertices[i].x / screenWidth);
		newVertices.push_back(this->vertices[i].y / screenWidth);
		newVertices.push_back(this->vertices[i].z / screenHeight);

		// Color with stride of 3
		newVertices.push_back(color.red + i/10.0f);
		newVertices.push_back(color.green + i/10.0f);
		newVertices.push_back(color.blue + i/10.0f);

	}

	// Sets index of indices for cube (Total of 6 indices)
	this->indexStart = indices.size();
	this->indexEnd = indexStart + 12 * 3;



	// Sets vertex indices for the indices vector to be appended
	std::vector<GLuint> newIndices = {
		// Top Face
		vertexStart + 0, vertexStart + 1, vertexStart + 2,
		vertexStart + 0, vertexStart + 2, vertexStart + 3,

		// Side Face
		vertexStart + 0, vertexStart + 4, vertexStart + 5,
		vertexStart + 0, vertexStart + 1, vertexStart + 5,

		vertexStart + 0, vertexStart + 4, vertexStart + 7,
		vertexStart + 0, vertexStart + 3, vertexStart + 7,

		vertexStart + 2, vertexStart + 1, vertexStart + 5,
		vertexStart + 2, vertexStart + 6, vertexStart + 5,

		vertexStart + 2, vertexStart + 6, vertexStart + 7,
		vertexStart + 2, vertexStart + 3, vertexStart + 7,

		// Bottom Face
		vertexStart + 4, vertexStart + 5, vertexStart + 6,
		vertexStart + 4, vertexStart + 6, vertexStart + 7
	};

	// Merges new vertices and indices to current vertices and indicies vector
	vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
	indices.insert(indices.end(), newIndices.begin(), newIndices.end());

}