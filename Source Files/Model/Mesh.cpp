#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Texture.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>

Mesh::Mesh(const string name, const BufferValues values, const shared_ptr<Shader> shader, const Transform transform, const Material material, const optional<shared_ptr<Texture>> texture)
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
	this->shader->render(modelTransform, this->transform, this->values, this->material, this->texture);
}

string Mesh::getName() const
{
	return this->name;
}

void Mesh::setShader(const shared_ptr<Shader> shader)
{
	this->shader = shader;
}

Material Mesh::getMaterial() const
{
	return this->material;
}

void Mesh::setMaterial(const Material material)
{
	this->material = material;
}

void Mesh::setTexture(const shared_ptr<Texture> texture)
{
	this->texture = texture;
}

optional<shared_ptr<Texture>> Mesh::getTexture() const
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
