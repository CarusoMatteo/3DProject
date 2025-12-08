#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Game Objects/PointLight.h"
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
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;
using namespace glm;

#pragma region Shader

shared_ptr<bool> Shader::drawWireframe = make_shared<bool>(false);
shared_ptr<bool> Shader::drawAnchor = make_shared<bool>(false);

shared_ptr<bool> Shader::getDrawWireframeFlag()
{
	return Shader::drawWireframe;
}

shared_ptr<bool> Shader::getDrawAnchorFlag()
{
	return Shader::drawAnchor;
}

Shader::Shader(const ShaderFiles files)
{
	this->programId = ShaderBuilder::buildShader(files);
	this->initVao();
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

void Shader::setBufferValues(const BufferValues bufferValues)
{
	this->initVbos(bufferValues);
	this->checkGLErrors();
}

void Shader::render(const Transform modelTransform, const Transform meshTransform, const BufferValues values, const Material material)
{
	glUseProgram(this->programId);
	this->updateUniformValues(modelTransform, meshTransform, material);
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
	// glGenBuffers(1, &this->addresses.textures);
	// glBindBuffer(GL_ARRAY_BUFFER, this->addresses.textures);
	// glBufferData(GL_ARRAY_BUFFER, values.textures.size() * sizeof(fvec2), values.textures.data(), GL_STATIC_DRAW);
	// glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	// glEnableVertexAttribArray(3);

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

	this->uniforms.lightPosition.location = glGetUniformLocation(this->programId, this->uniforms.lightPosition.name.c_str());
	this->uniforms.lightColor.location = glGetUniformLocation(this->programId, this->uniforms.lightColor.name.c_str());
	this->uniforms.lightPower.location = glGetUniformLocation(this->programId, this->uniforms.lightPower.name.c_str());

	this->uniforms.materialAmbient.location = glGetUniformLocation(this->programId, this->uniforms.materialAmbient.name.c_str());
	this->uniforms.materialDiffuse.location = glGetUniformLocation(this->programId, this->uniforms.materialDiffuse.name.c_str());
	this->uniforms.materialSpecular.location = glGetUniformLocation(this->programId, this->uniforms.materialSpecular.name.c_str());
	this->uniforms.materialShininess.location = glGetUniformLocation(this->programId, this->uniforms.materialShininess.name.c_str());

	this->uniforms.creationTime.location = glGetUniformLocation(this->programId, this->uniforms.creationTime.name.c_str());
	this->uniforms.currentTime.location = glGetUniformLocation(this->programId, this->uniforms.currentTime.name.c_str());
	this->uniforms.screenSize.location = glGetUniformLocation(this->programId, this->uniforms.screenSize.name.c_str());
	this->uniforms.isVisible.location = glGetUniformLocation(this->programId, this->uniforms.isVisible.name.c_str());
}

void Shader::updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material)
{
	this->uniforms.projectionMatrix.value = Camera::I()->makeProjectionMatrix();
	this->uniforms.modelMatrix.value = modelTransform.toMatrix() * meshTransform.toMatrix();
	this->uniforms.viewMatrix.value = Camera::I()->makeViewMatrix();
	this->uniforms.viewPosition.value = Camera::I()->getPosition();

	this->uniforms.lightPosition.value = PointLight::I()->getPosition();
	this->uniforms.lightColor.value = PointLight::I()->getColor();
	this->uniforms.lightPower.value = PointLight::I()->getPower();

	this->uniforms.materialAmbient.value = material.ambient;
	this->uniforms.materialDiffuse.value = material.diffuse;
	this->uniforms.materialSpecular.value = material.specular;
	this->uniforms.materialShininess.value = material.shininess;

	this->uniforms.currentTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.screenSize.value = Window::I()->getSize();
}

void Shader::passUniforms()
{
	glUniformMatrix4fv(this->uniforms.projectionMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.projectionMatrix.value));
	glUniformMatrix4fv(this->uniforms.modelMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.modelMatrix.value));
	glUniformMatrix4fv(this->uniforms.viewMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.viewMatrix.value));
	glUniform3fv(this->uniforms.viewPosition.location, 1, value_ptr(this->uniforms.viewPosition.value));

	glUniform3fv(this->uniforms.lightPosition.location, 1, value_ptr(this->uniforms.lightPosition.value));
	glUniform3fv(this->uniforms.lightColor.location, 1, value_ptr(this->uniforms.lightColor.value));
	glUniform1f(this->uniforms.lightPower.location, this->uniforms.lightPower.value);

	glUniform3fv(this->uniforms.materialAmbient.location, 1, value_ptr(this->uniforms.materialAmbient.value));
	glUniform3fv(this->uniforms.materialDiffuse.location, 1, value_ptr(this->uniforms.materialDiffuse.value));
	glUniform3fv(this->uniforms.materialSpecular.location, 1, value_ptr(this->uniforms.materialSpecular.value));
	glUniform1f(this->uniforms.materialShininess.location, this->uniforms.materialShininess.value);

	glUniform1f(this->uniforms.creationTime.location, this->uniforms.creationTime.value);
	glUniform1f(this->uniforms.currentTime.location, this->uniforms.currentTime.value);
	glUniform2iv(this->uniforms.screenSize.location, 1, value_ptr(this->uniforms.screenSize.value));
	glUniform1i(this->uniforms.isVisible.location, this->uniforms.isVisible.value ? 1 : 0);
}

void Shader::draw(const BufferValues values) const
{
	if (*Shader::drawWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Bind the Vertex Array Object (VAO) of the shape, which contains the vertex data to be drawn
	glBindVertexArray(this->addresses.vao);

	// Draw the vertices of the shape as specified by renderMode, starting from the first vertex (0), for vertexCount vertices in total
	glDrawElements(GL_TRIANGLES, static_cast<int>(values.indices.size() - 1), GL_UNSIGNED_INT, 0);

	if (*Shader::drawAnchor)
	{
		glPointSize(15.0f);
		const unsigned int anchorIndex = values.indices.back();
		glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(anchorIndex * sizeof(unsigned int)));
	}

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

#pragma endregion
#pragma region UnlitShader

UnlitShader::UnlitShader()
	: Shader({"Shaders/Unlit/Unlit.vert", "Shaders/Unlit/Unlit.frag"})
{
}

#pragma endregion
#pragma region PhongShader

PhongShader::PhongShader()
	: Shader({"Shaders/Phong/Phong.vert", "Shaders/Phong/Phong.frag"})
{
}

#pragma endregion
#pragma region BlinnPhongShader

BlinnPhongShader::BlinnPhongShader()
	: Shader({"Shaders/BlinnPhong/BlinnPhong.vert", "Shaders/BlinnPhong/BlinnPhong.frag"})
{
}

#pragma endregion
#pragma region ShaderFactory

shared_ptr<Shader> ShaderFactory::createUnlitShader()
{
	return shared_ptr<Shader>(new UnlitShader());
}

shared_ptr<Shader> ShaderFactory::createPhongShader()
{
	return shared_ptr<Shader>(new PhongShader());
}

shared_ptr<Shader> ShaderFactory::createBlinnPhongShader()
{
	return shared_ptr<Shader>(new BlinnPhongShader());
}

#pragma endregion
