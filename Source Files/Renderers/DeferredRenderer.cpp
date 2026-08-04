#include "../../Header Files/Renderers/DeferredRenderer.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/Buffers.h"
#include "../../Header Files/Renderers/ShaderBuilder.h"
#include "../../Header Files/Renderers/ShaderFiles.h"
#include "../../Header Files/Texture/Texture.h"
#include "../../Header Files/Window.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>

using namespace std;

DeferredRenderer::DeferredRenderer(const ShaderFiles files)
{
	this->programId = ShaderBuilder::buildShader(files);
	this->initGBuffer();
	this->initUniformReferences();

	this->uniforms.creationTime.value = static_cast<float>(glfwGetTime());
	this->uniforms.isVisible.value = true;
}

DeferredRenderer::~DeferredRenderer()
{
	glDeleteProgram(this->programId);
	glDeleteBuffers(1, &this->addresses.positions);
	glDeleteBuffers(1, &this->addresses.albedosSpecular);
	glDeleteBuffers(1, &this->addresses.normals);
	glDeleteBuffers(1, &this->addresses.depths);
	glDeleteFramebuffers(1, &this->addresses.gBuffer);
	// TODO: DeleteFramebuffers is the correct way to delete gBuffer?
}

void DeferredRenderer::setBufferValues(const DeferredBufferValues bufferValues)
{
	this->values = bufferValues;
	this->initSubBuffers();
	this->checkGLErrors();
}

void DeferredRenderer::render(const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
{
}

void DeferredRenderer::initGBuffer()
{
	glGenFramebuffers(1, &this->addresses.gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->addresses.gBuffer);
}

void DeferredRenderer::initSubBuffers()
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

	// TODO: Probably move this to render function if it needs to be called every frame?
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::initDepthRenderBuffer()
{
	ivec2 screenSize = Window::I()->getSize();

	glGenRenderbuffers(1, &this->addresses.depths);
	glBindRenderbuffer(GL_RENDERBUFFER, this->addresses.depths);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->addresses.depths);
}

void DeferredRenderer::initUniformReferences()
{
	// this->uniforms.projectionMatrix.location = glGetUniformLocation(this->programId, this->uniforms.projectionMatrix.name.c_str());
	// this->uniforms.modelMatrix.location = glGetUniformLocation(this->programId, this->uniforms.modelMatrix.name.c_str());
	// this->uniforms.viewMatrix.location = glGetUniformLocation(this->programId, this->uniforms.viewMatrix.name.c_str());
	// this->uniforms.viewPosition.location = glGetUniformLocation(this->programId, this->uniforms.viewPosition.name.c_str());

	// this->uniforms.lightPosition.location = glGetUniformLocation(this->programId, this->uniforms.lightPosition.name.c_str());
	// this->uniforms.lightColor.location = glGetUniformLocation(this->programId, this->uniforms.lightColor.name.c_str());
	// this->uniforms.lightPower.location = glGetUniformLocation(this->programId, this->uniforms.lightPower.name.c_str());

	// this->uniforms.materialAmbient.location = glGetUniformLocation(this->programId, this->uniforms.materialAmbient.name.c_str());
	// this->uniforms.materialDiffuse.location = glGetUniformLocation(this->programId, this->uniforms.materialDiffuse.name.c_str());
	// this->uniforms.materialSpecular.location = glGetUniformLocation(this->programId, this->uniforms.materialSpecular.name.c_str());
	// this->uniforms.materialShininess.location = glGetUniformLocation(this->programId, this->uniforms.materialShininess.name.c_str());

	// this->uniforms.creationTime.location = glGetUniformLocation(this->programId, this->uniforms.creationTime.name.c_str());
	// this->uniforms.currentTime.location = glGetUniformLocation(this->programId, this->uniforms.currentTime.name.c_str());
	// this->uniforms.screenSize.location = glGetUniformLocation(this->programId, this->uniforms.screenSize.name.c_str());
	// this->uniforms.isVisible.location = glGetUniformLocation(this->programId, this->uniforms.isVisible.name.c_str());

	// this->uniforms.texture.location = glGetUniformLocation(this->programId, this->uniforms.texture.name.c_str());
	// this->uniforms.useTexture.location = glGetUniformLocation(this->programId, this->uniforms.useTexture.name.c_str());

	// this->uniforms.skybox.location = glGetUniformLocation(this->programId, this->uniforms.skybox.name.c_str());
}