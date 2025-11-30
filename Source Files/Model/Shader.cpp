#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/ShaderBuilder.h"
#include "../../Header Files/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Shader::Shader(const ShaderFiles files, const BufferValues bufferValues)
{
	this->programId = ShaderBuilder::buildShader(files);
	this->initVao();
	this->initVbos(bufferValues);
	this->initUniformReferences();

	this->uniforms.creationTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.isVisible.value = true;
}

Shader::~Shader()
{
	glDeleteProgram(this->programId);
	glDeleteBuffers(1, &this->addresses.vertices);
	glDeleteBuffers(1, &this->addresses.colors);
	glDeleteBuffers(1, &this->addresses.normals);
	glDeleteBuffers(1, &this->addresses.indices);
	glDeleteBuffers(1, &this->addresses.textures);
	glDeleteVertexArrays(1, &this->addresses.vao);
}

void Shader::render(const Transform model, const BufferValues values)
{
	glUseProgram(this->programId);
	this->updateUniformValues(model);
	this->passUniforms();
	this->draw(values);
	this->checkGLErrors();
}

void Shader::initVao()
{
	glGenVertexArrays(1, &this->addresses.vao);
	glBindVertexArray(this->addresses.vao);
}

void Shader::initVbos(const BufferValues values)
{
	// Generates and makes active the VBO for the vertices
	glGenBuffers(1, &this->addresses.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.vertices);
	glBufferData(GL_ARRAY_BUFFER, values.vertices.size() * sizeof(fvec3), values.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// Generates and makes active the VBO for the colors
	glGenBuffers(1, &this->addresses.colors);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.colors);
	glBufferData(GL_ARRAY_BUFFER, values.colors.size() * sizeof(fvec4), values.colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);

	// Generates and makes active the VBO for the normals
	glGenBuffers(1, &this->addresses.normals);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.normals);
	glBufferData(GL_ARRAY_BUFFER, values.normals.size() * sizeof(fvec3), values.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(2);

	// Generates and makes active the VBO for the texture coordinates
	glGenBuffers(1, &this->addresses.textures);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.textures);
	glBufferData(GL_ARRAY_BUFFER, values.textures.size() * sizeof(fvec2), values.textures.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(3);

	// Generates and makes active the EBO for the indices
	glGenBuffers(1, &this->addresses.indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->addresses.indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, values.indices.size() * sizeof(unsigned int), values.indices.data(), GL_STATIC_DRAW);
}

void Shader::initUniformReferences()
{
	this->uniforms.projectionMatrix.location = glGetUniformLocation(this->programId, this->uniforms.projectionMatrix.name.c_str());
	this->uniforms.modelMatrix.location = glGetUniformLocation(this->programId, this->uniforms.modelMatrix.name.c_str());
	this->uniforms.viewMatrix.location = glGetUniformLocation(this->programId, this->uniforms.viewMatrix.name.c_str());
	this->uniforms.viewPosition.location = glGetUniformLocation(this->programId, this->uniforms.viewPosition.name.c_str());
	this->uniforms.creationTime.location = glGetUniformLocation(this->programId, this->uniforms.creationTime.name.c_str());
	this->uniforms.currentTime.location = glGetUniformLocation(this->programId, this->uniforms.currentTime.name.c_str());
	this->uniforms.screenSize.location = glGetUniformLocation(this->programId, this->uniforms.screenSize.name.c_str());
	this->uniforms.isVisible.location = glGetUniformLocation(this->programId, this->uniforms.isVisible.name.c_str());
}

void Shader::updateUniformValues(const Transform model)
{
	this->uniforms.projectionMatrix.value = Camera::I()->makeProjectionMatrix();
	this->uniforms.modelMatrix.value = model.toMatrix();
	this->uniforms.viewMatrix.value = Camera::I()->makeViewMatrix();
	this->uniforms.viewPosition.value = Camera::I()->getPosition();
	this->uniforms.currentTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.screenSize.value = Window::I()->getSize();
	this->uniforms.isVisible.value = true;
}

void Shader::passUniforms()
{
	glUniformMatrix4fv(this->uniforms.projectionMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.projectionMatrix.value));
	glUniformMatrix4fv(this->uniforms.modelMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.modelMatrix.value));
	glUniformMatrix4fv(this->uniforms.viewMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.viewMatrix.value));
	glUniform3fv(this->uniforms.viewPosition.location, 1, value_ptr(this->uniforms.viewPosition.value));

	glUniform1f(this->uniforms.creationTime.location, this->uniforms.creationTime.value);
	glUniform1f(this->uniforms.currentTime.location, this->uniforms.currentTime.value);
	glUniform2f(
		this->uniforms.screenSize.location,
		static_cast<GLfloat>(this->uniforms.screenSize.value.x),
		static_cast<GLfloat>(this->uniforms.screenSize.value.y));
	glUniform1i(this->uniforms.isVisible.location, this->uniforms.isVisible.value ? 1 : 0);
}

void Shader::draw(const BufferValues values) const
{
	// Bind the Vertex Array Object (VAO) of the shape, which contains the vertex data to be drawn
	glBindVertexArray(this->addresses.vao);

	// Draw the vertices of the shape as specified by renderMode, starting from the first vertex (0), for vertexCount vertices in total
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(values.indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Shader::checkGLErrors()
{
	const unsigned int error = glGetError();
	if (error != GL_NO_ERROR)
	{
		cerr << "OpenGL Error: " << error << endl;
		throw runtime_error("OpenGL encountered an error.");
	}
}
