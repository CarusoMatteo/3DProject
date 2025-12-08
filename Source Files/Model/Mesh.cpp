#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>

Mesh::Mesh(const string name, const BufferValues values, const shared_ptr<Shader> shader, const Transform transform, const Material material)
{
	this->name = name;
	this->values = values;
	this->shader = shader;
	this->transform = transform;
	this->material = material;
}

void Mesh::render(Transform modelTransform) const
{
	this->shader->render(modelTransform, this->transform, this->values, this->material);
}
