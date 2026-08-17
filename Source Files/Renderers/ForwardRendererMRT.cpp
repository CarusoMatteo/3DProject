#include "../../Header Files/Renderers/ForwardRendererMRT.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/BuffersAddresses.h"
#include "../../Header Files/Renderers/BuffersValues.h"
#include "../../Header Files/Renderers/ShaderBuilder.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include "../../Header Files/Texture/Texture.h"
#include "../../Header Files/Texture/TextureLoader.h"
#include "../../Header Files/Window.h"
#include <GLFW/glfw3.h>
#include <future>
#include <gl/GL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;
using namespace glm;

ForwardRendererMRT::ForwardRendererMRT(const ShaderFiles files)
{
	this->programId = ShaderBuilder::buildShader(files);
	this->initVao();
	this->initUniformReferences();

	this->uniforms.creationTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.isVisible.value = true;

	ivec2 size = Window::I()->getSize();

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Texture 0: The one that will be shown on screen
	glGenTextures(1, &texColor);
	glBindTexture(GL_TEXTURE_2D, texColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);

	// Texture 1: The one that will be saved to file.
	glGenTextures(1, &texFileColor);
	glBindTexture(GL_TEXTURE_2D, texFileColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texFileColor, 0);

	// Texture 2: Normal texture that will be saved to file.
	glGenTextures(1, &texNormal);
	glBindTexture(GL_TEXTURE_2D, texNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, texNormal, 0);
	
	// Texture 3: Depth texture that will be saved to file.
	glGenTextures(1, &texDepth);
	glBindTexture(GL_TEXTURE_2D, texDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, texDepth, 0);

	// Depth (necessary because of the depth test)
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GLenum drawBuffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cerr << "Warning: Incomplete FBO!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ForwardRendererMRT::~ForwardRendererMRT()
{
	glDeleteProgram(this->programId);
	glDeleteBuffers(1, &this->addresses.vertices);
	glDeleteBuffers(1, &this->addresses.colors);
	glDeleteBuffers(1, &this->addresses.normals);
	glDeleteBuffers(1, &this->addresses.indices);
	glDeleteBuffers(1, &this->addresses.textureCoordinates);
	glDeleteVertexArrays(1, &this->addresses.vao);
}

void ForwardRendererMRT::setBufferValues(const ForwardBufferValues bufferValues)
{
	this->values = bufferValues;
	this->initVbos();
	this->checkGLErrors();
}

void ForwardRendererMRT::render(const float currentTime, const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
{
	ivec2 size = Window::I()->getSize();

	// 1. Render the scene in the FBO (with MRT enabled)
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	// 2. Copy (blit) the main buffer on the screen
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default framebuffer
	glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// 3. Read the pixels of the texture to save
	saveBuffers(currentTime, size);
}

void ForwardRendererMRT::initVao()
{
	glGenVertexArrays(1, &this->addresses.vao);
	glBindVertexArray(this->addresses.vao);
}

void ForwardRendererMRT::initVbos()
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

void ForwardRendererMRT::initUniformReferences()
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

void ForwardRendererMRT::updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
{
	this->uniforms.projectionMatrix.value = Camera::I()->makeProjectionMatrix();
	this->uniforms.modelMatrix.value = modelTransform.toMatrix() * meshTransform.toMatrix();
	this->uniforms.viewMatrix.value = Camera::I()->makeViewMatrix();
	this->uniforms.viewPosition.value = Camera::I()->getPosition();

	// Use only first light in forward rendering
	const LightValue value = LightManager::I()->getValues()[0];
	const float power = LightManager::I()->getPowers()[0];

	this->uniforms.lightPosition.value = value.position;
	this->uniforms.lightColor.value = value.color;
	this->uniforms.lightPower.value = power;

	this->uniforms.materialAmbient.value = material.ambient;
	this->uniforms.materialDiffuse.value = material.diffuse;
	this->uniforms.materialSpecular.value = material.specular;
	this->uniforms.materialShininess.value = material.shininess;

	this->uniforms.currentTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.screenSize.value = Window::I()->getSize();

	this->uniforms.useTexture.value = texture.has_value();
}

void ForwardRendererMRT::passUniforms()
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

void ForwardRendererMRT::bindTexture(const Texture texture) const
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

void ForwardRendererMRT::bindNoTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ForwardRendererMRT::draw() const
{
	if (*ForwardRendererMRT::drawWireframe)
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

	if (*ForwardRendererMRT::drawAnchor)
	{
		glPointSize(15.0f);
		const unsigned int anchorIndex = this->values.indices.back();
		glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(anchorIndex * sizeof(unsigned int)));
	}

	glBindVertexArray(0);
}

void ForwardRendererMRT::saveBuffers(const float currentTime, const ivec2 size)
{
	if (InputEvents::shouldTakeScreenshotNextFrame())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		string filename;
		vector<float> pixelsFloat(size.x * size.y * 4);

		glReadBuffer(GL_COLOR_ATTACHMENT1);
		filename = ("img/" + to_string(currentTime) + "_main.bmp");
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		saveTexture(size, pixelsFloat, filename);

		glReadBuffer(GL_COLOR_ATTACHMENT2);
		filename = ("img/" + to_string(currentTime) + "_normal.bmp");
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		saveTexture(size, pixelsFloat, filename);

		glReadBuffer(GL_COLOR_ATTACHMENT3);
		filename = ("img/" + to_string(currentTime) + "_depth.bmp");
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		saveTexture(size, pixelsFloat, filename);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
