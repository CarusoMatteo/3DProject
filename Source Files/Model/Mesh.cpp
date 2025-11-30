#include "../../Header Files/Model/Buffers.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>

Mesh::Mesh(const BufferValues values, const ShaderFiles files, const Transform transform)
{
	this->values = values;
	this->shader = unique_ptr<Shader>(new Shader(files, values));
	this->transform = transform;
}

void Mesh::render()
{
	this->shader->render(this->transform, this->values);
}
