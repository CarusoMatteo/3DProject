#include "../../Header Files/Renderers/GeometryRenderer.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/BuffersAddresses.h"
#include "../../Header Files/Renderers/BuffersValues.h"
#include "../../Header Files/Renderers/ShaderBuilder.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include "../../Header Files/Texture/Texture.h"
#include "../../Header Files/Window.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>

using namespace std;

GeometryRenderer::GeometryRenderer(const ShaderFiles files)
{
	this->programId = ShaderBuilder::buildShader(files);
	this->initGBuffer();
	this->initUniformReferences();

	this->uniforms.creationTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.isVisible.value = true;
}

GeometryRenderer::~GeometryRenderer()
{
	glDeleteProgram(this->programId);
	glDeleteBuffers(1, &this->addresses.positions);
	glDeleteBuffers(1, &this->addresses.albedosSpecular);
	glDeleteBuffers(1, &this->addresses.normals);
	glDeleteBuffers(1, &this->addresses.depths);
	glDeleteFramebuffers(1, &this->addresses.gBuffer);
	// TODO: DeleteFramebuffers is the correct way to delete gBuffer?
}

void GeometryRenderer::setBufferValues(const GeometryBufferValues bufferValues)
{
	this->values = bufferValues;
	this->initSubBuffers();
	this->checkGLErrors();
}

void GeometryRenderer::render(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
{
	// Geometry pass: render scene's geometry/color data into g-buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->addresses.gBuffer);

	glUseProgram(this->programId);
	this->updateUniformValues(modelTransform, meshTransform, material, texture);
	this->checkGLErrors();

	this->passUniforms();
	this->checkGLErrors();

	this->draw();
	this->checkGLErrors();

	// Unbind the framebuffer to render to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeometryRenderer::initGBuffer()
{
	glGenFramebuffers(1, &this->addresses.gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->addresses.gBuffer);
}

void GeometryRenderer::initSubBuffers()
{
	ivec2 screenSize = Window::I()->getSize();

	// Position color buffer
	glGenTextures(1, &this->addresses.positions);
	glBindTexture(GL_TEXTURE_2D, this->addresses.positions);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->addresses.positions, 0);

	// Normal color buffer
	glGenTextures(1, &this->addresses.normals);
	glBindTexture(GL_TEXTURE_2D, this->addresses.normals);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenSize.x, screenSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->addresses.normals, 0);

	// Color + specular color buffer
	glGenTextures(1, &this->addresses.albedosSpecular);
	glBindTexture(GL_TEXTURE_2D, this->addresses.albedosSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.x, screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->addresses.albedosSpecular, 0);

	// Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);

	// Depth buffer
	glGenRenderbuffers(1, &this->addresses.depths);
	glBindRenderbuffer(GL_RENDERBUFFER, this->addresses.depths);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->addresses.depths);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Framebuffer not complete!" << endl;
		throw runtime_error("Framebuffer not complete!");
	}
}

void GeometryRenderer::initDepthRenderBuffer()
{
	ivec2 screenSize = Window::I()->getSize();

	glGenRenderbuffers(1, &this->addresses.depths);
	glBindRenderbuffer(GL_RENDERBUFFER, this->addresses.depths);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->addresses.depths);
}

void GeometryRenderer::initUniformReferences()
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

void GeometryRenderer::updateUniformValues(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
{
	this->uniforms.projectionMatrix.value = Camera::I()->makeProjectionMatrix();
	this->uniforms.modelMatrix.value = modelTransform.toMatrix() * meshTransform.toMatrix();
	this->uniforms.viewMatrix.value = Camera::I()->makeViewMatrix();
	this->uniforms.viewPosition.value = Camera::I()->getPosition();

	this->uniforms.currentTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.screenSize.value = Window::I()->getSize();
}

void GeometryRenderer::passUniforms()
{
	glUniformMatrix4fv(this->uniforms.projectionMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.projectionMatrix.value));
	glUniformMatrix4fv(this->uniforms.modelMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.modelMatrix.value));
	glUniformMatrix4fv(this->uniforms.viewMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.viewMatrix.value));
	glUniform3fv(this->uniforms.viewPosition.location, 1, value_ptr(this->uniforms.viewPosition.value));

	glUniform1f(this->uniforms.creationTime.location, this->uniforms.creationTime.value);
	glUniform1f(this->uniforms.currentTime.location, this->uniforms.currentTime.value);
	glUniform2iv(this->uniforms.screenSize.location, 1, value_ptr(this->uniforms.screenSize.value));
	glUniform1i(this->uniforms.isVisible.location, this->uniforms.isVisible.value ? 1 : 0);
}

void GeometryRenderer::draw() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->addresses.positions);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->addresses.normals);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->addresses.albedosSpecular);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->addresses.gBuffer);
	// write to default framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, this->uniforms.screenSize.value.x, this->uniforms.screenSize.value.y, 0, 0, this->uniforms.screenSize.value.x, this->uniforms.screenSize.value.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
