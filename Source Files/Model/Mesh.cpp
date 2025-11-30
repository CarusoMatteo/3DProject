#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

Mesh::Mesh(const vector<fvec3> vertices, const vector<fvec4> colors, const vector<unsigned int> indices, const ShaderFiles files, const Transform transform)
{
	this->vertices = vertices;
	this->colors = colors;
	this->indices = indices;
	this->shader = unique_ptr<Shader>(new Shader(files));
	this->transform = transform;
}

void Mesh::render()
{
	this->shader->render();
}
