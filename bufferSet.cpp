#include "bufferSet.h"
#include <glad/glad.h>
#include <iostream>

void BufferSet::generateBuffers() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
}

void BufferSet::linkAttributes(GLuint location, GLuint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(location, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(location);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BufferSet::updateArrayBuffer(std::vector<GLfloat>& vertices) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void BufferSet::updateElementBuffer(std::vector<GLuint>& indices) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


void BufferSet::bindVertexArray() {
	glBindVertexArray(VAO);
}

void BufferSet::unbindVertexArray() {
	glBindVertexArray(0);
}

void BufferSet::deleteBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}