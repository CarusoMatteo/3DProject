#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/ShaderBuilder.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
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
}

void Shader::render()
{
	// TODO
}

void Shader::initVao()
{
	glGenVertexArrays(1, &this->buffers.vao);
	glBindVertexArray(this->buffers.vao);
}

void Shader::initVbos(const BufferValues values)
{
	// Generates and makes active the VBO for the vertices
	glGenBuffers(1, &this->buffers.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vertices);

	glBufferData(GL_ARRAY_BUFFER, values.vertices.size() * sizeof(fvec3), values.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// Generates and makes active the VBO for the colors
	glGenBuffers(1, &this->buffers.colors);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers.colors);
	glBufferData(GL_ARRAY_BUFFER, values.colors.size() * sizeof(fvec4), values.colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);
}

void Shader::initUniformReferences()
{
	this->uniforms.projectionMatrix.location = glGetUniformLocation(this->programId, this->uniforms.projectionMatrix.name.c_str());
	this->uniforms.modelMatrix.location = glGetUniformLocation(this->programId, this->uniforms.modelMatrix.name.c_str());
	this->uniforms.creationTime.location = glGetUniformLocation(this->programId, this->uniforms.creationTime.name.c_str());
	this->uniforms.currentTime.location = glGetUniformLocation(this->programId, this->uniforms.currentTime.name.c_str());
	this->uniforms.screenSize.location = glGetUniformLocation(this->programId, this->uniforms.screenSize.name.c_str());
	this->uniforms.isVisible.location = glGetUniformLocation(this->programId, this->uniforms.isVisible.name.c_str());
}
