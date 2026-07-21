#include "../../Header Files/Model/CubeMapModel.h"
#include "../../Header Files/Model/CubeMapMesh.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>

CubeMapModel::CubeMapModel(const Transform transform, const shared_ptr<CubeMapMesh> mesh)
	: transform(transform), mesh(mesh)
{
}

void CubeMapModel::render() const
{
	this->mesh->render(this->transform);
}

Transform CubeMapModel::getTransform() const
{
	return this->transform;
}

shared_ptr<CubeMapMesh> CubeMapModel::getMesh() const
{
	return this->mesh;
}

void CubeMapModel::setTransform(const Transform transform)
{
	this->transform = transform;
}
