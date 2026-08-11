#include "../../Header Files/Renderers/GeometryRenderer.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Material.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/BuffersAddresses.h"
#include "../../Header Files/Renderers/BuffersValues.h"
#include "../../Header Files/Renderers/ForwardRenderer.h"
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
#include <string>

using namespace std;

GeometryRenderer::GeometryRenderer(const ShaderFiles geometryPassFiles, const ShaderFiles lightingPassFiles)
{
	this->geometryPassProgramId = ShaderBuilder::buildShader(geometryPassFiles);
	this->lightingPassProgramId = ShaderBuilder::buildShader(lightingPassFiles);

	this->initGBuffer();
	this->initUniformReferences();
	this->initVao();
	this->initQuad();
}

GeometryRenderer::~GeometryRenderer()
{
	glDeleteProgram(this->geometryPassProgramId);
	glDeleteProgram(this->lightingPassProgramId);

	glDeleteBuffers(1, &this->addresses.positions);
	glDeleteBuffers(1, &this->addresses.albedosSpecular);
	glDeleteBuffers(1, &this->addresses.normals);
	glDeleteBuffers(1, &this->addresses.depths);
	glDeleteFramebuffers(1, &this->addresses.gBuffer);
	// TODO: DeleteFramebuffers is the correct way to delete gBuffer?

	glDeleteBuffers(1, &this->forwardAddresses.vertices);
	glDeleteBuffers(1, &this->forwardAddresses.colors);
	glDeleteBuffers(1, &this->forwardAddresses.normals);
	glDeleteBuffers(1, &this->forwardAddresses.indices);
	glDeleteBuffers(1, &this->forwardAddresses.textureCoordinates);
	glDeleteVertexArrays(1, &this->forwardAddresses.vao);

	glDeleteBuffers(1, &this->quadAddresses.vbo);
	glDeleteVertexArrays(1, &this->quadAddresses.vao);
}

void GeometryRenderer::setBufferValues(const GeometryBufferValues bufferValues, const ForwardBufferValues forwardBufferValues)
{
	this->values = bufferValues;
	this->initFrameBuffer();

	this->forwardValues = forwardBufferValues;
	this->initVbos();

	this->checkGLErrors();
}

void GeometryRenderer::render(const float currentTime, const Transform modelTransform, const Transform meshTransform, const Material material, const optional<shared_ptr<Texture>> texture)
{
	this->geometryPass(currentTime, modelTransform, meshTransform);
	this->draw();
	this->checkGLErrors();
}

void GeometryRenderer::finishGeometryPass()
{
	// Reset to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeometryRenderer::lightingPass() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(this->lightingPassProgramId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->addresses.positions);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->addresses.normals);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->addresses.albedosSpecular);

	unsigned int i = 0;
	for (const LightValue &lightValue : LightManager::I()->getValues())
	{
		fvec3 lightPosition = lightValue.position;
		fvec3 lightColor = lightValue.color;
		float linear = lightValue.linear;
		float quadratic = lightValue.quadratic;

		glUniform3fv(glGetUniformLocation(this->lightingPassProgramId, (this->uniforms.lightsStructName + "[" + std::to_string(i) + "]." + this->uniforms.lightPositionName).c_str()), 1, &lightPosition[0]);
		glUniform3fv(glGetUniformLocation(this->lightingPassProgramId, (this->uniforms.lightsStructName + "[" + std::to_string(i) + "]." + this->uniforms.lightColorName).c_str()), 1, &lightColor[0]);

		glUniform1f(glGetUniformLocation(this->lightingPassProgramId, (this->uniforms.lightsStructName + "[" + std::to_string(i) + "]." + this->uniforms.lightLinearName).c_str()), linear);
		glUniform1f(glGetUniformLocation(this->lightingPassProgramId, (this->uniforms.lightsStructName + "[" + std::to_string(i) + "]." + this->uniforms.lightQuadraticName).c_str()), quadratic);

		i++;
	}

	glBindVertexArray(this->quadAddresses.vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->addresses.gBuffer);
	// write to default framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	const ivec2 screenSize = Window::I()->getSize();

	glBlitFramebuffer(0, 0, screenSize.x, screenSize.y, 0, 0, screenSize.x, screenSize.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GeometryRenderer::initGBuffer()
{
	glGenFramebuffers(1, &this->addresses.gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->addresses.gBuffer);
}

void GeometryRenderer::initUniformReferences()
{
	this->uniforms.projectionMatrix.location = glGetUniformLocation(this->geometryPassProgramId, this->uniforms.projectionMatrix.name.c_str());
	this->uniforms.modelMatrix.location = glGetUniformLocation(this->geometryPassProgramId, this->uniforms.modelMatrix.name.c_str());
	this->uniforms.viewMatrix.location = glGetUniformLocation(this->geometryPassProgramId, this->uniforms.viewMatrix.name.c_str());

	this->uniforms.positions.location = glGetUniformLocation(this->lightingPassProgramId, this->uniforms.positions.name.c_str());
	this->uniforms.normals.location = glGetUniformLocation(this->lightingPassProgramId, this->uniforms.normals.name.c_str());
	this->uniforms.albedosSpecular.location = glGetUniformLocation(this->lightingPassProgramId, this->uniforms.albedosSpecular.name.c_str());

	this->uniforms.positions.value = 0;
	this->uniforms.normals.value = 1;
	this->uniforms.albedosSpecular.value = 2;

	// This uniform values don't change every frame, so we set them once here instead of every frame in passUniforms().
	glUseProgram(this->lightingPassProgramId);
	glUniform1i(this->uniforms.positions.location, this->uniforms.positions.value);
	glUniform1i(this->uniforms.normals.location, this->uniforms.normals.value);
	glUniform1i(this->uniforms.albedosSpecular.location, this->uniforms.albedosSpecular.value);
}

void GeometryRenderer::initVao()
{
	glGenVertexArrays(1, &this->forwardAddresses.vao);
	glBindVertexArray(this->forwardAddresses.vao);
}

void GeometryRenderer::initVbos()
{
	// Generates and makes active the VBO for the vertices
	glGenBuffers(1, &this->forwardAddresses.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->forwardAddresses.vertices);
	glBufferData(GL_ARRAY_BUFFER, this->forwardValues.vertices.size() * sizeof(fvec3), this->forwardValues.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// Generates and makes active the VBO for the normals
	glGenBuffers(1, &this->forwardAddresses.normals);
	glBindBuffer(GL_ARRAY_BUFFER, this->forwardAddresses.normals);
	glBufferData(GL_ARRAY_BUFFER, this->forwardValues.normals.size() * sizeof(fvec3), this->forwardValues.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);

	// Generates and makes active the VBO for the texture coordinates
	glGenBuffers(1, &this->forwardAddresses.textureCoordinates);
	glBindBuffer(GL_ARRAY_BUFFER, this->forwardAddresses.textureCoordinates);
	glBufferData(GL_ARRAY_BUFFER, this->forwardValues.textureCoordinates.size() * sizeof(fvec2), this->forwardValues.textureCoordinates.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(2);

	// Generates and makes active the EBO for the indices
	glGenBuffers(1, &this->forwardAddresses.indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->forwardAddresses.indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->forwardValues.indices.size() * sizeof(unsigned int), this->forwardValues.indices.data(), GL_STATIC_DRAW);
}

void GeometryRenderer::initQuad()
{
	// 3 for positions then 2 for texture Coords
	this->quadValues.values = {
		-1, 1, 0, 0, 1,
		-1, -1, 0, 0, 0,
		1, -1, 0, 1, 0,
		1, 1, 0, 1, 1};
	// setup plane VAO
	glGenVertexArrays(1, &this->quadAddresses.vao);
	glGenBuffers(1, &this->quadAddresses.vbo);
	glBindVertexArray(this->quadAddresses.vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadAddresses.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->quadValues.values), this->quadValues.values.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
}

void GeometryRenderer::initFrameBuffer()
{
	const ivec2 screenSize = Window::I()->getSize();

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

// After this function, draw every mesh that uses deferred rendering. After that, call finishGeometryPass() to reset the framebuffer to default.
void GeometryRenderer::geometryPass(const float currentTime, const Transform modelTransform, const Transform meshTransform)
{
	cout << "Starting geometry pass at " << currentTime << ".\t ";

	// Only once per frame
	if (this->lastFrameTime.has_value() && this->lastFrameTime.value() == currentTime)
	{
		cout << "Geometry pass already executed for this frame. Skipping." << endl;
		return;
	}
	cout << "Executing geometry pass." << endl;
	this->lastFrameTime = currentTime;

	// Geometry pass: render scene's geometry/color data into g-buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->addresses.gBuffer);

	glUseProgram(this->geometryPassProgramId);
	this->updateUniformValues(modelTransform, meshTransform);
	this->checkGLErrors();

	this->passUniforms();
	this->checkGLErrors();
}

void GeometryRenderer::updateUniformValues(const Transform modelTransform, const Transform meshTransform)
{
	this->uniforms.projectionMatrix.value = Camera::I()->makeProjectionMatrix();
	this->uniforms.modelMatrix.value = modelTransform.toMatrix() * meshTransform.toMatrix();
	this->uniforms.viewMatrix.value = Camera::I()->makeViewMatrix();
}

void GeometryRenderer::passUniforms()
{
	glUniformMatrix4fv(this->uniforms.projectionMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.projectionMatrix.value));
	glUniformMatrix4fv(this->uniforms.modelMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.modelMatrix.value));
	glUniformMatrix4fv(this->uniforms.viewMatrix.location, 1, GL_FALSE, value_ptr(this->uniforms.viewMatrix.value));
}

void GeometryRenderer::draw() const
{
	if (*ForwardRenderer::drawWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(this->forwardAddresses.vao);

	if (this->bufferIsUsed(this->forwardAddresses.indices, this->forwardValues.indices.size()))
		glDrawElements(GL_TRIANGLES, static_cast<int>(this->forwardValues.indices.size() - 1), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}
