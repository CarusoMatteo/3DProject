#include "../../Header Files/Renderers/ForwardRenderer.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/Buffers.h"
#include "../../Header Files/Renderers/ShaderBuilder.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include "../../Header Files/Texture/Texture.h"
#include "../../Header Files/Window.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;
using namespace glm;

ForwardRenderer::ForwardRenderer(const ShaderFiles files)
{
	this->programId = ShaderBuilder::buildShader(files);
	this->initVao();
	this->initUniformReferences();

	this->uniforms.creationTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.isVisible.value = true;
}

ForwardRenderer::~ForwardRenderer()
{
	glDeleteProgram(this->programId);
	glDeleteBuffers(1, &this->addresses.vertices);
	glDeleteBuffers(1, &this->addresses.colors);
	glDeleteBuffers(1, &this->addresses.normals);
	glDeleteBuffers(1, &this->addresses.indices);
	glDeleteBuffers(1, &this->addresses.textureCoordinates);
	glDeleteVertexArrays(1, &this->addresses.vao);
}

void ForwardRenderer::setBufferValues(const ForwardBufferValues bufferValues)
{
	this->values = bufferValues;
	this->initVbos();
	this->checkGLErrors();
}

void ForwardRenderer::render(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
{
	// Disable writing to depth buffer
	if (texture.has_value() && texture.value()->isCubemap)
		glDepthMask(false);

	glUseProgram(this->programId);
	this->updateUniformValues(modelTransform, meshTransform, material, texture);
	this->checkGLErrors();

	this->passUniforms();
	this->checkGLErrors();

#pragma warning(suppress : 26859)
	if (this->uniforms.useTexture.value)
		this->bindTexture(*texture.value());
	else
		this->bindNoTexture();
	this->checkGLErrors();

	this->draw();
	this->checkGLErrors();

	// Re-enable writing to depth buffer
	if (texture.has_value() && texture.value()->isCubemap)
		glDepthMask(true);
}

void ForwardRenderer::initVao()
{
	glGenVertexArrays(1, &this->addresses.vao);
	glBindVertexArray(this->addresses.vao);
}

void ForwardRenderer::initVbos()
{
	// Generates and makes active the VBO for the vertices
	glGenBuffers(1, &this->addresses.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.vertices);
	glBufferData(GL_ARRAY_BUFFER, this->values.vertices.size() * sizeof(fvec3), this->values.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// Generates and makes active the VBO for the colors
	glGenBuffers(1, &this->addresses.colors);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.colors);
	glBufferData(GL_ARRAY_BUFFER, this->values.colors.size() * sizeof(fvec4), this->values.colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);

	// Generates and makes active the VBO for the normals
	glGenBuffers(1, &this->addresses.normals);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.normals);
	glBufferData(GL_ARRAY_BUFFER, this->values.normals.size() * sizeof(fvec3), this->values.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(2);

	// Generates and makes active the VBO for the texture coordinates
	glGenBuffers(1, &this->addresses.textureCoordinates);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.textureCoordinates);
	glBufferData(GL_ARRAY_BUFFER, this->values.textureCoordinates.size() * sizeof(fvec2), this->values.textureCoordinates.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(3);

	// Generates and makes active the EBO for the indices
	glGenBuffers(1, &this->addresses.indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->addresses.indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->values.indices.size() * sizeof(unsigned int), this->values.indices.data(), GL_STATIC_DRAW);
}

void ForwardRenderer::initUniformReferences()
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

	this->uniforms.texture.location = glGetUniformLocation(this->programId, this->uniforms.texture.name.c_str());
	this->uniforms.useTexture.location = glGetUniformLocation(this->programId, this->uniforms.useTexture.name.c_str());

	this->uniforms.skybox.location = glGetUniformLocation(this->programId, this->uniforms.skybox.name.c_str());
}

void ForwardRenderer::updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
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

	this->uniforms.useTexture.value = texture.has_value();
}

void ForwardRenderer::passUniforms()
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

	glUniform1i(this->uniforms.useTexture.location, this->uniforms.useTexture.value ? 1 : 0);
}

void ForwardRenderer::bindTexture(const Texture texture) const
{
	glActiveTexture(GL_TEXTURE0);
	if (texture.isCubemap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);
		glUniform1i(this->uniforms.skybox.location, 0); // samplerCube -> texture unit 0
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture.id);
		// The shader must read the texture from texture unit 0.
		glUniform1i(this->uniforms.texture.location, 0); // sampler2D -> texture unit 0
	}
}

void ForwardRenderer::bindNoTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ForwardRenderer::draw() const
{
	if (*ForwardRenderer::drawWireframe)
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
	// If this object uses the index buffer, use glDrawElements instead of glDrawArrays
	if (this->bufferIsUsed(this->addresses.indices, this->values.indices.size()))
		glDrawElements(GL_TRIANGLES, static_cast<int>(this->values.indices.size() - 1), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (*ForwardRenderer::drawAnchor)
	{
		glPointSize(15.0f);
		const unsigned int anchorIndex = this->values.indices.back();
		glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(anchorIndex * sizeof(unsigned int)));
	}

	glBindVertexArray(0);
}
