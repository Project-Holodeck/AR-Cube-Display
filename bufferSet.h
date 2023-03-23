#ifndef BUFFER_SET_H
#define BUFFER_SET_H

#include <glad/glad.h>
#include <vector>

class BufferSet {

	private:

	public:
		GLuint VBO, VAO, EBO;

		void generateBuffers();
		void linkAttributes(GLuint location, GLuint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

		void updateArrayBuffer(std::vector<GLfloat>& vertices);
		void updateElementBuffer(std::vector<GLuint>& indices);

		void bindVertexArray();
		void unbindVertexArray();

		void deleteBuffers();

};

#endif