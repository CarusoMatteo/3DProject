#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <vector>

Model::Model(const Transform transform, const vector<shared_ptr<Mesh>> meshes)
{
	this->transform = transform;
	this->meshes = meshes;
}

void Model::render() const
{
	for (const auto &mesh : meshes)
	{
		mesh->render(this->transform);
	}
}

Transform Model::getTransform() const
{
	return this->transform;
}

void Model::setTransform(const Transform transform)
{
	this->transform = transform;
}

vector<shared_ptr<Mesh>> Model::getMeshes() const
{
	return this->meshes;
}
