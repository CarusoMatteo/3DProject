#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Texture.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/ShaderBuilder.h"
#include "../../Header Files/Window.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <glad/glad.h>
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

#pragma region Singleton instances of shaders

// Paths to the shader files

const string ShaderFactory::SHADER_PATH = "Shaders/";
const ShaderFiles ShaderFactory::UNLIT_PATH = {
	ShaderFactory::SHADER_PATH + "Unlit/Unlit.vert",
	ShaderFactory::SHADER_PATH + "Unlit/Unlit.frag"};
const ShaderFiles ShaderFactory::PHONG_PATH = {
	ShaderFactory::SHADER_PATH + "Phong/Phong.vert",
	ShaderFactory::SHADER_PATH + "Phong/Phong.frag"};
const ShaderFiles ShaderFactory::BLINN_PHONG_PATH = {
	ShaderFactory::SHADER_PATH + "BlinnPhong/Blinn.vert",
	ShaderFactory::SHADER_PATH + "BlinnPhong/BlinnPhong.frag"};
const ShaderFiles ShaderFactory::REFLECTION_PATH = {
	ShaderFactory::SHADER_PATH + "Reflection/Reflection.vert",
	ShaderFactory::SHADER_PATH + "Reflection/Reflection.frag"};
const ShaderFiles ShaderFactory::CUBE_MAP_PATH = {
	ShaderFactory::SHADER_PATH + "CubeMap/CubeMap.vert",
	ShaderFactory::SHADER_PATH + "CubeMap/CubeMap.frag"};
const ShaderFiles ShaderFactory::INTERPOLATIVE_PATH = {
	ShaderFactory::SHADER_PATH + "Interpolative/Interpolative.vert",
	ShaderFactory::SHADER_PATH + "Interpolative/Interpolative.frag"};

// Shader singletons

optional<shared_ptr<Shader>> ShaderFactory::unlitShader = nullopt;
optional<shared_ptr<Shader>> ShaderFactory::phongShader = nullopt;
optional<shared_ptr<Shader>> ShaderFactory::blinnPhongShader = nullopt;
optional<shared_ptr<Shader>> ShaderFactory::reflectionShader = nullopt;
optional<shared_ptr<Shader>> ShaderFactory::cubeMapShader = nullopt;
optional<shared_ptr<Shader>> ShaderFactory::interpolativeShader = nullopt;

shared_ptr<Shader> ShaderFactory::unlit()
{
	if (!unlitShader.has_value())
		unlitShader = make_shared<Shader>(UNLIT_PATH);
	return unlitShader.value();
}

shared_ptr<Shader> ShaderFactory::phong()
{
	if (!phongShader.has_value())
		phongShader = make_shared<Shader>(PHONG_PATH);
	return phongShader.value();
}

shared_ptr<Shader> ShaderFactory::blinnPhong()
{
	if (!blinnPhongShader.has_value())
		blinnPhongShader = make_shared<Shader>(BLINN_PHONG_PATH);
	return blinnPhongShader.value();
}

shared_ptr<Shader> ShaderFactory::reflection()
{
	if (!reflectionShader.has_value())
		reflectionShader = make_shared<Shader>(REFLECTION_PATH);
	return reflectionShader.value();
}

shared_ptr<Shader> ShaderFactory::cubeMap()
{
	if (!cubeMapShader.has_value())
		cubeMapShader = make_shared<Shader>(CUBE_MAP_PATH);
	return cubeMapShader.value();
}

shared_ptr<Shader> ShaderFactory::interpolative()
{
	if (!interpolativeShader.has_value())
		interpolativeShader = make_shared<Shader>(INTERPOLATIVE_PATH);
	return interpolativeShader.value();
}

#pragma endregion

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
	glDeleteBuffers(1, &this->addresses.textureCoordinates);
	glDeleteVertexArrays(1, &this->addresses.vao);
}

void Shader::setBufferValues(const BufferValues bufferValues)
{
	this->initVbos(bufferValues);
	this->checkGLErrors();
}

void Shader::render(const Transform modelTransform, const Transform meshTransform, const BufferValues values, const Material material, const optional<shared_ptr<Texture>> texture)
{
	glUseProgram(this->programId);
	this->updateUniformValues(modelTransform, meshTransform, material);
	this->checkGLErrors();

	this->passUniforms();
	this->checkGLErrors();

	this->uniforms.useTexture.value = texture.has_value();
	if (this->uniforms.useTexture.value)
		this->bindTexture(*texture.value());
	else
		this->bindNoTexture();
	this->checkGLErrors();

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
	glGenBuffers(1, &this->addresses.textureCoordinates);
	glBindBuffer(GL_ARRAY_BUFFER, this->addresses.textureCoordinates);
	glBufferData(GL_ARRAY_BUFFER, values.textureCoordinates.size() * sizeof(fvec2), values.textureCoordinates.data(), GL_STATIC_DRAW);
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

	// this->uniforms.skybox.location = glGetUniformLocation(this->programId, this->uniforms.skybox.name.c_str());
	// this->uniforms.cubeMap.location = glGetUniformLocation(this->programId, this->uniforms.cubeMap.name.c_str());
	this->uniforms.texture.location = glGetUniformLocation(this->programId, this->uniforms.texture.name.c_str());
	this->uniforms.useTexture.location = glGetUniformLocation(this->programId, this->uniforms.useTexture.name.c_str());
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

	// Set once and kept like that.
	// If it can be changed dinamically, it should be updated here.
	// this->uniforms.useTexture.value = false;

	// this->uniforms.skybox.value = ???;
	// this->uniforms.cubeMap.value = ???;
	// this->uniforms.texture.value = ???;
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

void Shader::bindTexture(const Texture texture) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glUniform1i(this->uniforms.texture.location, texture.id);
	// The shader must read the texture from texture unit 0.
	glUniform1i(this->uniforms.texture.location, 0); // sampler2D -> texture unit 0
}

void Shader::bindNoTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
