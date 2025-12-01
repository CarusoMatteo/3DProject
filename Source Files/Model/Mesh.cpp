#include "../../Header Files/Game Objects/Material.h"
#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>

Mesh::Mesh(const string name, const BufferValues values, const ShaderFiles files, const Transform transform, const Material material)
{
	this->name = name;
	this->values = values;
	this->shader = unique_ptr<Shader>(new Shader(files, values));
	this->transform = transform;
	this->material = material;
}

void Mesh::render(Transform modelTransform) const
{
	this->shader->render(modelTransform, this->transform, this->values, this->material);
}
