#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Model.h"
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
