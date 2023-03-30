#include "ObjectRender.h"
#include "Objects.h"


ObjectRender::ObjectRender() {
	color = { 0, 0, 0 };
	vertexStart = 0;
	vertexEnd = 0;
	indexStart = 0;
	indexEnd = 0;
}


void ObjectRender::setColor(GLfloat red, GLfloat green, GLfloat blue) { color = { red, green, blue }; }

void ObjectRender::setColor(Color color) { this->color = color; }

void ObjectRender::updateVertices(std::vector<Point> positions, std::vector<GLfloat>& vertices, int stride) {
	int screenWidth = 1920;
	int screenHeight = 1080;
	for (int i = 0; i < (vertexStart-vertexEnd)/6; i++) {
		vertices[i*stride] = positions[i].x / screenWidth;
		vertices[i*stride + 1] = positions[i].y / screenHeight;
		vertices[i*stride + 2] = 1 /*positions[i].z*/;
	}

}


void ObjectRender::updateColor(std::vector<GLfloat>& vertices, int stride) {

	for (int i = vertexStart; i < vertexEnd; i += stride) {
		vertices[i] = color.red;
		vertices[i + 1] = color.green;
		vertices[i + 2] = color.blue;
	}

}


void ObjectRender::deleteObject(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
	int indexSize = vertexEnd - vertexStart;

	std::vector<GLfloat> newVertices;
	std::vector<GLuint> newIndices;

	for (int i = indexEnd; i < indices.size(); i++) 
		indices[indexEnd] -= indexSize;
	
	newVertices.insert(newVertices.begin(), vertices.begin(), vertices.begin() + vertexStart);


}



void Cube::addCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
	int screenWidth = 1920;
	int screenHeight = 1080;
	// Sets index of vertices for cube (Total of 4 vertices)
	this->vertexStart = vertices.size() / 6;
	this->vertexEnd = vertexStart + 8;

	// Sets positions of each vertex (stride of 3) and color (stride of 3)
	std::vector<GLfloat> newVertices;

	for (int i = 0; i < 8; i++) {

		// Positions with stride of 3
		newVertices.push_back(this->vertices[i].x / screenWidth * 2 - 1);
		newVertices.push_back(this->vertices[i].y / screenHeight * 2 - 1);
		newVertices.push_back(0.0f /*positions[i].z */);

		// Color with stride of 3
		newVertices.push_back(color.red);
		newVertices.push_back(color.green);
		newVertices.push_back(color.blue);

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