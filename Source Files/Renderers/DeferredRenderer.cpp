#include "../../Header Files/Renderers/DeferredRenderer.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/Buffers.h"
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
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::render(const Transform modelTransform, const Transform meshTransform, const ForwardBufferValues values, const Material material, const optional<shared_ptr<Texture>> texture)
{
}

void DeferredRenderer::initGBuffer()
{
	ivec2 screenSize = Window::I()->getSize();

	glGenFramebuffers(1, &this->addresses.gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->addresses.gBuffer);

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
	glGenTextures(1, &this->addresses.albedosSpeculars);
	glBindTexture(GL_TEXTURE_2D, this->addresses.albedosSpeculars);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.x, screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->addresses.albedosSpeculars, 0);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
