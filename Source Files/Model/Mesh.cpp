#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Game Objects/Texture.h"
#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>

Mesh::Mesh(const string name, const BufferValues values, const shared_ptr<Shader> shader, const Transform transform, const MaterialType material, const TextureType texture)
{
	this->name = name;
	this->values = values;
	this->shader = shader;
	this->transform = transform;
	this->material = material;
	this->texture = texture;
}

void Mesh::render(Transform modelTransform) const
{
	if (this->material != MaterialType::CUSTOM_MATERIAL)
		this->shader->render(modelTransform, this->transform, this->values, MaterialsFactory::makeMaterial(this->material));
	else
		this->shader->render(modelTransform, this->transform, this->values, *this->customMaterial);
}

string Mesh::getName() const
{
	return this->name;
}

void Mesh::setShader(const shared_ptr<Shader> shader)
{
	this->shader = shader;
}

MaterialType Mesh::getMaterialType() const
{
	return this->material;
}

void Mesh::setMaterialType(const MaterialType material)
{
	this->material = material;
}

shared_ptr<Material> Mesh::getCustomMaterial() const
{
	return this->customMaterial;
}

void Mesh::setTextureType(const TextureType texture)
{
	this->texture = texture;
}

TextureType Mesh::getTextureType() const
{
	return this->texture;
}

Transform Mesh::getTransform() const
{
	return this->transform;
}

void Mesh::setTransform(const Transform transform)
{
	this->transform = transform;
}

ShaderType Mesh::getShaderType() const
{
	return this->shader->getShaderType();
}
