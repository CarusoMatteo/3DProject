#include "../../Header Files/Model/CubeMapMesh.h"
#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Texture.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>

CubeMapMesh::CubeMapMesh(const string name, const shared_ptr<Shader> shader, const Transform transform, const Texture texture)
{
	this->name = name;
	this->shader = shader;
	this->transform = transform;
	this->texture = texture;
}

void CubeMapMesh::render(Transform modelTransform) const
{
	this->shader->render(modelTransform, this->transform, this->values, *this->customMaterial);
}

string CubeMapMesh::getName() const
{
	return this->name;
}

void CubeMapMesh::setShader(const shared_ptr<Shader> shader)
{
	this->shader = shader;
}

MaterialType CubeMapMesh::getMaterialType() const
{
	return this->material;
}

void CubeMapMesh::setMaterialType(const MaterialType material)
{
	this->material = material;
}

shared_ptr<Material> CubeMapMesh::getCustomMaterial() const
{
	return this->customMaterial;
}

void CubeMapMesh::setTexture(const Texture texture)
{
	this->texture = texture;
}

Texture CubeMapMesh::getTexture() const
{
	return this->texture;
}

Transform CubeMapMesh::getTransform() const
{
	return this->transform;
}

void CubeMapMesh::setTransform(const Transform transform)
{
	this->transform = transform;
}
